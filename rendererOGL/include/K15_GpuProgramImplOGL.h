/**
 * @file K15_GpuProgramImplOGL.h
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

#ifndef _K15Engine_RendererOGL_GpuProgramImpl_h_
#define _K15Engine_RendererOGL_GpuProgramImpl_h_

#ifndef K15_RENDERER_OGL_USE_PREDEFINED_HEADERS
#	include "K15_RendererOGLPrerequisites.h"
#endif //K15_RENDERER_OGL_USE_PREDEFINED_HEADERS

#include "K15_GpuProgram.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	class GpuProgramImplOGL : public GpuProgramImplBase
	{
	public:
		/*********************************************************************************/
		static const GLenum GLShaderStageConverter[GpuProgram::PS_COUNT];
		/*********************************************************************************/
	public:
		GpuProgramImplOGL();
		virtual ~GpuProgramImplOGL();

		virtual bool compileShaderCode() OVERRIDE;
		virtual void loadBinaryCode() OVERRIDE;
		virtual void reflect() OVERRIDE;

		virtual void getBinaryCode(RawData* p_Buffer) OVERRIDE;

		virtual String getShaderExtension(Enum p_ProgramStage) OVERRIDE;

		GLuint getProgramGL() const;

	private:
		Enum _getParameterType(GLenum p_GLType);

	private:
		GLuint m_Program;
		GLuint m_Shader;
	};// end of GpuProgramImplOGL class declaration
}}}// end of K15_Engine::Rendering::OGL namespace

#endif //_K15Engine_RendererOGL_GpuProgramImpl_h_