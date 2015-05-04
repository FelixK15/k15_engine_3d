/**
 * @file K15_Material.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/07
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_Material_h_
#define _K15Engine_Renderer_Material_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_ResourceBase.h"
#include "K15_Texture.h"
#include "K15_TextureSampler.h"
#include "K15_GpuProgram.h"

#include "K15_DepthState.h"
#include "K15_AlphaState.h"

namespace K15_Engine { namespace Rendering { 
	/*********************************************************************************/
	class K15_CORE_API MaterialPass : public Object
	{
	public:
		typedef FixedArray(GpuProgram*,GpuProgram::PS_COUNT) GpuProgramArray;

	public:
		MaterialPass();
		~MaterialPass();

		INLINE GpuProgram* getProgram(Enum p_Stage) const;
		INLINE GpuProgramBatch* getProgramBatch() const;

		INLINE void setProgram(GpuProgram* p_Program, Enum p_Stage);
		INLINE void setProgramBatch(GpuProgramBatch* p_ProgramBatch);

		INLINE ColorRGBA& getSpecular();
		INLINE ColorRGBA& getDiffuse();
		INLINE ColorRGBA& getAmbient();
		INLINE float getShininess() const;

		INLINE Texture* getDiffuseMap() const;
		INLINE Texture* getNormalMap() const;

		INLINE TextureSampler* getDiffuseSampler() const;
		INLINE TextureSampler* getNormalSampler() const;

		INLINE void setSpecular(const ColorRGBA& p_Specular);
		INLINE void setDiffuse(const ColorRGBA& p_Diffuse);
		INLINE void setAmbient(const ColorRGBA& p_Ambient);
		INLINE void setShininess(float p_Shininess);

		INLINE void setFillMode(Enum p_FillMode);
		INLINE Enum getFillMode() const;

		INLINE void setDiffuseMap(Texture* p_DiffuseMap);
		INLINE void setNormalMap(Texture* p_NormalMap);

		INLINE void setDiffuseSampler(TextureSampler* p_DiffuseSampler);
		INLINE void setNormalSampler(TextureSampler* p_NormalSampler);

		INLINE bool isTransparent() const;
		INLINE void setTransparent(bool p_Transparent);

		INLINE bool isLightningEnabled() const;
		INLINE void setLightningEnabled(bool p_LightReceive);

		INLINE Enum getCullingMode() const;
		INLINE void setCullingMode(Enum p_CullingMode);

		INLINE bool isDepthTestEnabled() const;
		INLINE void setDepthTestEnabled(bool p_DepthTestEnabled);
		
		INLINE AlphaState& getAlphaState();
		INLINE void setAlphaState(const AlphaState& p_AlphaState);

		INLINE DepthState& getDepthState();
		INLINE void setDepthState(const DepthState& p_DepthState);

		INLINE bool isEnabled() const;
		INLINE void setEnabled(bool p_Enabled);

	private:
		expose GpuProgramArray m_Programs; 
		expose GpuProgramBatch* m_ProgramBatch;
		expose Texture* m_DiffuseMap;
		expose Texture* m_NormalMap;
		expose TextureSampler* m_DiffuseSampler;
		expose TextureSampler* m_NormalSampler;
		expose AlphaState m_AlphaState;
		expose DepthState m_DepthState;

		expose ColorRGBA m_Specular;
		expose ColorRGBA m_Diffuse;
		expose ColorRGBA m_Ambient;
		expose Enum m_FillMode;
		expose Enum m_CullingMode;
		expose float m_Shininess;
		expose bool m_Transparent;
		expose bool m_BackFaceCullingEnabled;
		expose bool m_LightningEnabled;
		expose bool m_PassEnabled;
	}; //end of MaterialPass class declaration
	K15_PTR(MaterialPass);
	/*********************************************************************************/
	class K15_CORE_API Material : public ResourceBase,
								  public RenderingAllocatedObject
	{
		K15_DECLARE_RTTI;

	public:
		/*********************************************************************************/
		static const uint32 MaximumMaterialPasses = 8;
		typedef FixedArray(MaterialPass,MaximumMaterialPasses) MaterialPassArray;
		/*********************************************************************************/
	public:
		Material();
		~Material();

		INLINE uint32 getAmountPasses() const;
		INLINE MaterialPass* getPass(uint32 p_Index, bool p_EnablePass = false);

		INLINE void enablePass(uint32 p_Index);
		INLINE void disablePass(uint32 p_Index);
	private:
		MaterialData* m_Data;
		MaterialPassArray m_Passes;
	}; // end of Material class declaration
	K15_PTR(Material);
#include "K15_Material.inl"
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_Material_h_