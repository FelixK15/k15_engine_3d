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

/*********************************************************************************/
double ResourceBase::getCreated() const
{
	return m_Created;
}
/*********************************************************************************/
double ResourceBase::getLastUsed() const
{
	return m_LastUsed;
}
/*********************************************************************************/
uint32 ResourceBase::getResourceReferences() const
{
	return m_ReferenceCount;
}
/*********************************************************************************/
void ResourceBase::increseResourceReference()
{
	++m_ReferenceCount;
}
/*********************************************************************************/
void ResourceBase::decreaseResourceReference()
{
	--m_ReferenceCount;
}
/*********************************************************************************/
const TypeName& ResourceBase::getResourceType() const
{
	return TypeName::BLANK;
}
/*********************************************************************************/
bool ResourceBase::isMarkedAsUnreferenced() const
{
	return m_MarkAsUnreferenced;
}
/*********************************************************************************/
void ResourceBase::setMarkedAsUnreferenced(bool p_MarkAsUnreferenced) 
{
	m_MarkAsUnreferenced = p_MarkAsUnreferenced;
}
/*********************************************************************************/
ResourceManager* ResourceBase::getResourceManager() const
{
	return m_ResourceManager;
}
/*********************************************************************************/
ResourceFileBase* ResourceBase::getResourceFile() const
{
	return m_ResourceFile;
}
/*********************************************************************************/
void ResourceBase::setResourceManager(ResourceManager* p_ResourceManager)
{
	m_ResourceManager = p_ResourceManager;
}
/*********************************************************************************/
void ResourceBase::setResourceFile(ResourceFileBase* p_ResourceFile)
{
	m_ResourceFile = p_ResourceFile;
}
/*********************************************************************************/
byte* ResourceBase::getRawData()
{
	return m_RawData;
}
/*********************************************************************************/
uint32 ResourceBase::getRawDataSize()
{
	return m_RawDataSize;
}
/*********************************************************************************/
Enum ResourceBase::getPriority() const
{
	return (Enum)m_Priority;
}
/*********************************************************************************/
void ResourceBase::setPriority(Enum p_Priority)
{
	m_Priority = (eResourcePriority)p_Priority;
}
/*********************************************************************************/