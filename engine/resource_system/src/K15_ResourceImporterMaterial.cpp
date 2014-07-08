/**
 * @file K15_ResourceImporterMaterial.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/05
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "K15_PrecompiledHeader.h"

#include "K15_ResourceImporterMaterial.h"
#include "K15_RendererBase.h"
#include "K15_ResourceManager.h"
#include "K15_Material.h"
#include "K15_GpuProgramBatch.h"
#include "K15_GpuProgramCatalog.h"
#include "K15_RawData.h"
#include "K15_IOUtil.h"


#include "json/reader.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	ResourceImporterMaterial::ResourceImporterMaterial()
		: ResourceImporterBase("Material")
	{

	}
	/*********************************************************************************/
	ResourceImporterMaterial::~ResourceImporterMaterial()
	{

	}
	/*********************************************************************************/
	void ResourceImporterMaterial::getExtensionList(ExtensionSet& p_ExtensionSet)
	{
		p_ExtensionSet.insert(".json");
	}
	/*********************************************************************************/
	void ResourceImporterMaterial::getMagicNumber(MagicNumberSet& p_MagicNumber)
	{

	}
	/*********************************************************************************/
	ResourceBase* ResourceImporterMaterial::_load(const RawData& p_ResourceData, const Rtti& p_ResourceType)
	{
		StringStream stream;
		stream.write((const char*)p_ResourceData.data,p_ResourceData.size);

		Json::Value root;
		Json::Reader reader;
		Material* material = 0;
		if(reader.parse((std::istream&)stream,root,false))
		{
			material = K15_NEW Material();
			material->setName(root["Name"].asString());

			Json::Value& passes = root["Passes"];
			uint32 passCounter = 0;
			for(uint32 i = 0; i < passes.size(); ++i)
			{
				Json::Value& pass = passes[i];
				
				MaterialPass* materialPass = material->getPass(passCounter,true);

				materialPass->setDiffuseMap(g_ResourceManager->getResource<Texture>(pass["DiffuseMap"].asString()));
				
				String gpuProgramBatchName = pass.get("ShaderBatch","").asString();

				if(!gpuProgramBatchName.empty())
				{
					GpuProgramBatch* batch = GpuProgramCatalog::getGpuProgramBatch(gpuProgramBatchName);
					materialPass->setProgramBatch(batch);
				}

				///cullingmode
				String cullingmode = pass.get("CullingMode","CM_CCW").asString();

				if(cullingmode == "CM_CCW")
				{
					materialPass->setCullingMode(RendererBase::CM_CCW);
				}
				else 
				{
					materialPass->setCullingMode(RendererBase::CM_CW);
				}
			}
		}
		else
		{
			setError(reader.getFormattedErrorMessages());
		}

		return material;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace
