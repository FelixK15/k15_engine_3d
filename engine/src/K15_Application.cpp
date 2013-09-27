/**
 * @file K15_Application.cpp
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/12
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

#include "K15_Application.h"
#include "K15_LogManager.h"
#include "K15_DynamicLibraryManager.h"
#include "K15_EventManager.h"
#include "K15_ProfilingManager.h"
#include "K15_TaskManager.h"
#include "K15_DynamicLibraryBase.h"
#include "K15_Functor.h"
#include "K15_ApplicationModule.h"
#include "K15_ApplicationModuleDescription.h"

namespace K15_Engine { namespace System { 
	/*********************************************************************************/
	const String Application::SettingsFileName = "settings.ini";
	const String Application::PluginFileName = "plugins.ini";
	/*********************************************************************************/
	Application::Application()
		: StackAllocator(GIGABYTE),
		m_Commands(),
		m_DynamicLibraryManager(0),
		m_EventManager(0),
		m_RenderWindow(0),
		m_ProfileManager(0),
		m_TaskManager(0),
		m_LogManager(0),
		m_Plugins(),
		m_ApplicationParameter(),
		m_GameRootDir()

	{
		//creating the Log Manager is first thing we'll do.
		m_LogManager = K15_NEW LogManager();
	}
	/*********************************************************************************/
	Application::Application( int p_CommandCount,char** p_Commands )
		: StackAllocator(GIGABYTE),
		m_Commands(),
		m_DynamicLibraryManager(0),
		m_EventManager(0),
		m_RenderWindow(0),
		m_ProfileManager(0),
		m_TaskManager(0),
		m_LogManager(0),
		m_Plugins(),
		m_ApplicationParameter(),
		m_GameRootDir()
	{
		//creating the Log Manager is first thing we'll do.
		m_LogManager = K15_NEW LogManager();

		createCommandList(p_CommandCount,p_Commands);
		createApplicationParameterList();
	}
	/*********************************************************************************/
	Application::~Application()
	{
		m_Commands.clear();

		K15_LogNormalMessage("Destroying DynamicLibraryManager...");
		K15_DELETE_T(m_DynamicLibraryManager);

		K15_LogNormalMessage("Destroying EventManager...");
		K15_DELETE_T(m_EventManager);

		K15_LogNormalMessage("Destroying ProfilingManager...");
		K15_DELETE_T(m_ProfileManager);

		K15_LogNormalMessage("Destroying LogManager...");
		K15_DELETE_T(m_LogManager);
	}
	/*********************************************************************************/
	void Application::createCommandList(int p_CommandCount,char** p_Commands)
	{
		//the 1. entry in the command array is always (on every system?) the path to the application. 
		//We'll set the path to the application as the default game root dir.
		String appPath = p_Commands[0];

		uint32 pos = String::npos;

		if((pos = appPath.find_last_of('//')) != String::npos)
		{
			m_GameRootDir = appPath.substr(0,pos);
		}

		for(int i = 1;i < p_CommandCount;++i){
			m_Commands.insert(String(p_Commands[i]));
		}
	}
	/*********************************************************************************/
	void Application::createApplicationParameterList()
	{
		for(StringSet::const_iterator iter = m_Commands.begin();iter != m_Commands.end();++iter)
		{
			const String& command = *iter;
			addSingleApplicationParameter(command,"Application Parameter");
		}
	}
	/*********************************************************************************/
	void Application::loadSettingsFile()
	{
		String settingsFileName = m_GameRootDir += SettingsFileName;

		FileStream settingsFile(SettingsFileName);
		
		if(settingsFile.is_open())
		{
			static const uint32 lineLength = 256;
			String line; line.reserve(lineLength);
			String group;

			char* buffer = (char*)alloca(lineLength);
			while(!settingsFile.eof())
			{
				settingsFile.getline(buffer,lineLength);
				line = buffer;
				if(line.find_first_of('[') != String::npos)
				{
					group = line.substr(1,line.size() - 2);
				}
				else if(line.find_first_of('=') != String::npos)
				{
					addSingleApplicationParameter(line,group);
				}
				else
				{
					char* warningBuffer = (char*)alloca(K15_ERROR_BUFFER_SIZE);
					sprintf(warningBuffer,"(%s) Line \"%s\" cant get processed.",settingsFileName.c_str(),line.c_str());
					K15_LogWarningMessage(warningBuffer);
					memset(warningBuffer,0,K15_ERROR_BUFFER_SIZE);
				}

				memset(buffer,0,lineLength);
			}
			
		}

		settingsFile.close();
	}
	/*********************************************************************************/
	void Application::addSingleApplicationParameter(const String& p_Parameter,const String& p_Group)
	{
		int32 pos = String::npos;

		if((pos = p_Parameter.find_first_of('=')) != String::npos)
		{
			const String& key = p_Parameter.substr(0,pos-1);
			const String& value = p_Parameter.substr(pos+1);

			ApplicationParameter param;
			param.Name = key;
			param.Value = value;
			param.Group = p_Group;

			m_ApplicationParameter.push_back(param);
		}
	}
	/*********************************************************************************/
	void Application::initialize()
	{
		loadSettingsFile();

		K15_LogNormalMessage("Initializing DynamicLibraryManager...");
		m_DynamicLibraryManager = K15_NEW DynamicLibraryManager();

		K15_LogNormalMessage("Initializing EventManager...");
		m_EventManager = K15_NEW EventManager();

		K15_LogNormalMessage("Initializing ProfilingManager...");
		m_ProfileManager = K15_NEW ProfilingManager();

		K15_LogNormalMessage("Initializing TaskManager...");
		m_TaskManager = K15_NEW TaskManager();

		loadPluginsFile();
	}
	/*********************************************************************************/
	void Application::loadPluginsFile()
	{
		String pluginsFileName = m_GameRootDir += PluginFileName;
		StringSet pluginsToLoad;

		FileStream pluginsFile(pluginsFileName);

		if(pluginsFile.is_open())
		{
			static const uint32 lineLength = 256;
			char* buffer = (char*)alloca(lineLength);
			String line; line.reserve(lineLength);
			String pluginName; uint32 pos = 0;
			while(!pluginsFile.eof())
			{
				pluginsFile.getline(buffer,lineLength);
				line = buffer;

				if((pos = line.find_first_of('=')) != String::npos)
				{
					pluginName = line.substr(pos+1);
					pluginsToLoad.insert(pluginName);
				}
				else
				{
					char* warningBuffer = (char*)alloca(K15_ERROR_BUFFER_SIZE);
					sprintf(warningBuffer,"(%s) Line \"%s\" cant get processed.",pluginsFileName.c_str(),line.c_str());
					K15_LogWarningMessage(warningBuffer);
				}
			}
		}

		pluginsFile.close();

		initializePlugins(pluginsToLoad);
	}
	/*********************************************************************************/
	void Application::initializePlugins(const StringSet& p_PluginNames)
	{
		DynamicLibraryBase* lib = 0;
		char* messageBuffer = (char*)alloca(K15_PLUGIN_INFO_BUFFER_SIZE);
		for(StringSet::const_iterator iter = p_PluginNames.begin();iter != p_PluginNames.end();++iter)
		{
			K15_LogNormalMessage("Trying to initialize plugin " + *iter);
			
			if((lib = m_DynamicLibraryManager->load(*iter)) != 0)
			{
				Functor0<void> initFunc = lib->getSymbol<void>("loadPlugin");
				Functor0<ApplicationModuleDescription> moduleDescFunc = lib->getSymbol<ApplicationModuleDescription>("getDescription");
				Functor0<ApplicationModule*> moduleFunc = lib->getSymbol<ApplicationModule*>("getModule");
				Functor0<void> shutdownFunc = lib->getSymbol<void>("closePlugin");

				if(!moduleDescFunc.isValid() || !moduleFunc.isValid())
				{
					K15_LogErrorMessage("Failed to initialize plugin " + *iter);
					
					if(!moduleDescFunc.isValid())
					{
						K15_LogErrorMessage("Plugin " + *iter + " has no \"getDescription\" function.");
					}

					if(!moduleFunc.isValid())
					{
						K15_LogErrorMessage("Plugin " + *iter + " has no \"getModule\" function.");
					}

					break;
				}

				//initialize plugin
				if(initFunc.isValid()) initFunc();

				//get the plugins module description to check what kind of module we're loading
				ApplicationModuleDescription description = moduleDescFunc();
				
				sprintf(messageBuffer,"Plugin information:\n\tAuthor:\t%s\n\tVersion:\t\%i.%i",
					description.Author,description.MajorVersion,description.MinorVersion);
				K15_LogNormalMessage(messageBuffer);
				memset(messageBuffer,0,K15_PLUGIN_INFO_BUFFER_SIZE);

				//get the module
				ApplicationModule* module = moduleFunc();
				if(description.SettingsBitMask | ApplicationModuleDescription::PF_OwnTask)
				{
					//m_TaskManager->addTask(module->getTask());
				}

				//uninitialize plugin
				if(shutdownFunc.isValid()) shutdownFunc();
			}
		}
	}
	/*********************************************************************************/
}}