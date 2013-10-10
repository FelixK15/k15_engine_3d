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
#include "K15_StringUtil.h"
#include "K15_Functor.h"
#include "K15_ApplicationModule.h"
#include "K15_ApplicationModuleDescription.h"

#ifdef K15_OS_WINDOWS
#	include "K15_RenderWindow_Win32.h"
#	include "K15_ApplicationOSLayer_Win32.h"
#endif //K15_OS_WINDOWS

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

		_LogNormal("Destroying DynamicLibraryManager...");
		K15_DELETE_T(m_DynamicLibraryManager);

		_LogNormal("Destroying EventManager...");
		K15_DELETE_T(m_EventManager);

		_LogNormal("Destroying ProfilingManager...");
		K15_DELETE_T(m_ProfileManager);

		_LogNormal("Destroying LogManager...");
		K15_DELETE_T(m_LogManager);
	}
	/*********************************************************************************/
	void Application::createCommandList(int p_CommandCount,char** p_Commands)
	{
		//the 1. entry in the command array is always (on every system?) the path to the application. 
		//We'll set the path to the application as the default game root dir.
		String appPath = p_Commands[0];

		String::size_type pos = String::npos;

		if((pos = appPath.find_last_of('\\')) != String::npos)
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

			char* buffer = (char*)_malloca(lineLength);
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
					_LogWarning(StringUtil::format("(%s) Line \"%s\" cant get processed.",settingsFileName.c_str(),line.c_str()));
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
		_LogNormal(StringUtil::format("Initializing OS layer (OS:\"%s\")",m_OSLayer.OSName));

		//try to initialize the os layer
		if(!m_OSLayer.initialize())
		{
			_LogError("Failed to initialize OS layer");
		}

		_LogSuccess("OS layer initialized!");

		m_Started = m_OSLayer.getTime();

		loadSettingsFile();

		_LogNormal("Initializing DynamicLibraryManager...");
		m_DynamicLibraryManager = K15_NEW DynamicLibraryManager();
		
		_LogNormal("Initializing EventManager...");
		m_EventManager = K15_NEW EventManager();
		
		_LogNormal("Initializing ProfilingManager...");
		m_ProfileManager = K15_NEW ProfilingManager();

		_LogNormal("Initializing TaskManager...");
		m_TaskManager = K15_NEW TaskManager();

		//Load plugins
		loadPluginsFile();

		//call callbacks on plugin modules
		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onInitialize();
		}

		m_RenderWindow = K15_NEW RenderWindowType();

		m_RenderWindow->initialize();
	}
	/*********************************************************************************/
	void Application::run()
	{
		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onPreRun();
		}

		m_Running = true;
		while(m_Running)
		{
			tick();
		}

		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onPostRun();
		}
	}
	/*********************************************************************************/
	void Application::onPreTick()
	{
		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onPreTick();
		}
	}
	/*********************************************************************************/
	void Application::tick()
	{
		onPreTick();

		//Update gametime
		m_GameTime.setDeltaTime(getTime() - m_TimeLastFrame);
		m_TimeLastFrame = getTime();

		//dispatch events
		m_EventManager->update();
		m_TaskManager->update(m_GameTime);

		//Render frame
		onPostTick();
	}
	/*********************************************************************************/
	void Application::onPostTick()
	{
		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onPostTick();
		}
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
			char* buffer = (char*)_malloca(lineLength);
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
					_LogWarning(StringUtil::format("(%s) Line \"%s\" cant get processed.",pluginsFileName.c_str(),line.c_str()));
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
		char* messageBuffer = (char*)_malloca(K15_PLUGIN_INFO_BUFFER_SIZE);
		for(StringSet::const_iterator iter = p_PluginNames.begin();iter != p_PluginNames.end();++iter)
		{
			_LogNormal("Trying to initialize plugin " + *iter);
			
			if((lib = m_DynamicLibraryManager->load(*iter)) != 0)
			{
				Functor0<void> initFunc = lib->getSymbol<void>("loadPlugin");
				Functor0<ApplicationModuleDescription> moduleDescFunc = lib->getSymbol<ApplicationModuleDescription>("getDescription");
				Functor0<ApplicationModule*> moduleFunc = lib->getSymbol<ApplicationModule*>("getModule");
				Functor0<void> shutdownFunc = lib->getSymbol<void>("closePlugin");

				if(!moduleDescFunc.isValid() || !moduleFunc.isValid())
				{
					_LogError("Failed to initialize plugin " + *iter);
					
					if(!moduleDescFunc.isValid())
					{
						_LogError("Plugin " + *iter + " has no \"getDescription\" function.");
					}

					if(!moduleFunc.isValid())
					{
						_LogError("Plugin " + *iter + " has no \"getModule\" function.");
					}

					break;
				}

				//initialize plugin
				if(initFunc.isValid())
				{
					initFunc();
				}

				//get the plugins module description to check what kind of module we're loading
				ApplicationModuleDescription description = moduleDescFunc();
				
				_LogSuccess(StringUtil::format("Plugin information:\n\tAuthor:\t%s\n\tVersion:\t\%i.%i",description.Author,description.MajorVersion,description.MinorVersion));

				static ApplicationParameterList pluginSettings;

				//Filter plugin parameter by group name
				for(StringSet::iterator groupIter = description.GroupFilter.begin();groupIter != description.GroupFilter.end();++groupIter)
				{
					_LogNormal(StringUtil::format("Filtering plugin parameter (by group name \"%s\")...",(*groupIter).c_str()));
					const String& groupName = *groupIter;
					uint32 counter = 0;
					for(ApplicationParameterList::iterator paramIter = m_ApplicationParameter.begin();paramIter != m_ApplicationParameter.end();++paramIter)
					{
						if((*paramIter).Group == groupName)
						{
							pluginSettings.push_back(*paramIter);
							++counter;
						}
					}
					_LogNormal(StringUtil::format("...Done filtering plugin parameter. (%i parameter found)",counter));
				}

				//get the module
				ApplicationModule* module = moduleFunc();
				m_LoadedModules.push_back(module);
				//Check if plugin provides an own Task...if so, add it to the task manager
				if(description.PluginFlagBitMask | ApplicationModuleDescription::PF_ProvidesTask)
				{
					Task* pluginTask = module->createTask();
					if(!pluginTask)
					{
						_LogError(StringUtil::format("Plugin \"%s\" flag PF_ProvidesTask set, but no Task could be obtained via createTask",lib->getFileName()));
					}
					else
					{
						m_TaskManager->addTask(pluginTask);
						_LogSuccess(StringUtil::format("Task successfully loaded from plugin \"%s\".",lib->getFileName()));
					}
					//Check if the plugin provides a renderer.
				}
				else if(description.PluginFlagBitMask | ApplicationModuleDescription::PF_ProvidesRenderer)
				{
					//Add Renderer
				}

				//uninitialize plugin
				if(shutdownFunc.isValid())
				{
					shutdownFunc();	
				}
			}
		}
	}
	/*********************************************************************************/
	void Application::shutdown()
	{
		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onShutdown();
		}

		m_RenderWindow->shutdown();
		m_OSLayer.shutdown();
	}
	/*********************************************************************************/
}}//end of K15_Engine::System namespace