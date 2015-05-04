/**
 * @file K15_GpuProgramParameter.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/18
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

#include "K15_GpuProgramParameter.h"
#include "K15_RendererBase.h"
#include "K15_CameraComponent.h"
/*#include "K15_Matrix4.h"*/
#include "K15_RawData.h"

namespace K15_Engine { namespace Rendering {
  /*********************************************************************************/
	const String GpuProgramParameter::UniformIdentifierName[GpuProgramParameter::PI_COUNT] = {
		"g_ViewMatrix",     //PI_VIEW_MATRIX
		"g_ProjMatrix",     //PI_PROJECTION_MATRIX
		"g_ModelMatrix",    //PI_MODEL_MATRIX
		"g_ViewProjMatrix", //PI_VIEW_PROJECTION_MATRIX
		"g_NormalMatrix",	//PI_NORMAL_MATRIX
		"g_Time",			//PI_TIME
		"g_ViewportSize",	//PI_RESOLUTION

		"g_Texture1",      //PI_TEXTURE_1
		"g_Texture2",      //PI_TEXTURE_2
		"g_Texture3",      //PI_TEXTURE_3
		"g_Texture4",      //PI_TEXTURE_4
		"g_Texture5",      //PI_TEXTURE_5
		"g_Texture6",      //PI_TEXTURE_6
		"g_Texture7",      //PI_TEXTURE_7
		"g_Texture8",      //PI_TEXTURE_8

		"g_Lights[].type",					//PI_LIGHT_TYPE
		"g_Lights[].radius",				//PI_LIGHT_RADIUS
		"g_Lights[].positionES",			//PI_LIGHT_POSITION
		"g_Lights[].direction",				//PI_LIGHT_DIRECTION
		"g_Lights[].diffuse",				//PI_LIGHT_DIFFUSE
		"g_Lights[].specular",				//PI_LIGHT_SPECULAR
		"g_Lights[].coneAngle",				//PI_LIGHT_CONE_ANGLE
		"g_Lights[].constantAttenuation",	//PI_LIGHT_CONSTANT_ATTENUATION
		"g_Lights[].linearAttenuation",		//PI_LIGHT_LINEAR_ATTENUATION
		"g_Lights[].quadricAttenuation",	//PI_LIGHT_QUADRIC_ATTENUATION
		"g_Lights[].specularExponent",		
		//PI_LIGHT_SPOT_EXPONENT

		"g_LightCount",		//PI_LIGHT_COUNT
		"g_AmbientColor"	//PI_AMBIENT_COLOR
	}; //UniformIdentifierName
	/*********************************************************************************/
	const Enum GpuProgramParameter::UniformUpdateFrequency[PI_COUNT] = {
		UF_PER_CAMERA | UF_PER_FRAME,	//PI_VIEW_MATRIX
		UF_PER_CAMERA,					//PI_PROJECTION_MATRIX
		UF_PER_OBJECT,					//PI_MODEL_MATRIX
		UF_PER_CAMERA | UF_PER_FRAME,	//PI_VIEW_PROJECTION_MATRIX
		UF_PER_OBJECT,					//PI_NORMAL_MATRIX
		UF_PER_FRAME,					//PI_TIME
		UF_PER_RENDERSETTINGS,

		UF_PER_MATERIAL,	//PI_TEXTURE_1,
		UF_PER_MATERIAL,	//PI_TEXTURE_2,
		UF_PER_MATERIAL,	//PI_TEXTURE_3,
		UF_PER_MATERIAL,	//PI_TEXTURE_4,
		UF_PER_MATERIAL,	//PI_TEXTURE_5,
		UF_PER_MATERIAL,	//PI_TEXTURE_6,
		UF_PER_MATERIAL,	//PI_TEXTURE_7,
		UF_PER_MATERIAL,	//PI_TEXTURE_8,

		UF_PER_OBJECT,	//PI_LIGHT_TYPE
		UF_PER_OBJECT,	//PI_LIGHT_RADIUS
		UF_PER_OBJECT,	//PI_LIGHT_POSITION
		UF_PER_OBJECT,	//PI_LIGHT_DIRECTION
		UF_PER_OBJECT,	//PI_LIGHT_DIFFUSE
		UF_PER_OBJECT,	//PI_LIGHT_SPECULAR
		UF_PER_OBJECT,	//PI_LIGHT_CONE_ANGLE
		UF_PER_OBJECT,	//PI_LIGHT_CONSTANT_ATTENUATION
		UF_PER_OBJECT,	//PI_LIGHT_LINEAR_ATTENUATION
		UF_PER_OBJECT,	//PI_LIGHT_QUADRIC_ATTENUATION
		UF_PER_OBJECT,	//PI_LIGHT_SPOT_EXPONENT

		UF_PER_FRAME,	//PI_LIGHT_COUNT
		UF_PER_FRAME	//PI_AMBIENT_COLOR
	};
	/*********************************************************************************/
	GpuProgramParameter::GpuProgramParameter()
		:  m_Offset(0),
		m_Size(0),
		m_Type(0),
		m_Name(),
		m_RegisterIndex(0),
		m_UpdateFrequency(0),
		m_Identifier(0),
		m_BufferIndex(-1),
		m_AutoParameter(false),
		m_Program(0),
		m_Data(0),
		m_ArrayIndex(-1)
	{

	}
	/*********************************************************************************/
	GpuProgramParameter::~GpuProgramParameter()
	{

	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace