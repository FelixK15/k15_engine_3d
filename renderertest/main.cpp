#include "K15_System.h"
#include "K15_Renderer.h"

#include "K15_Geometry.h"
#include "K15_Buffer.h"
#include "K15_VertexLayout.h"

#include "K15_PixelShader.h"
#include "K15_ShaderVariable.h"

#include "K15_Game.h"
#include "K15_Camera.h"

#include "K15_Importer.h"

#include "K15_Light.h"

using namespace K15_EngineV2;
using namespace K15_Renderer;

class ObjectParameter : public ShaderVariable
{
	virtual void Update(Geometry *pGeometry)
	{
		Camera *pCamera = g_pRenderer->GetCamera();
		const U32 iBufferSize = (sizeof(Matrix4x4F) * 2) + sizeof(Material);
		U32 Offset = 0;
		char buffer[iBufferSize];

		Matrix4x4F matWorldViewProj = pGeometry->Transformation.Matrix();
		Memory::MemCopy(&buffer,&matWorldViewProj.m_fMatrix,sizeof(Matrix4x4F));
		Offset += sizeof(Matrix4x4F);
		matWorldViewProj *= pCamera->GetViewMatrix();
		matWorldViewProj *= pCamera->GetProjectionMatrix();

		matWorldViewProj.Transpose();
		Memory::MemCopy(&buffer[Offset],&matWorldViewProj.m_fMatrix,sizeof(Matrix4x4F));
		Offset += sizeof(Matrix4x4F);
		Memory::MemCopy(&buffer[Offset],&pGeometry->Material,sizeof(Material));
		SetData((float*)&buffer);
	}
};

class TestParameter : public ShaderVariable
{
	virtual void Update(Geometry *pGeometry) 
	{
		Camera *pCamera = g_pRenderer->GetCamera();

		Matrix4x4F matWorldViewProj = pGeometry->Transformation.Matrix();
		matWorldViewProj *= pCamera->GetViewMatrix();
		matWorldViewProj *= pCamera->GetProjectionMatrix();

		matWorldViewProj.Transpose();
		SetData((float*)&matWorldViewProj);
	}
};

struct Vertex
{
	Vector3F Position;
	Vector3F Normal;

	Vertex()
	{

	}

	Vertex(const Vector3F &Pos,const Vector3F &Norm)
	{
		Position = Pos;
		Normal = Norm;
	}
};

