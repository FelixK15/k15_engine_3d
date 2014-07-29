/**
 * @file K15_MatrixUtil.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#include "K15_MatrixUtil.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace K15_Engine { namespace Math {
  /*********************************************************************************/
  Matrix4 MatrixUtil::createPerspectiveMatrix(float p_FOV, float p_AspectRatio, float p_NearPlane, float p_FarPlane)
  {
    Matrix4 matProj;
    glm::mat4 mat = glm::perspective(p_FOV, p_AspectRatio, p_NearPlane, p_FarPlane);

    memcpy(&matProj, &mat, sizeof(Matrix4));

    return matProj;
  }
  /*********************************************************************************/
  Matrix4 MatrixUtil::createOrthographicMatrix(float p_Left, float p_Right, float p_Top, float p_Bottom, float p_NearPlane, float p_FarPlane)
  {
    Matrix4 matOrtho;
    glm::mat4 mat = glm::ortho(p_Left, p_Right, p_Bottom, p_Top, p_NearPlane, p_FarPlane);

    memcpy(&matOrtho, &mat, sizeof(Matrix4));

    return matOrtho;
  }
  /*********************************************************************************/
  Matrix4 MatrixUtil::translate(const Vector3& p_Translate)
  {
    Matrix4 matTrans;

    glm::vec3 trans;

    memcpy(&trans, &p_Translate, sizeof(Vector3));

    glm::mat4 mat = glm::translate(glm::mat4(1.f), trans);

    memcpy(&matTrans, &mat, sizeof(Matrix4));

    return matTrans;
  }
  /*********************************************************************************/
  Matrix4 MatrixUtil::scale(const Vector3& p_Scale)
  {
    Matrix4 matScale;

    glm::vec3 scale;

    memcpy(&scale, &p_Scale, sizeof(Vector3));

    glm::mat4 mat = glm::scale(glm::mat4(1.f), scale);

    memcpy(&matScale, &mat, sizeof(Matrix4));

    return matScale;
  }
  /*********************************************************************************/
  Matrix4 MatrixUtil::rotate(const Vector3& p_Axis, float p_AngleInRadian)
  {
    Matrix4 matRot;

    glm::vec3 axis;

    memcpy(&axis, &p_Axis, sizeof(Vector3));

    glm::mat4 mat = glm::rotate(glm::mat4(1.f), p_AngleInRadian, axis);

    memcpy(&matRot, &mat, sizeof(Matrix4));

    return matRot;
  }
  /*********************************************************************************/
}} //end of K15_Engine::Math namespace