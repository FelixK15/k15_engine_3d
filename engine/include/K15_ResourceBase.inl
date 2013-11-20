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
inline double ResourceBase::getCreated() const
{
	return m_Created;
}
/*********************************************************************************/
inline double ResourceBase::getLastUsed() const
{
	return m_LastUsed;
}
/*********************************************************************************/
inline uint32 ResourceBase::getResourceReferences() const
{
	return m_ReferenceCount;
}
/*********************************************************************************/
inline void ResourceBase::setAssetName(const ResourceName& p_AssetName)
{
	m_AssetName = p_AssetName;
}
/*********************************************************************************/
inline const ResourceName& ResourceBase::getAssetName() const
{
	return m_AssetName;
}
/*********************************************************************************/
inline void ResourceBase::increseResourceReference()
{
	++m_ReferenceCount;
}
/*********************************************************************************/
inline void ResourceBase::decreaseResourceReference()
{
	--m_ReferenceCount;
}
/*********************************************************************************/
inline const TypeName& ResourceBase::getResourceType() const
{
	return TypeName::BLANK;
}
/*********************************************************************************/
inline bool ResourceBase::isMarkedAsUnreferenced() const
{
	return m_MarkAsUnreferenced;
}
/*********************************************************************************/
inline void ResourceBase::setMarkedAsUnreferenced(bool p_MarkAsUnreferenced) 
{
	m_MarkAsUnreferenced = p_MarkAsUnreferenced;
}
/*********************************************************************************/
inline ResourceManager* ResourceBase::getResourceManager() const
{
	return m_ResourceManager;
}
/*********************************************************************************/
inline ResourceFileBase* ResourceBase::getResourceFile() const
{
	return m_ResourceFile;
}
/*********************************************************************************/
inline void ResourceBase::setResourceManager(ResourceManager* p_ResourceManager)
{
	m_ResourceManager = p_ResourceManager;
}
/*********************************************************************************/
inline void ResourceBase::setResourceFile(ResourceFileBase* p_ResourceFile)
{
	m_ResourceFile = p_ResourceFile;
}
/*********************************************************************************/
inline byte* ResourceBase::getRawData()
{
	return m_RawData;
}
/*********************************************************************************/
inline uint32 ResourceBase::getRawDataSize()
{
	return m_RawDataSize;
}
/*********************************************************************************/
inline Enum ResourceBase::getPriority() const
{
	return (Enum)m_Priority;
}
/*********************************************************************************/
inline void ResourceBase::setPriority(Enum p_Priority)
{
	m_Priority = (eResourcePriority)p_Priority;
}
/*********************************************************************************/