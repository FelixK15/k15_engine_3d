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

#include "K15_Prerequisites.h"
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
			LT_POINT = 0,	//<! Point Light
			LT_SPOT,		//<! Spot Light
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
		INLINE float getConeAngle() const;
		INLINE float getConstantAttenuation() const;
		INLINE float getLinearAttenuation() const;
		INLINE float getQuadricAttenuation() const;
		INLINE float getSpotExponent() const;
		INLINE const ColorRGBA& getDiffuseColor() const;
		INLINE const ColorRGBA& getSpecularColor() const;
		INLINE const Vector3& getDirection() const;
		INLINE const Vector3& getAttenuation() const;
		bool isDirty() const;

		INLINE void setLightType(Enum p_LightType);
		INLINE void setRadius(float p_Radius);
		INLINE void setConstantAttenuation(float p_ConstAttenuation);
		INLINE void setLinearAttenuation(float p_LinearAttenuation);
		INLINE void setQuadricAttenuation(float p_QuadricAttenuation);
		INLINE void setConeAngle(float p_ConeAngle);
		INLINE void setDiffuseColor(const ColorRGBA& p_DiffuseColor);
		INLINE void setSpecularColor(const ColorRGBA& p_SpecularColor);
		INLINE void setDirection(const Vector3& p_Direction);
		INLINE void setAttenuation(const Vector3& p_Attenuation);
		INLINE void setSpotExponent(float p_SpotExponent);
		INLINE void setActive(bool p_Active);
		
		virtual void onAddedToRenderQueue(RenderQueue* p_RenderQueue);

	private:
		expose Vector3 m_Direction;
		expose Vector3 m_Attenuation;
		expose ColorRGBA m_DiffuseColor;
		expose ColorRGBA m_SpecularColor;
		expose float m_ConeAngle;
		expose float m_Range;
		expose float m_SpotExponent;
		expose_enum(eLightType) Enum m_LightType;
		expose bool m_Active;
		bool m_Dirty;
	};// end of LightComponent class declaration

	K15_PTR(LightComponent);
	K15_WEAKPTR(LightComponent);

#	include "K15_LightComponent.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_LightComponent_h_