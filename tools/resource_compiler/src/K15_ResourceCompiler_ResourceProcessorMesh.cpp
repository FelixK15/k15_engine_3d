#include <K15_HeaderFormat.h>
#include <K15_MeshFormat.h>
#include <K15_MaterialFormat.h>
#include <K15_DataAccessHelper.h>

#include <enums/K15_ReturnValues.h>
#include <enums/K15_FormatValues.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "K15_ResourceCompiler_ResourceProcessorMesh.h"
#include "K15_ResourceCompiler_StringUtil.h"
#include "K15_ResourceCompiler_ResourceData.h"


namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	namespace internal
	{
		bool saveMesh(ResourceProcessorMesh* p_MeshProcessor, const String& p_OutputPath, const aiScene* p_SceneData);
		bool saveMaterial(ResourceProcessorMesh* p_MeshProcessor, const String& p_OutputPath, const aiScene* p_SceneData, ResourceProcessResult* p_ProcessResult);
		void checkTextureDependencies(aiTextureType p_TextureType, aiMaterial* p_Material, ResourceData& p_ResourceData);
		char convertTextureMapping(int p_InputMapping);
	}
	/*********************************************************************************/



	/*********************************************************************************/
	ResourceProcessorMesh::ResourceProcessorMesh(const CompilerEnvironment& p_CompilerEnvironment)
		: ResourceProcessorBase(p_CompilerEnvironment)
	{

	}
	/*********************************************************************************/
	ResourceProcessorMesh::~ResourceProcessorMesh()
	{

	}
	/*********************************************************************************/
	ProcessorEnvironment ResourceProcessorMesh::getProcessorEnvironment() const
	{
		static ProcessorEnvironment environment = {".k15mesh", "Mesh Processor"};

		return environment;
	}
	/*********************************************************************************/
	bool ResourceProcessorMesh::canProcess(const String& p_FileExtension) const
	{
		static String extensions[] = {".dae", ".blend", ".3ds", ".ase", ".obj", 
			".ifc", ".xgl", ".zgl", ".ply", ".dxf", 
			".lwo", ".lws", ".lxo", ".stl", ".x", 
			".ac", ".ms3d", ".cob", ".scn", ".irrmesh"};

		static StringSet assimpExtensions = StringSet(extensions, extensions + sizeof(extensions) / sizeof(extensions[0]));

		return std::find(assimpExtensions.begin(), assimpExtensions.end(), p_FileExtension) != assimpExtensions.end();
	}
	/*********************************************************************************/
	ResourceProcessResult ResourceProcessorMesh::process(const ResourceData& p_File)
	{
		ResourceProcessResult result;
		result.processState = RPS_FAILURE;

		Assimp::Importer importer;

		String meshOutputPath = getCompilerEnvironment().getOutputPath();
		meshOutputPath += StringUtil::getFileNameWithoutExtension(p_File.filePath);
		meshOutputPath += getProcessorEnvironment().outputExtension;

		String materialOutputPath = getCompilerEnvironment().getOutputPath();
		materialOutputPath += StringUtil::getFileNameWithoutExtension(p_File.filePath);
		materialOutputPath += ".k15material";

		const aiScene* scene = importer.ReadFile(p_File.filePath, 
			aiProcess_CalcTangentSpace			|
			aiProcess_Triangulate				|
			aiProcess_GenNormals				|
			aiProcess_JoinIdenticalVertices		|
			aiProcess_SortByPType);

		if(!scene)
		{
			setError(importer.GetErrorString());
			return result;
		}

		uint32 sceneFlags = scene->mFlags;

		if((sceneFlags & AI_SCENE_FLAGS_INCOMPLETE) > 0)
		{
			delete scene;
			setError(importer.GetErrorString());
			return result;
		}

		bool meshSavedSuccessfully = internal::saveMesh(this, meshOutputPath, scene);
		bool materialsSaveSuccessfully = internal::saveMaterial(this, materialOutputPath, scene, &result);
		
		//don't delete scene - will get deleted automatically.
		//delete scene;

		result.processState = meshSavedSuccessfully && materialsSaveSuccessfully ? RPS_SUCCESS : RPS_FAILURE;

		return result;
	}
	/*********************************************************************************/
	ResourceData ResourceProcessorMesh::gatherData(const String& p_File) const 
	{
		Assimp::Importer importer;
		ResourceData resourceData;

		resourceData.filePath = p_File;
		resourceData.type = RT_Mesh;

		const aiScene* scene = importer.ReadFile(p_File, aiProcess_CalcTangentSpace			|
														 aiProcess_Triangulate				|
														 aiProcess_GenNormals				|
														 aiProcess_JoinIdenticalVertices	|
														 aiProcess_SortByPType);

		if (scene)
		{
			uint32 amountMaterials = scene->mNumMaterials;
			uint32 amountDiffuseTextures = 0;

			aiString texturePath;

			aiMaterial* material = 0;

			for (uint32 materialIndex = 0;
				materialIndex < amountMaterials;
				++materialIndex)
			{
				material = scene->mMaterials[materialIndex];

				internal::checkTextureDependencies(aiTextureType_DIFFUSE, material, resourceData);
				internal::checkTextureDependencies(aiTextureType_NORMALS, material, resourceData);
				internal::checkTextureDependencies(aiTextureType_SPECULAR, material, resourceData);
			}
		}

		return resourceData;
	}
	/*********************************************************************************/
	bool internal::saveMesh(ResourceProcessorMesh* p_MeshProcessor, const String& p_OutputPath, const aiScene* p_SceneData)
	{
		uint32 amountSubmeshes = p_SceneData->mNumMeshes;

		K15_MeshFormat meshFormat;

		K15_SetMeshName(&meshFormat, StringUtil::getFileNameWithoutExtension(p_OutputPath).c_str());
		K15_SetMeshSubmeshCount(&meshFormat, amountSubmeshes);

		for(uint32 submeshIndex = 0;
			submeshIndex < amountSubmeshes;
			++submeshIndex)
		{
			aiMesh* mesh = p_SceneData->mMeshes[submeshIndex];
			
			uint32 amountColorChannel = mesh->GetNumColorChannels();
			uint32 amountTextureChannel = mesh->GetNumUVChannels();

			K15_SetSubmeshVertexCount(&meshFormat.submeshes[submeshIndex], mesh->mNumVertices);
			K15_SetSubmeshTriangleCount(&meshFormat.submeshes[submeshIndex], mesh->mNumFaces);
			K15_SetSubmeshMaterial(&meshFormat.submeshes[submeshIndex], mesh->mMaterialIndex);

			K15_SetSubmeshData(&meshFormat.submeshes[submeshIndex], (float*)mesh->mVertices, K15_MESH_DATA_POSITION);
			K15_SetSubmeshData(&meshFormat.submeshes[submeshIndex], (float*)mesh->mNormals, K15_MESH_DATA_NORMAL);
			K15_SetSubmeshData(&meshFormat.submeshes[submeshIndex], (float*)mesh->mTangents, K15_MESH_DATA_TANGENT);
			K15_SetSubmeshData(&meshFormat.submeshes[submeshIndex], (float*)mesh->mBitangents, K15_MESH_DATA_BITANGENT);

			for(uint32 faceIndex = 0;
				faceIndex < mesh->mNumFaces;
				++faceIndex)
			{
				K15_SetSubmeshTriangleData(&meshFormat.submeshes[submeshIndex], faceIndex, mesh->mFaces[faceIndex].mIndices);
			}

			K15_SetSubmeshChannelCount(&meshFormat.submeshes[submeshIndex], amountColorChannel, K15_MESH_DATA_COLOR);
			K15_SetSubmeshChannelCount(&meshFormat.submeshes[submeshIndex], amountTextureChannel, K15_MESH_DATA_TEXCOORD);

			for(uint32 colorChannelIndex = 0; 
				colorChannelIndex < amountColorChannel; 
				++colorChannelIndex)
			{
				K15_SetSubmeshDataWithIndex(colorChannelIndex, &meshFormat.submeshes[submeshIndex], (float*)mesh->mColors[colorChannelIndex], K15_MESH_DATA_COLOR);
			}

			for(uint32 textureChannelIndex = 0;
				textureChannelIndex < amountTextureChannel;
				++textureChannelIndex)
			{
				K15_SetSubmeshDataWithIndex(textureChannelIndex, &meshFormat.submeshes[submeshIndex], (float*)mesh->mTextureCoords[textureChannelIndex], K15_MESH_DATA_TEXCOORD);
			}
		}

		K15_DataAccessContext accessContext;

		if (K15_CreateDataAccessContextFromFile(&accessContext, p_OutputPath.c_str()) == K15_SUCCESS)
		{
			//error
		}

		uint8 result = K15_InternalSaveMeshFormat(&meshFormat, &accessContext, K15_SAVE_FLAG_FREE_DATA);

		return result == K15_SUCCESS;
	}
	/*********************************************************************************/
	bool internal::saveMaterial(ResourceProcessorMesh* p_MeshProcessor, const String& p_OutputPath, const aiScene* p_SceneData, ResourceProcessResult* p_ProcessResult)
	{
		uint32 amountMaterials = p_SceneData->mNumMaterials;

		//free memory
		K15_MaterialFormat materialFormat;
		
		K15_SetMaterialName(&materialFormat, StringUtil::getFileNameWithoutExtension(p_OutputPath).c_str());
		K15_SetSubMaterialCount(&materialFormat, amountMaterials);

		for(uint32 materialIndex = 0;
			materialIndex < amountMaterials;
			++materialIndex)
		{
			K15_SubMaterialFormat* subMaterialFormat = &materialFormat.subMaterials[materialIndex];
			aiMaterial* material = p_SceneData->mMaterials[materialIndex];
			aiColor3D diffuseColor;
			aiColor3D specularColor;
			aiColor3D ambientColor;
			aiString tempTexturePath;
			String textureFileName;
			String texturePath;

			float shininess;
			float shininessStrength;
			float textureBlendStrength;
			int textureMappingU;
			int textureMappingV;
			int textureBlendOperation;
			int textureFlags;

			material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
			material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
			material->Get(AI_MATKEY_SHININESS, shininess);
			material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);

			///set color
			subMaterialFormat->diffuseColor[0] = diffuseColor.r;
			subMaterialFormat->diffuseColor[1] = diffuseColor.g;
			subMaterialFormat->diffuseColor[2] = diffuseColor.b;

			subMaterialFormat->specularColor[0] = specularColor.r;
			subMaterialFormat->specularColor[1] = specularColor.g;
			subMaterialFormat->specularColor[2] = specularColor.b;

			subMaterialFormat->ambientColor[0] = ambientColor.r;
			subMaterialFormat->ambientColor[1] = ambientColor.g;
			subMaterialFormat->ambientColor[2] = ambientColor.b;

			subMaterialFormat->shininess = shininess;
			subMaterialFormat->sininessStrength = shininessStrength;


			uint32 diffuseTextureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
//TODO: Add Later
// 			uint32 specularTextureCoint = material->GetTextureCount(aiTextureType_SPECULAR);
// 			uint32 diffuseTextureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
// 			uint32 diffuseTextureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
// 			uint32 diffuseTextureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
// 			uint32 diffuseTextureCount = material->GetTextureCount(aiTextureType_DIFFUSE);

			K15_SetSubMaterialTextureCount(subMaterialFormat, diffuseTextureCount, K15_MATERIAL_TEXTURE_DIFFUSE);

			for(uint32 diffuseTextureIndex = 0;
				diffuseTextureIndex < diffuseTextureCount;
				++diffuseTextureIndex)
			{			
				K15_SubMaterialTextureFormat* subMaterialTextureFormat = &subMaterialFormat->diffuseTextureFormat[diffuseTextureIndex];

				material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, diffuseTextureIndex), tempTexturePath);
				material->Get(AI_MATKEY_TEXBLEND(aiTextureType_DIFFUSE, diffuseTextureIndex), textureBlendStrength);
				material->Get(AI_MATKEY_TEXOP(aiTextureType_DIFFUSE, diffuseTextureIndex), textureBlendOperation);
				material->Get(AI_MATKEY_MAPPINGMODE_U(aiTextureType_DIFFUSE, diffuseTextureIndex), textureMappingU);
				material->Get(AI_MATKEY_MAPPINGMODE_V(aiTextureType_DIFFUSE, diffuseTextureIndex), textureMappingV);
				material->Get(AI_MATKEY_TEXFLAGS(aiTextureType_DIFFUSE, diffuseTextureIndex), textureFlags);
				
				//texture path is a local path
				texturePath = (tempTexturePath.C_Str() + 1); //+1 as we don't want the leading folder slash
				textureFileName = StringUtil::getFileName(texturePath);

				texturePath = p_MeshProcessor->getCompilerEnvironment().getOutputPath() + texturePath;

				K15_SetSubMaterialTextureFormatTextureName(subMaterialTextureFormat, textureFileName.c_str());

				subMaterialTextureFormat->blendOperation = (uint8)textureBlendOperation;
				subMaterialTextureFormat->blendStrength = textureBlendStrength;

				// convert texture mapping values to the ones used by the format
				subMaterialTextureFormat->textureMappingU = internal::convertTextureMapping(textureMappingU);
				subMaterialTextureFormat->textureMappingV = internal::convertTextureMapping(textureMappingV);
				subMaterialTextureFormat->hasAlpha = (uint8)((textureFlags & aiTextureFlags_UseAlpha) > 0 ? 1 : 0);

				p_ProcessResult->resourceDependencies.insert(texturePath);
			}
		}

		K15_DataAccessContext accessContext;

		if (K15_CreateDataAccessContextFromFile(&accessContext, p_OutputPath.c_str()) == K15_SUCCESS)
		{
			//error
		}

		uint8 result = K15_InternalSaveMaterialFormat(&accessContext, &materialFormat, K15_SAVE_FLAG_FREE_DATA);

		return result == K15_SUCCESS;
	}
	/*********************************************************************************/
	char internal::convertTextureMapping(int p_InputMapping)
	{
		switch(p_InputMapping)
		{
		case aiTextureMapMode_Wrap:
			{
				return K15_MATERIAL_TEXTURE_MAP_WRAP;
			}
		
		case aiTextureMapMode_Clamp:
			{
				return K15_MATERIAL_TEXTURE_MAP_CLAMP;
			}

		case aiTextureMapMode_Mirror:
			{
				return K15_MATERIAL_TEXTURE_MAP_MIRROR;
			}
		}

		return 0;
	}
	/*********************************************************************************/
	void internal::checkTextureDependencies(aiTextureType p_TextureType, aiMaterial* p_Material, ResourceData& p_ResourceData)
	{
		uint32 amountTexture = p_Material->GetTextureCount(p_TextureType);
		aiString texturePath;

		for (uint32 textureIndex = 0;
			textureIndex < amountTexture;
			++textureIndex)
		{
			ResourceData dependencyResourceData = {};
			ResourceParameter resourceParameter = {};

			resourceParameter.name = PARAMETER_TYPE_DIFFUSE_TEXTURE;

			p_Material->Get(AI_MATKEY_TEXTURE(p_TextureType, textureIndex), texturePath);

			//fill dependency data
			dependencyResourceData.filePath = texturePath.C_Str();
			dependencyResourceData.type = RT_Texture;
			dependencyResourceData.parameters.push_back(resourceParameter);

			p_ResourceData.dependencies.push_back(dependencyResourceData);
		}
	}
	/*********************************************************************************/
}