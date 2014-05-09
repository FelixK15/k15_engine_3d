/**
 * @file K15_GpuProgramParameter.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/12
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

#ifndef _K15Engine_Rendering_GpuProgramParameter_h_
#define _K15Engine_Rendering_GpuProgramParameter_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_Object.h"
#	include "K15_HashedString.h"
#endif //K15_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API GpuProgramParameter
	{
	public:
		/*********************************************************************************/
		enum eVariableType
		{
		  VT_UNKNOW = 0,
		  VT_INT,
		  VT_BOOL,
		  VT_FLOAT,

		  VT_MATRIX2,
		  VT_MATRIX3,
		  VT_MATRIX4,

		  VT_VECTOR2,
		  VT_VECTOR3,
		  VT_VECTOR4,

		  VT_SAMPLER_1D,
		  VT_SAMPLER_2D,
		  VT_SAMPLER_3D,

		  VT_COUNT
		}; //VariableType
		/*********************************************************************************/
		enum eUpdateFrequency
		{
			UF_PER_MATERIAL		= 0x0001,
			UF_PER_OBJECT		= 0x0002,
			UF_PER_FRAME		= 0x0004,
			UF_PER_CAMERA		= 0x0008,

			UF_ALL			= UF_PER_CAMERA | UF_PER_OBJECT | UF_PER_FRAME | UF_PER_CAMERA,

			UF_COUNT = 4
		}; //UpdateFrequency
		/*********************************************************************************/
		enum eParameterIdentifier
		{
			PI_VIEW_MATRIX = 0,
			PI_PROJECTION_MATRIX,
			PI_MODEL_MATRIX,
			PI_VIEW_PROJECTION_MATRIX,
			PI_TIME,

			PI_TEXTURE_1,
			PI_TEXTURE_2,
			PI_TEXTURE_3,
			PI_TEXTURE_4,
			PI_TEXTURE_5,
			PI_TEXTURE_6,
			PI_TEXTURE_7,
			PI_TEXTURE_8,

			PI_COUNT,
			PI_USER = PI_COUNT + 128
		}; //ParameterName
		/*********************************************************************************/

		/*********************************************************************************/
		static const String UniformIdentifierName[PI_COUNT];
		static const Enum UniformUpdateFrequency[PI_COUNT];
		/*********************************************************************************/

	public:
		GpuProgramParameter();
		~GpuProgramParameter();

		INLINE bool isAutoParameter() const;
		INLINE Enum getIdentifier() const;
		INLINE Enum getType() const;
		INLINE Enum getUpdateFrequency() const;
		INLINE uint32 getSize() const;
		INLINE const String& getName() const;
		INLINE int32 getBufferIndex() const;
		INLINE uint32 getOffset() const;
		INLINE uint32 getRegisterIndex() const;
		INLINE void* getData() const;
		INLINE GpuProgram* getGpuProgram() const;
		
		INLINE void setIdentifier(Enum p_Identifier);
		INLINE void setAutoParameter(bool p_Value);
		INLINE void setSize(uint32 p_Size);
		INLINE void setName(const String& p_Name);
		INLINE void setType(Enum p_VariableType);
		INLINE void setUpdateFrequency(Enum p_UpdateFrequency);
		INLINE void setBufferIndex(int32 p_Block);
		INLINE void setOffset(uint32 p_Offset);
		INLINE void setRegisterIndex(uint32 p_Location);
		INLINE void setData(void* p_UserData);
		INLINE void setGpuProgram(GpuProgram* p_Program);
    
	private:
		GpuProgram* m_Program;
		String m_Name;
		Enum m_Type;
		Enum m_UpdateFrequency;
		Enum m_Identifier;
		int32 m_BufferIndex;
		uint32 m_RegisterIndex;
		uint32 m_Size;
		uint32 m_Offset;
		void* m_Data;
		bool m_AutoParameter;
	};// end of GpuProgramParameter class declaration
#include "K15_GpuProgramParameter.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Rendering_GpuProgramParameter_h_