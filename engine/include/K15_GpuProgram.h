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

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_ResourceBase.h"
#include "K15_GpuProgramParameter.h"

namespace K15_Engine { namespace Rendering { 
	class K15_CORE_API GpuProgramImplBase : public RenderingAllocatedObject
	{
	public:
		virtual bool compileShaderCode() = 0;
		virtual void loadBinaryCode() = 0;
		virtual void getBinaryCode(RawData* p_Buffer) = 0;
		virtual void reflect() = 0;

		virtual String getShaderExtension(Enum p_ProgramStage) = 0;

		INLINE void setGpuProgram(GpuProgram* p_GpuProgram);
		INLINE GpuProgram* getGpuProgram() const;

	protected:
		GpuProgram* m_GpuProgram;
	};// end of GpuProgramImplBase class declaration
	/*********************************************************************************/
	class K15_CORE_API GpuProgram : public ResourceBase, public RenderingAllocatedObject
	{
	public:
		/*********************************************************************************/
		static const uint32 MaxParameter;
		typedef DynamicArray(GpuProgramParameter) ParameterList;
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
		GpuProgram(const String& p_ProgramName, Enum p_ProgramStage);
		virtual ~GpuProgram();

		INLINE bool isCompiled() const;
		INLINE const RawData* getBinaryCode() const;
		INLINE const String& getError() const;
		INLINE const String& getShaderCode() const;
		INLINE uint32 getAmountUniforms() const;
		INLINE uint32 getAmountAttributes() const;
		INLINE Enum getStage() const;

		bool compile();
		bool reflect();
		void setProgramCode(const String& p_Code, bool p_Compile = false);

		INLINE void setError(const String& p_Error);
		INLINE void setCompiled(bool p_Compiled);

		void setAmountUniforms(uint32 p_Amount);
		void setAmountAttributes(uint32 p_Amount);
		
		virtual void loadDebug(RawData& p_Data);
		virtual bool internalLoad(const RawData& p_Data);

		GpuProgramParameter& getUniform(uint32 p_Index);
		GpuProgramParameter& getAttribute(uint32 p_Index);
		
		INLINE const GpuProgramImplBase* getImpl() const;

	private:
		const String& _resolveIncludes(const char* p_ShaderCode);

	protected:
		GpuProgramImplBase* m_Impl;
		ParameterList m_Uniforms;
		ParameterList m_Attributes;
		RawData m_BinaryCode;
		Enum m_Stage;
		uint32 m_UsedUniforms;
		uint32 m_UsedAttributes;
		String m_Error;
		String m_ShaderCode;
		bool m_Compiled;
	};/// end of GpuProgram class declaration
#include "K15_GpuProgram.inl"
}}//end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_GpuProgram_h_