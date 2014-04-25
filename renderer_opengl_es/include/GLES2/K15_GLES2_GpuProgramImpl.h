/**
 * @file K15_GpuProgramImpl.h
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

#ifndef _K15Engine_RendererGLES2_GpuProgramImpl_h_
#define _K15Engine_RendererGLES2_GpuProgramImpl_h_

#ifndef K15_RENDERER_USE_PREDEFINED_HEADERS
#	include "K15_RendererGLESPrerequisites.h"
#endif //K15_RENDERER_OGL_USE_PREDEFINED_HEADERS

#include "K15_GpuProgram.h"

namespace K15_Engine { namespace Rendering { namespace GLES2 {
	class GpuProgramImpl : public GpuProgramImplBase
	{
	public:
		/*********************************************************************************/
		static const GLenum GLShaderStageConverter[GpuProgram::PS_COUNT];
		/*********************************************************************************/
	public:
		GpuProgramImpl();
		virtual ~GpuProgramImpl();

		virtual bool compileShaderCode() OVERRIDE;
		virtual void loadBinaryCode() OVERRIDE;
		virtual void reflect() OVERRIDE;

		virtual void getBinaryCode(RawData* p_Buffer) OVERRIDE;

		virtual String getShaderExtension(Enum p_ProgramStage) OVERRIDE;

		void setShaderGL(GLuint p_Shader);
		void setProgramGL(GLuint p_Program);

		GLuint getShaderGL() const;
		GLuint getProgramGL() const;

	private:
		Enum _getParameterType(GLenum p_GLType);

	private:
		GLuint m_Shader;
		GLuint m_Program;
	};// end of GpuProgramImpl class declaration
}}}// end of K15_Engine::Rendering::GLES2 namespace

#endif //_K15Engine_RendererGLES2_GpuProgramImpl_h_