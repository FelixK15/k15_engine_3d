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

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_Object.h"
#	include "K15_HashedString.h"
#endif //K15_USE_PRECOMPILED_HEADER

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
		struct K15_CORE_API Default
		{
			static const ObjectName WorldViewProjectionMatrix;
			static const ObjectName InverseWorldViewProjectionMatrix;
		}; //Default
		/*********************************************************************************/
		typedef HashMap(ObjectName,String) DefaultVariableNameMap;
		static const DefaultVariableNameMap DefaultVariableNames;

	public:
		GpuProgramParameter();
		virtual ~GpuProgramParameter();

		INLINE bool isAutoParameter() const;
		INLINE Enum getType() const;
		INLINE uint32 getSize() const;
		INLINE const String& getName() const;
		INLINE const ObjectName& getAutoName() const;
		INLINE int32 getBufferIndex() const;
		INLINE uint32 getOffset() const;
		INLINE uint32 getRegisterIndex() const;

		INLINE void setAutoParameter(bool p_Value);
		INLINE void setSize(uint32 p_Size);
		INLINE void setName(const String& p_Name);
		INLINE void setAutoName(const ObjectName& p_Name);
		INLINE void setType(Enum p_VariableType);
		INLINE void setBufferIndex(int32 p_Block);
		INLINE void setOffset(uint32 p_Offset);
		INLINE void setRegisterIndex(uint32 p_Location);

	private:
		static DefaultVariableNameMap _createDefaultVariableNameMap();

	private:
		String m_Name;
		ObjectName m_AutoName;
		Enum m_Type;
		int32 m_BufferIndex;
		uint32 m_RegisterIndex;
		uint32 m_Size;
		uint32 m_Offset;
		bool m_AutoParameter;
	};// end of GpuProgramParameter class declaration
#include "K15_GpuProgramParameter.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Rendering_GpuProgramParameter_h_