/**
 * @file K15_ShaderImpl.h
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

#ifndef __K15_SHADERIMPL__
#define __K15_SHADERIMPL__

#include "K15_StdInclude.h"
#include "K15_HashMap.h"
#include "K15_ShaderVariable.h"

using namespace K15_Renderer;

namespace K15_Renderer
{
	class ShaderImpl
	{
	public:
		enum ShaderType
		{
			ST_NONE,			/*!<Default value. If this ShaderType is set, the shader won't get compiled.*/
			ST_VERTEXSHADER,	/*!<Shader will get compiled and used as vertex shader.*/
			ST_FRAGMENTSHADER,	/*!<Shader will get compiled and used as fragment / pixel shader.*/
			ST_GEOMETRYSHADER	/*!<Shader will get compiled and used as geometry shader.*/
		};

	public:
		virtual ~ShaderImpl();

		/**
		 * Tries to return a specific ShaderVariable based on its name.
		 *
		 * @param pVariableName - Name of the ShaderVariable object.
		 * 
		 * @return ShaderVariable * - Pointer to a ShaderVariable object or NULL if the variable couldn't be found.
		 */
		ShaderVariable *GetShaderVariable(const char *pVariableName);

		/**
		 * Tries to add a new ShaderVariable to this shader.
		 *
		 * @param pVariableName - Name of the variable.
		 * @param pShaderVariable - Pointer to a ShaderVariable object.
		 */
		void AddShaderVariable(const char *pVariableName,ShaderVariable *pShaderVariable);

		/**
		 * Try to compile the shader.
		 *
		 * @param pDefines - Preprocessor defines with which the shader will get compiled.
		 * @note The syntax for the preprocessor defines are "-PREPROCESSOR1=VALUE 
		 *													  -PREPROCESSOR2
		 *													  -..."
		 * 
		 * @return bool - true if the shader was compiled without problems and false if an error occurred during compilation. (call GetCompilerError to get the error)
		 */
		virtual bool Compile(const char *pDefines) = 0;

		/**
		 * Overloaded from Resource::Load(). Tries to load the resource from memory.
		 * @note Shader won't get compiled during Load(), you need to manually compile the shader afterwards.
		 */
		virtual bool Load(byte *pBuffer,U32 iSize,const String &sExtension);

		/**
		 * Returns a reference to the HashMap that holds all the ShaderVariable objects associated with this shader.
		 * 
		 * @return const HashMap<const char*,ShaderVariable*> & - Reference to the ShaderVariable objects.
		 */
		inline const HashMap<const char*,ShaderVariable*> &GetShaderVariables();

		/**
		 * Check whether or not the shader has been compiled successfully.
		 * 
		 * @return bool - true if shader has been compiled successfully and false if not.
		 */
		inline bool IsCompiled() const;

		/**
		 * Returns compile errors (if any happened)
		 * 
		 * @return String - compile error or NULL (if compilation was successful)
		 */
		inline const String &GetCompileError() const;

		/**
		 * Returns the source code of the shader.
		 *
		 * @return String - Source code of the shader.
		 */
		inline const String &GetShaderCode() const;

		inline ShaderType GetShaderType() const;

		inline void SetShaderType(ShaderType stType);

	protected:
		ShaderImpl();

	protected:
		bool m_bCompiled;
		ShaderType m_stType;
		String m_sShaderCode;
		String m_sErrorString;

		HashMap<const char*,ShaderVariable*> m_hmShaderVariables;
	};
	#include "..\src\K15_ShaderImpl.inl"
}

#endif