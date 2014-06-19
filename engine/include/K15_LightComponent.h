/**
 * @file K15_LightComponent.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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

#ifndef _K15Engine_Core_LightComponent_h_
#define _K15Engine_Core_LightComponent_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_GameObjectComponentBase.h"
#include "K15_ColorRGBA.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API LightComponent : public GameObjectComponentBase
	{
		K15_DECLARE_RTTI;

	public:
		/*********************************************************************************/
		enum eLightType
		{
			LT_POINT = 0,		//<! Point Light
			LT_SPOT,			  //<! Spot Light
			LT_DIRECTIONAL,	//<! Directional Light

			LT_COUNT
		}; //LightType
		/*********************************************************************************/

	public:
		LightComponent();
		~LightComponent();

		INLINE Enum getLightType() const;
		INLINE bool isActive() const;
		INLINE float getRadius() const;
		INLINE const ColorRGBA& getDiffuseColor() const;
    INLINE const ColorRGBA& getSpecularColor() const;
		INLINE bool isDirty() const;

		INLINE void setLightType(Enum p_LightType);
		INLINE void setRadius(float p_Radius);
		INLINE void setDiffuseColor(const ColorRGBA& p_DiffuseColor);
    INLINE void setSpecularColor(const ColorRGBA& p_SpecularColor);
		INLINE void setActive(bool p_Active);
		
		virtual void onAddedToRenderQueue(RenderQueue* p_RenderQueue);

	private:
		expose ColorRGBA m_DiffuseColor;
    expose ColorRGBA m_SpecularColor;
		expose float m_Range;
		expose_enum(eLightType) Enum m_LightType;
		expose bool m_Active;
		bool m_Dirty;
	};// end of LightComponent class declaration

	K15_PTR(LightComponent);
	K15_WEAKPTR(LightComponent);

#	include "K15_LightComponent.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_LightComponent_h_