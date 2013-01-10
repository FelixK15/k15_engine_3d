/**
 * @file K15_ShaderImpl.inl
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

inline const HashMap<const char*,ShaderVariable*> &ShaderImpl::GetShaderVariables()
{
	return m_hmShaderVariables;
}

inline bool ShaderImpl::IsCompiled() const
{
	return m_bCompiled;
}

inline const String &ShaderImpl::GetCompileError() const
{
	return m_sErrorString;
}

inline const String &ShaderImpl::GetShaderCode() const
{
	return m_sShaderCode;
}

inline ShaderImpl::ShaderType ShaderImpl::GetShaderType() const
{
	return m_stType;
}

inline void ShaderImpl::SetShaderType(ShaderImpl::ShaderType stType)
{
	m_stType = stType;
}