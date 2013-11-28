/**
 * @file K15_GpuProgram.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_GpuProgram_h_
#define _K15Engine_Renderer_GpuProgram_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_ResourceBase.h"

namespace K15_Engine { namespace Rendering { 
	class K15_CORE_API GpuProgramImplBase
	{
	public:
		virtual bool compileShaderCode() = 0;
		virtual bool loadBinaryCode() = 0;
		virtual bool getBinaryCode(RawData* p_Buffer) = 0;

		inline void setGpuProgram(GpuProgram* p_GpuProgram);
		inline GpuProgram* getGpuProgram() const;

	protected:
		GpuProgram* m_GpuProgram;
	};// end of GpuProgramImplBase class declaration
	/*********************************************************************************/
	class K15_CORE_API GpuProgram : public ResourceBase
	{
	public:
		/*********************************************************************************/
		enum eProgramStage
		{
			PS_VERTEX = 0,
			PS_GEOMETRY,
			PS_FRAGMENT,
			PS_COMPUTE,

			PS_COUNT
		};
		/*********************************************************************************/
	public:
		GpuProgram();
		virtual ~GpuProgram();

		inline bool isCompiled() const;
		inline const RawData* getBinaryCode() const;
		inline const String& getError() const;
		inline const String& getShaderCode() const;
		inline Enum getStage() const;

		bool compile();
		void setProgramCode(const String& p_Code);

		inline void setError(const String& p_Error);

		virtual void loadDebug(RawData& p_Data);
		virtual bool internalLoad(const RawData& p_Data);

		inline const GpuProgramImplBase* getImpl() const;

	protected:
		GpuProgramImplBase* m_Impl;
		RawData m_BinaryCode;
		Enum m_Stage;
		String m_Error;
		String m_ShaderCode;
		bool m_Compiled;
	};/// end of GpuProgram class declaration
#include "K15_GpuProgram.inl"
}}//end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_GpuProgram_h_