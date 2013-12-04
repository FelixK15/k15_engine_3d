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

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_Texture.h"
#include "K15_TextureSampler.h"
#include "K15_GpuProgram.h"
#include "K15_MemoryPools.h"
#include "K15_AllocatedObject.h"

namespace K15_Engine { namespace Rendering { 
	class K15_CORE_API Material : public AllocatedObject<MaterialAllocator>
	{
	public:
		Material();
		~Material();

		inline const ColorRGBA& getSpecular() const;
		inline const ColorRGBA& getDiffuse() const;
		inline const ColorRGBA& getAmbient() const;
		inline float getShininess() const;

		inline GpuProgram* getProgram(Enum p_Stage) const;
		inline Texture* getDiffuseMap() const;
		inline TextureSampler* getDiffuseSampler() const;

		inline void setSpecular(const ColorRGBA& p_Specular);
		inline void setDiffuse(const ColorRGBA& p_Diffuse);
		inline void setAmbient(const ColorRGBA& p_Ambient);
		inline void setShininess(float p_Shininess);

		inline void setDiffuseMap(Texture* p_DiffuseMap);
		inline void setDiffuseSampler(TextureSampler* p_DiffuseSampler);
		inline void setProgram(GpuProgram* p_Program, Enum p_Stage);
		
// 		Texture* getNormalMap() const;
// 		Texture* getGlossMap() const;

	private:
		expose GpuProgram* m_Programs[GpuProgram::PS_COUNT];
		expose Texture* m_DiffuseMap;
		expose TextureSampler* m_DiffuseSampler;

		expose ColorRGBA m_Specular;
		expose ColorRGBA m_Diffuse;
		expose ColorRGBA m_Ambient;
		expose float m_Shininess;
	}; // end of Material class declaration
#include "K15_Material.inl"
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_TextureSampler_h_