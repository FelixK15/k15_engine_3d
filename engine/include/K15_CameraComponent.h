/**
 * @file K15_Camera.h
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

#ifndef _K15Engine_Renderer_Camera_h_
#define _K15Engine_Renderer_Camera_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
# include "K15_Object.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_GameObjectComponentBase.h"
#include "K15_Matrix4.h"

namespace K15_Engine { namespace Rendering {
  class K15_CORE_API CameraComponent : public GameObjectComponentBase
  {
	/*********************************************************************************/
	K15_DECLARE_RTTI;
	/*********************************************************************************/
  public:
	/*********************************************************************************/
	enum eProjectionType
	{
		PT_PERSPECTIVE = 0,
		PT_ORTHOGRAPHIC,

		PT_COUNT
	}; //ProjectionType
	/*********************************************************************************/

  public:
    CameraComponent(GameObject* p_Parent);
    virtual ~CameraComponent();
  
    INLINE bool getActive() const;
    INLINE float getFov() const;
    INLINE float getFarClipDistance() const;
    INLINE float getNearClipDistance() const;

    const Math::Matrix4& getProjectionMatrix();
    const Math::Matrix4& getViewMatrix();
    
    void setActive(bool p_Active);
    INLINE void setFieldOfView(float p_Fov);
    INLINE void setProjectionType(Enum p_ProjectionType);
    INLINE void setFarClipDistance(float p_Far);
    INLINE void setNearClipDistance(float p_Near);

	virtual void update(const GameTime& p_GameTime) OVERRIDE {};

  private:
    Math::Matrix4 m_ProjectionMatrix;
    Math::Matrix4 m_ViewMatrix;

    expose float m_Fov;
    expose float m_FarClipDistance;
    expose float m_NearClipDistance;
    expose Enum m_ProjectionType;
    bool m_Dirty;
    bool m_Active;
  }; //end of Camera class declaration
#include "K15_CameraComponent.inl"
}} // end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_Camera_h_