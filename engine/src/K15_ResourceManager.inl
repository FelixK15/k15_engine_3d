/**
 * @file K15_ResourceManager.inl
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

template<class T>
bool ResourceManager::CacheResource(String& sFileName,Resource::ResourcePriority rpPriority)
{
	U32 iPos = sFileName.FindLastOf('.');
	String sExtension = sFileName.SubString(iPos);

	m_pResoureFile->OpenResourceFile();
	if(m_pResoureFile->HasResource(sFileName.C_Str())){
		byte *pBuffer = NULL;
		char *pErrorMessage = NULL;
		U32 iSize = 0;

		if(m_pResoureFile->GetResource(sFileName,pBuffer,iSize,pErrorMessage)){
			//Resource gets created and loaded.
			Resource *pResource = K15_NEW T();
			pResource->SetFilename(sFileName);

			bool bLoaded = pResource->Load(pBuffer,iSize,sExtension);

			K15_DELETE[] pBuffer;
			pBuffer = NULL;

			if(bLoaded){
				//Add Resource to Cache
				pResource->m_rpPriority = rpPriority;
				pResource->SetName(sFileName);

				m_hmResources.Insert(sFileName.C_Str(),pResource);
				m_arrResource.PushBack(pResource);
				return true;
			}else{
				WriteDefaultLog(String("Resource could not get loaded - resource :") + sFileName);
				return false;
			}
		}else{
			//Error in ResourceFile::GetResource()
			if(pErrorMessage){
				WriteDefaultLog(String("Error during resource caching - resource :") + sFileName);
				WriteDefaultLog(pErrorMessage);
			}else{
				WriteDefaultLog(String("Error during resource caching - but no error message was set - resource :") + sFileName);
			}

			K15_DELETE[] pErrorMessage;
			return false;
		}
	}else{
		WriteDefaultLog(String("Could not load resource file - resource :") + sFileName);
		return false;
	}
}

template<class T>
ResourceHandlePtr ResourceManager::GetResource(String &sFileName,Resource::ResourcePriority rpPriority)
{
	HashItem<const char*,Resource*> *pItem = m_hmResources.Get(sFileName.C_Str());

	if(!pItem){
		if(CacheResource<T>(sFileName,rpPriority)){
			pItem = m_hmResources.Get(sFileName.C_Str());
		}else{
			return NULL;
		}
	}

	Resource *pResource = pItem->GetValue();
	ResourceHandlePtr pResourceHandle(K15_NEW ResourceHandle(pResource));

	return pResourceHandle;
}