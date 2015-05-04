/**
 * @file K15_ResourceArchiveAndroid.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/09
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

#include "K15_PrecompiledHeader.h"

#ifdef K15_OS_ANDROID

#include "K15_RawData.h"
#include "Android/K15_ResourceArchive_Android.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,ResourceArchiveAndroid,ResourceArchiveBase);
	/*********************************************************************************/

	/*********************************************************************************/
	ResourceArchiveAndroid::ResourceArchiveAndroid()
		: ResourceArchiveBase("Android AssetManager"),
		m_AssetManager(0)
	{
		android_app* app = g_Application->getOSLayer().getAndroidApp();
		m_AssetManager = app->activity->assetManager;
	}
	/*********************************************************************************/
	ResourceArchiveAndroid::~ResourceArchiveAndroid()
	{

	}
	/*********************************************************************************/
	bool ResourceArchiveAndroid::getResource(const String& p_ResourceName, RawData* p_ResourceData)
	{
		AAsset* asset = AAssetManager_open(m_AssetManager,p_ResourceName.c_str(),AASSET_MODE_BUFFER);

		if(!asset)
		{
			setError(StringUtil::format("Could not open Resource \"%s\" from Android Assetmanager.",p_ResourceName.c_str()));
			return false;
		}

		p_ResourceData->data = (byte*)AAsset_getBuffer(asset);
		p_ResourceData->size = AAsset_getLength(asset);
		
		return true;
	}
	/*********************************************************************************/
	bool ResourceArchiveAndroid::hasResource(const String& p_ResourceName)
	{
		//we actually have to open the asset to check if it is available or not
		AAsset* asset = AAssetManager_open(m_AssetManager,p_ResourceName.c_str(),AASSET_MODE_STREAMING);
		bool exists = asset != 0;
		AAsset_close(asset);

		return exists;
	}
	/*********************************************************************************/
	bool ResourceArchiveAndroid::_open()
	{
		return true;
	}
	/*********************************************************************************/
	bool ResourceArchiveAndroid::_close()
	{
		return true;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace

#endif //K15_OS_ANDROID
