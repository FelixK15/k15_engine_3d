/**
 * @file K15_Shader.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/23
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

#ifndef __K15_SHADER__
#define __K15_SHADER__

#include "K15_Resource.h"
#include "K15_ShaderImpl.h"

using namespace K15_EngineV2;

namespace K15_Renderer
{
	class ShaderVariable;

	class Shader : public ResourceBase
	{
		DECLARE_RTTI;

	public:
		Shader();
		virtual ~Shader();

		/**
		 * Tries to return a specific ShaderVariable based on its name.
		 *
		 * @param pVariableName - Name of the ShaderVariable object.
		 * 
		 * @return ShaderVariable * - Pointer to a ShaderVariable object or NULL if the variable couldn't be found.
		 */
		inline ShaderVariable *GetShaderVariable(const char *pVariableName);

		/**
		 * Tries to add a new ShaderVariable to this shader.
		 *
		 * @param pVariableName - Name of the variable.
		 * @param pShaderVariable - Pointer to a ShaderVariable object.
		 */
		inline void AddShaderVariable(const char *pVariableName,ShaderVariable *pShaderVariable);

		/**
		 * Try to compile the shader.
		 * @note You need to set the ShaderType via Shader::SetShaderType before you can compile the shader.
		 *
		 * @param pDefines - Preprocessor defines with which the shader will get compiled.
		 * @note The syntax for the preprocessor defines are "-PREPROCESSOR1=VALUE 
		 *													  -PREPROCESSOR2
		 *													  -..."
		 * 
		 * @return bool - true if the shader was compiled without problems and false if an error occured during compilation. (call GetCompilerError to get the error)
		 */
		inline bool Compile(const char *pDefines);

		/**
		 * Overloaded from Resource::Load(). Tries to load the resource from memory.
		 * @note Shader won't get compiled during Load(), you need to manually compile the shader afterwards.
		 */
		virtual inline bool Load(byte *pBuffer,U32 iSize,const String &sExtension);

		/**
		 * Returns a reference to the HashMap that holds all the ShaderVariable objects associated with this shader.
		 * 
		 * @return const HashMap<const char*,ShaderVariable*> & - Reference to the ShaderVariable objects.
		 */
		inline const HashMap<const char*,ShaderVariable*> &GetShaderVariables();

		/**
		 * Check wether or not the shader has been compiled successfully.
		 * 
		 * @return bool - true if shader has been compiled successfully and false if not.
		 */
		inline bool IsCompiled() const;

		/**
		 * Returns compile errors (if any happened)
		 * 
		 * @return const char * - compile error or NULL (if compilation was successful)
		 */
		inline const String &GetCompileError() const;

		/**
		 * Returns the source code of the shader.
		 *
		 * @return const char * - Source code of the shader.
		 */
		inline const String &GetShaderCode() const;

		inline ShaderImpl::ShaderType GetShaderType() const;

		inline void SetShaderType(ShaderImpl::ShaderType stType);

	protected:
		ShaderImpl *m_pShaderImpl;
		
	};
	#include "../src/K15_Shader.inl"
}

#endif //__K15_SHADER__