class TestGame : public Game
{
	virtual bool Initialize() 
	{
// 		Vertex vertices[] = 
// 		{
// 			Vertex(Vector3F(-1.f,-1.f,-1.f),Vector3F(0.f,0.f,0.f)),		//	front left bottm
// 			Vertex(Vector3F(-1.f,+1.f,-1.f),Vector3F(0.f,0.f,0.f)),     //	front right bottom
// 			Vertex(Vector3F(+1.f,+1.f,-1.f),Vector3F(0.f,0.f,0.f)),       //	front right top
// 			Vertex(Vector3F(+1.f,-1.f,-1.f),Vector3F(0.f,0.f,0.f)),		//	front left top
// 
// 
// 			Vertex(Vector3F(-1.f,-1.f,+1.f),Vector3F(0.f,0.f,0.f)),		//	back left bottom
// 			Vertex(Vector3F(-1.f,+1.f,+1.f),Vector3F(0.f,0.f,0.f)),		//	back right bottom
// 			Vertex(Vector3F(+1.f,+1.f,+1.f),Vector3F(0.f,0.f,0.f)),			//	back right top
// 			Vertex(Vector3F(+1.f,-1.f,+1.f),Vector3F(0.f,0.f,0.f)),		//	back left top
// 		};
// 
// 		U32 indices[] =
// 		{
// 			0,1,2,
// 			0,2,3,
// 
// 			4,6,5,
// 			4,7,6,
// 
// 			4,5,1,
// 			4,1,0,
// 
// 			3,2,6,
// 			3,6,7,
// 
// 			1,5,6,
// 			1,6,2,
// 			4,0,3,
// 			4,3,7
// 		};
		
		vsShader = g_pRenderer->CreateVertexShader("shader.fx","VS",VertexShader::SP_VS_4_0);
		psShader = g_pRenderer->CreatePixelShader("shader.fx","PS",PixelShader::SP_PS_4_0);

// 		for(int i = 0;i<12;++i){
// 			U32 i0 = indices[i*3+0];
// 			U32 i1 = indices[i*3+1];
// 			U32 i2 = indices[i*3+2];
// 
// 			Vertex &v0 = vertices[i0];
// 			Vertex &v1 = vertices[i1];
// 			Vertex &v2 = vertices[i2];
// 
// 			Vector3F e0 = v1.Position - v0.Position;
// 			Vector3F e1 = v2.Position - v0.Position;
// 			Vector3F faceNormal = e0.Cross(e1);
// 
// 			v0.Normal += faceNormal;
// 			v1.Normal += faceNormal;
// 			v2.Normal += faceNormal;
// 		}
// 
// 		for(int i = 0;i < 8;++i){
// 			vertices[i].Normal.Normalize();
// 		}

		VertexLayoutPtr vLayout = g_pRenderer->CreateVertexLayout(1,vsShader,FormatType::FT_32R32G32B_FLOAT,VertexLayout::VS_POSITION);

		float *vertices = NULL;
		U32 *indices = NULL;

		U32 iVertexCount = 0;
		U32 iIndexCount = 0;

		Importer::ImportOBJ("Weapon.obj",vLayout,vertices,indices,iVertexCount,iIndexCount);

		vbBuffer = g_pRenderer->CreateBuffer(vertices,iVertexCount * vLayout->GetSizeOfVertex(),vLayout->GetSizeOfVertex(),FormatType::FT_UNKNOWN,Buffer::BU_IMMUTABLE,Buffer::BA_NO_ACCESS,Buffer::BB_VERTEX_BUFFER);
		idBuffer = g_pRenderer->CreateBuffer(indices,iIndexCount * sizeof(U32),sizeof(U32),FormatType::FT_32R_UINT,Buffer::BU_IMMUTABLE,Buffer::BA_NO_ACCESS,Buffer::BB_INDEX_BUFFER);


		pBlock = K15_NEW Geometry(vbBuffer,idBuffer,vsShader,psShader,Geometry::PT_TRIANGLELIST);
		TestParameter *pTestParameter = K15_NEW TestParameter();
		pTestParameter->SetByteSize(sizeof(Matrix4x4F));
		pTestParameter->SetUpdateFrequenzy(ShaderVariable::UF_PER_OBJECT);

		pBlock->Material.Ambient = ColorRGBA(0.48,0.77,0.45,1.f);
		pBlock->Material.Diffuse = ColorRGBA(0.48,0.77,0.45,1.f);
		pBlock->Material.Specular = ColorRGBA(0.2,0.2,0.2,1.f);
		pBlock->Transformation.Translate(Vector3F(0.0f,0.0f,-5.f));
		vsShader->AddShaderVariable("WorldMatrix",pTestParameter);

		pLight = K15_NEW Light(LT_DIRECT);
		pLight->Position.x = 0.0;
		pLight->Position.y = 0.0;
		pLight->Position.z = -2.0f;

		pLight->Direction.z = 1;

		pLight->Range = 5.f;
		pLight->Ambient.R = 0.2;
		pLight->Ambient.G = 0.2;
		pLight->Ambient.B = 0.2;
		pLight->Ambient.A = 1.f;

		pLight->Specular.R = 0.5;
		pLight->Specular.G = 0.5;
		pLight->Specular.B = 0.5;
		pLight->Specular.A = 1.f;

		pLight->Diffuse.R = 0.5;
		pLight->Diffuse.G = 0.5;
		pLight->Diffuse.B = 0.5;
		pLight->Diffuse.A = 1.f;

		pLight->Attenuation0 = 0.1;
		pLight->Attenuation1 = 0.2;
		pLight->Attenuation2 = 0.2;

		g_pRenderer->AddLight(pLight);
		return true;
	}

	virtual void Update( const GameTime &gtGameTime ) 
	{
		//pText->Transformation.Rotate(Vector3(0.f,1.f,0.f),0.0005f);
		pBlock->Transformation.Rotate(Vector3F(0.f,1.f,0.f),0.0002f);

	//	g_pRenderer->Draw(pText);
		g_pRenderer->Draw(pBlock);
		g_pRenderer->Update();
	}

	virtual void Shutdown() 
	{
		K15_DELETE pText;
		K15_DELETE pBlock;
	}

	ColorRGBA Specular;

	Light *pLight;

	Geometry *pText;
	Geometry *pBlock;
	PixelShaderPtr psShader2;
	VertexShaderPtr vsShader2;
	PixelShaderPtr psShader;
	VertexShaderPtr vsShader;
	BufferPtr idBuffer;
	BufferPtr vbBuffer;
	BufferPtr coBuffer;

	BufferPtr idBuffer2;
	BufferPtr vbBuffer2;
	BufferPtr coBuffer2;
};

int main()
{
	if(g_pSystem->Initialize(System::SF_ENABLE_MEMORY_LOGGING)){
		g_pSystem->InitializeWindow("Renderer Test",System::WF_FRAME,0,0,800,600);
		g_pRenderer->Initialize(0,0,800,600,false);
		g_pSystem->SetGame(K15_NEW TestGame());
		g_pSystem->Run();
		g_pRenderer->Shutdown();
		g_pSystem->Shutdown();
	}

	return 0;
}