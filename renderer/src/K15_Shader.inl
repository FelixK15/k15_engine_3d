/**
 * @file K15_Shader.inl
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
 */

inline ShaderVariable *Shader::GetShaderVariable(const char *pVariableName)
{
	return m_pShaderImpl->GetShaderVariable(pVariableName);
}

inline void Shader::AddShaderVariable(const char *pVariableName,ShaderVariable *pShaderVariable)
{
	return m_pShaderImpl->AddShaderVariable(pVariableName,pShaderVariable);
}

inline bool Shader::Compile(const char *pDefines)
{
	return m_pShaderImpl->Compile(pDefines);
}

inline bool Shader::Load(byte *pBuffer,U32 iSize,const String &sExtension)
{
	return m_pShaderImpl->Load(pBuffer,iSize,sExtension);
}

inline const HashMap<const char*,ShaderVariable*> &Shader::GetShaderVariables()
{
	return m_pShaderImpl->GetShaderVariables();
}

inline bool Shader::IsCompiled() const
{
	return m_pShaderImpl->IsCompiled();
}

inline const String &Shader::GetCompileError() const
{
	return m_pShaderImpl->GetCompileError();
}

inline const String &Shader::GetShaderCode() const
{
	return m_pShaderImpl->GetShaderCode();
}

inline ShaderImpl::ShaderType Shader::GetShaderType() const
{
	return m_pShaderImpl->GetShaderType();
}

inline void Shader::SetShaderType(ShaderImpl::ShaderType stType)
{
	return m_pShaderImpl->SetShaderType(stType);
}