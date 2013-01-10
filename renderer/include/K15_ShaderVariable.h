/**
 * @file K15_ShaderVariable.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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
#pragma once

#ifndef __K15_SHADERVARIABLE__
#define __K15_SHADERVARIABLE__

#include "K15_StdInclude.h"
#include "K15_Buffer.h"

namespace K15_Renderer
{
	class Geometry;

	class ShaderVariable
	{
	public:
		enum UpdateFrequenzy{
			UF_PER_OBJECT,
			UF_PER_FRAME,
			UF_MANUAL
		};
		ShaderVariable();
		ShaderVariable(U32 iRegisterNumber,U32 iFloatPackCount,UpdateFrequenzy ufFrequenzy,float *pData);
		ShaderVariable(const ShaderVariable &svVariable);
		virtual ~ShaderVariable();

		void SetByteSize(U32 iByteSize);
		void SetSingleElementCount(U32 iNumElements);
		void SetPackElementCount(U32 iFloatPackCount);
		void SetRegisterNumber(U32 iRegisterNumber);
		void SetUpdateFrequenzy(UpdateFrequenzy ufFrequenzy);

		void SetData(float *pData);
		void SetData(U32 iFloatPackCount, float *pData);

		const ShaderVariable &operator=(float *pData);

		virtual void Update(Geometry*) = 0;

		BufferPtr GetBuffer() const;
		float *GetData() const;
		U32 GetElementCount() const;
		U32 GetRegisterNumber() const;
		UpdateFrequenzy GetUpdateFrequenzy() const;

	private:
		void _Allocate(U32);

	private:
		UpdateFrequenzy m_ufFrequenzy;	//Update frequenzy.
		U32 m_iFloatPackCount;			//Amount of float packs. float pack = 4 x float.
		U32 m_iRegisterNumber;			//Number of shader register.
		BufferPtr m_pBuffer;				//Pointer to the GPU Buffer holding the shader variables.
		float *m_pData;					//Pointer to shader variable.
	};
	#include "../src/K15_ShaderVariable.inl"
}

#endif //__K15_SHADERVARIABLE__