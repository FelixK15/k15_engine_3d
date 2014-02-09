/**
 * @file K15_MatrixUtil.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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

#include "K15_MatrixUtil.h"
#include "K15_RenderWindowBase.h"

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  const Math::Matrix4& MatrixUtil::createPerspectiveProjectionMatrix(float p_Fov, float p_NearClip, float p_FarClip)
  {
    static Math::Matrix4 mat;
    mat = Math::Matrix4::Identity;
    RenderWindowBase* window = g_Application->getRenderWindow();
    float aspect = (float)window->getResolution().width / (float)window->getResolution().height;
    float halfFov = p_Fov * 0.5f;
    float h = p_NearClip * ::tan(p_Fov * Math::DegreeToRadian);
    float w = h * aspect;

    mat._1_1 = w;
    mat._2_2 = h;
    mat._3_3 = (p_FarClip + p_NearClip) / (p_NearClip - p_FarClip);
    mat._3_4 = (2 * p_NearClip * p_FarClip) / (p_NearClip - p_FarClip);
    mat._4_3 = 1;
    mat._4_4 = 0.0f;

    return mat;
  }
  /*********************************************************************************/
  const Math::Matrix4& MatrixUtil::createOrthographicProjectionMatrix(float p_NearClip, float p_FarClip)
  {
    static Math::Matrix4 mat;
    mat = Math::Matrix4::Identity;
    RenderWindowBase* window = g_Application->getRenderWindow();
    const Resolution& res = window->getResolution();
    float tx = 1.0f;
    float ty = 1.0f;
    float tz = (p_FarClip + p_NearClip) / (p_FarClip - p_NearClip);

    mat._1_1 = 2 / (float)res.width;
    mat._2_2 = 2 / (float)res.height;
    mat._3_3 = -2 / p_FarClip - p_NearClip;
    mat._4_1 = tx;
    mat._4_2 = ty;
    mat._4_3 = tz;
  
    return mat;
  }
  /*********************************************************************************/
}} //end of K15_Engine::Core namespace