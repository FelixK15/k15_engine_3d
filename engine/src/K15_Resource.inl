/**
 * @file K15_Resource.inl
 * @author  Felix Klinge <f.klinge15@gmail.com>
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
 */

const inline double Resource::LastUsed() const
{
	return m_dLastUsed;
}

const  U32 Resource::GetResourceReferences() const
{
	return m_iResourceReferences;
}

const Resource::ResourcePriority Resource::GetPriority() const
{
	return m_rpPriority;
}

inline const String &Resource::GetFilename() const
{
	return m_sFilename;
}

inline void Resource::SetFilename(const String& sFileName)
{
	m_sFilename = sFileName;
}

void Resource::_IncreseResourceReference()
{
	++m_iResourceReferences;
}