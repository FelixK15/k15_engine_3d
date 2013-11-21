/**
 * @file K15_ResourceHandle.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/15
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
template<class ResourceType>
ResourceHandle<ResourceType>::ResourceHandle()
	: m_ResourceID(K15_INVALID_RESOURCE_ID)
{

}
/*********************************************************************************/
template<class ResourceType>
ResourceHandle<ResourceType>::ResourceHandle(ResourceID p_ResourceID)
	: m_ResourceID(m_ResourceID)
{
	if(m_ResourceID != K15_INVALID_RESOURCE_ID)
	{
		getResource()->increseResourceReference();
	}
}
/*********************************************************************************/
template<class ResourceType>
ResourceHandle<ResourceType>::ResourceHandle( const ResourceHandle<ResourceType>& p_Other )
	: m_ResourceID(p_Other.m_ResourceID)
{
	if(m_ResourceID != K15_INVALID_RESOURCE_ID)
	{
		getResource()->increseResourceReference();
	}
}
/*********************************************************************************/
template<class ResourceType>
ResourceHandle<ResourceType>::~ResourceHandle()
{
	if(m_ResourceID != K15_INVALID_RESOURCE_ID)
	{
		getResource()->decreaseResourceReference();
	}
}
/*********************************************************************************/
template<class ResourceType>
bool ResourceHandle<ResourceType>::isValid() const
{
	return m_ResourceID != K15_INVALID_RESOURCE_ID;
}
/*********************************************************************************/
template<class ResourceType>
const ResourceHandle<ResourceType>& ResourceHandle<ResourceType>::operator=(const ResourceHandle<ResourceType>& p_Other)
{
	if(m_ResourceID != K15_INVALID_RESOURCE_ID)
	{
		getResource()->decreaseResourceReference();
	}

	m_ResourceID = p_Other.getResourceID();

	if(m_ResourceID != K15_INVALID_RESOURCE_ID)
	{
		getResource()->increseResourceReference();
	}

	return *this;
}
/*********************************************************************************/
template<class ResourceType>
ResourceHandle<ResourceType>::operator ResourceType*() const
{
	return getResource();
}
/*********************************************************************************/
template<class ResourceType>
ResourceType* ResourceHandle<ResourceType>::getResource() const
{
	return (ResourceType*)ResourceManager::getInstance()->getResourceByID(m_ResourceID);
}
/*********************************************************************************/