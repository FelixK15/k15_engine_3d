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
    virtual bool reflect() = 0;

		INLINE void setGpuProgram(GpuProgram* p_GpuProgram);
		INLINE GpuProgram* getGpuProgram() const;

	protected:
		GpuProgram* m_GpuProgram;
	};// end of GpuProgramImplBase class declaration
	/*********************************************************************************/
	class K15_CORE_API GpuProgram : public ResourceBase
	{
	public:
    /*********************************************************************************/
    typedef List(GpuProgramParameter*) ParameterList;
		/*********************************************************************************/
		enum eProgramStage
		{
			PS_VERTEX = 0,
			PS_GEOMETRY,
			PS_FRAGMENT,
			PS_COMPUTE,

			PS_COUNT
		}; //ProgramStage
    /*********************************************************************************/
	public:
		GpuProgram();
		virtual ~GpuProgram();

		INLINE bool isCompiled() const;
		INLINE const RawData* getBinaryCode() const;
		INLINE const String& getError() const;
		INLINE const String& getShaderCode() const;
		INLINE Enum getStage() const;

		bool compile();
		void setProgramCode(const String& p_Code);

		INLINE void setError(const String& p_Error);

		virtual void loadDebug(RawData& p_Data);
		virtual bool internalLoad(const RawData& p_Data);

    void addParameter(const String& p_Name, Enum p_Type, uint32 p_Size);

		INLINE const GpuProgramImplBase* getImpl() const;

  private:
    const String& _resolveIncludes(const char* p_ShaderCode);

	protected:
		GpuProgramImplBase* m_Impl;
    ParameterList m_Parameter;
		RawData m_BinaryCode;
		Enum m_Stage;
		String m_Error;
		String m_ShaderCode;
		bool m_Compiled;
	};/// end of GpuProgram class declaration
#include "K15_GpuProgram.inl"
}}//end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_GpuProgram_h_