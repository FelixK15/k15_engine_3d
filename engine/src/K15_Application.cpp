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

#include "K15_PrecompiledHeader.h"

#include "K15_DynamicLibraryManager.h"
#include "K15_EventManager.h"
#include "K15_ProfilingManager.h"
#include "K15_TaskManager.h"
#include "K15_DynamicLibraryBase.h"
#include "K15_StringUtil.h"
#include "K15_Functor.h"
#include "K15_RenderTask.h"
#include "K15_PhysicsTask.h"
#include "K15_EventTask.h"
#include "K15_ApplicationModule.h"
#include "K15_ApplicationModuleDescription.h"

#ifdef K15_DEBUG
#	include "K15_TextConsoleLog.h"
#endif //K15_DEBUG

#ifdef K15_OS_WINDOWS
#	include "Win32\K15_RenderWindow_Win32.h"
#endif //K15_OS_WINDOWS

#include "K15_RendererBase.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	const String Application::SettingsFileName = "settings.ini";
	const String Application::PluginFileName = "plugins.ini";
	const String Application::GameDirFileName = "gamedir.ini";
	/*********************************************************************************/
	Application::Application()
		: StackAllocator(30 * MEGABYTE,_N(ApplicationAllocator)),
		m_Commands(),
		m_DynamicLibraryManager(0),
		m_EventManager(0),
		m_RenderWindow(0),
		m_ProfileManager(0),
		m_TaskManager(0),
		m_LogManager(0),
		m_Plugins(),
		m_RunningTime(0.0),
		m_ApplicationParameter(),
		m_GameRootDir(),
		m_GameTime(0.0,1.0),
		m_FrameCounter(0),
		m_MaxFPS(30),
		m_AvgFrameTime(0.33),
		m_FrameAllocator(this,FrameAllocatorSize,_N(FrameAllocator))
	{
		//creating the Log Manager is first thing we'll do.
		m_LogManager = K15_NEW LogManager();

#		if defined (K15_DEBUG)
			m_LogManager->addLog(K15_NEW TextConsoleLog(),true,LogManager::LP_ALL);
#		endif //K15_DEBUG

		m_EventTask = K15_NEW EventTask();
		m_RenderTask = K15_NEW RenderTask();
		m_PhysicsTask = K15_NEW PhysicsTask();
	}
	/*********************************************************************************/
	Application::~Application()
	{
		m_Commands.clear();
		
		K15_DELETE m_RenderTask;
		K15_DELETE m_PhysicsTask;
	}
	/*********************************************************************************/
	void Application::createCommandList(int p_CommandCount,char** p_Commands)
	{
		//the 1. entry in the command array is always (on every system?) the path to the application. 
		//We'll set the path to the application as the default game root dir.
		static String appPath = p_Commands[0];
		String::size_type pos = String::npos;

		if((pos = appPath.find_last_of('\\')) != String::npos)
		{
			m_GameRootDir = appPath.substr(0,pos+1);
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
		static String settingsFileName;
		settingsFileName = m_GameRootDir + SettingsFileName;

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
				//if this line is comment, just skip it.
				if(line.find_first_of("\\\\") != String::npos)
				{
					continue;
				}
				else if(line.find_first_of('[') != String::npos)
				{
					group = line.substr(1,line.size() - 2);
				}
				else if(line.find_first_of('=') != String::npos)
				{
					addSingleApplicationParameter(line,group);
				}
				else
				{
					_LogWarning("(%s) Line \"%s\" cant get processed.",settingsFileName.c_str(),line.c_str());
				}

				memset(buffer,0,lineLength);
			}
			
		}

		settingsFile.close();
	}
	/*********************************************************************************/
	void Application::addSingleApplicationParameter(const String& p_Parameter,const String& p_Group)
	{
		static String key;
		static String value;
		int32 pos = String::npos;

		if((pos = p_Parameter.find_first_of('=')) != String::npos)
		{
			key = p_Parameter.substr(0,pos);
			value = p_Parameter.substr(pos+1);

			ApplicationParameter param;
			param.Name = key;
			param.Value = value;
			param.Group = p_Group;

			m_ApplicationParameter.push_back(param);
		}
	}
	/*********************************************************************************/
	void Application::initialize(int p_CommandCount,char** p_Commands)
	{
		createCommandList(p_CommandCount,p_Commands);
		createApplicationParameterList();

		initialize();
	}
	/*********************************************************************************/
	void Application::initialize()
	{
		_LogNormal("Initializing OS layer (OS:\"%s\")",m_OSLayer.OSName.c_str());

		//try to initialize the os layer
		if(!m_OSLayer.initialize())
		{
			_LogError("Failed to initialize OS layer");
		}

		_LogSuccess("OS layer initialized!");

		loadSettingsFile();
		loadGameDirFile();
		
		_LogNormal("Initializing DynamicLibraryManager...");
		m_DynamicLibraryManager = K15_NEW DynamicLibraryManager();
		
		_LogNormal("Initializing EventManager...");
		m_EventManager = K15_NEW EventManager();
		
		_LogNormal("Initializing ProfilingManager...");
		m_ProfileManager = K15_NEW ProfilingManager();

		_LogNormal("Initializing TaskManager...");
		m_TaskManager = K15_NEW TaskManager();

		_LogNormal("Initializing InputManager...");
		m_InputManager = K15_NEW InputManager();

		//Load plugins
		loadPluginsFile();

		//call callbacks on plugin modules
		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onInitialize();
		}

		m_RenderWindow = K15_NEW RenderWindowType();

		m_RenderWindow->initialize();
	
		//process settings
		processSettings();

		_LogNormal("Tasks will get created and added to the task manager.");
		_LogNormal("Adding event task...");
		m_TaskManager->addTask(m_EventTask);

		_LogNormal("Adding render task...");
		m_TaskManager->addTask(m_RenderTask);

		_LogNormal("Adding physics task...");
		m_TaskManager->addTask(m_PhysicsTask);

		m_RenderTask->getRenderer()->setRenderWindow(m_RenderWindow);
		if(!m_RenderTask->getRenderer()->initialize())
		{
			_LogError("Could not initialize renderer.");
		}
	}
	/*********************************************************************************/
	void Application::setWindowTitle(const String& p_WindowTitle) 
	{
		if(m_RenderWindow)
			m_RenderWindow->setWindowTitle(p_WindowTitle);
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
		m_OSLayer.onPreTick();
		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onPreTick();
		}
	}
	/*********************************************************************************/
	void Application::tick()
	{
		static double startFrameTime = 0.0;
		static double endFrameTime = 0.0;
		static double diffTime = m_AvgFrameTime;

		//clear the frame allocator on the start of each frame
		m_FrameAllocator.clear();

		startFrameTime = getTime();

		{
			K15_PROFILE("Application::onPreTick");
			onPreTick();
		}
		
		{
			K15_PROFILE(Application_TaskManager_Tick);
			//update tasks (render frame)
			m_TaskManager->update(m_GameTime);
		}
		
		{
			K15_PROFILE(Application_Post_Tick);
			onPostTick();
		}

		endFrameTime = getTime();

		if(m_InputManager->isActive(_N(Jump)))
		{
			_LogSuccess("JUMP!");
		}

		//so is there any frame time left?
		diffTime = endFrameTime - startFrameTime;

		//update running time
		m_RunningTime += diffTime;

		if(diffTime < m_AvgFrameTime)
		{
			static double sleepTime = 0.0;
			sleepTime = m_AvgFrameTime - diffTime;

			m_OSLayer.sleep(sleepTime);
		}
		else if(diffTime > m_AvgFrameTime)
		{
			//frame took longer than expected. fire a warning
			//and then clip diffTime to the max frame time
			_LogWarning("Frame %i took %.3f seconds to render! (%.3f seconds is average)",m_FrameCounter,diffTime,m_AvgFrameTime);

			diffTime = m_AvgFrameTime;
		}
		
		//update game time
		m_GameTime.setDeltaTime(diffTime);

		//increase frame counter
		if(++m_FrameCounter == UINT_MAX) m_FrameCounter = 0;

		//save stastic for this frame
		static uint32 FrameStatisticIndex = 0;
		FrameStatisticIndex = m_FrameCounter % (FrameStatisticCount - 1);
		m_FrameStatistics[FrameStatisticIndex].Time = diffTime;
		m_FrameStatistics[FrameStatisticIndex].FrameNumber = m_FrameCounter;

		//m_RenderWindow->setWindowTitle(StringUtil::format("msec: %.3f - Frame Index: %i",diffTime * 100,m_FrameCounter));
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
		_LogNormal("Trying to open plugin file \"%s\".",PluginFileName.c_str());

		static String pluginsFileName;
		pluginsFileName = m_GameRootDir + PluginFileName;

		StringSet pluginsToLoad;

		FileStream pluginsFile(pluginsFileName);

		if(pluginsFile.is_open())
		{
			_LogSuccess("Opened plugin file \"%s\".",PluginFileName.c_str());

			static const uint32 lineLength = 256;
			char* buffer = (char*)_malloca(lineLength);
			String line; line.reserve(lineLength);
			String pluginName; uint32 pos = 0;
			while(!pluginsFile.eof())
			{
				pluginsFile.getline(buffer,lineLength);
				line = buffer;

				//if this line is comment, just skip it.
				if(line.find_first_of("\\\\") != String::npos)
				{
					continue;
				}
				else if((pos = line.find_first_of('=')) != String::npos)
				{
					pluginName = line.substr(pos+1) + '.' + m_OSLayer.PluginExtension;
					pluginsToLoad.insert(pluginName);
				}
				else
				{
					_LogWarning("(%s) Line \"%s\" cant get processed.",pluginsFileName.c_str(),line.c_str());
				}
			}
			pluginsFile.close();
			initializePlugins(pluginsToLoad);
		}
		else
		{
			_LogError("Could not open plugin file \"%s\".",PluginFileName.c_str());
		}
	}
	/*********************************************************************************/
	void Application::initializePlugins(const StringSet& p_PluginNames)
	{
		DynamicLibraryBase* lib = 0;
		char* messageBuffer = (char*)_malloca(K15_PLUGIN_INFO_BUFFER_SIZE);
		for(StringSet::const_iterator iter = p_PluginNames.begin();iter != p_PluginNames.end();++iter)
		{
			_LogNormal("Trying to initialize plugin \"%s\"",iter->c_str());
			
			if((lib = m_DynamicLibraryManager->load(*iter)) != 0)
			{
				Functor0<void> initFunc = lib->getSymbol<void>("pluginLoad");
				Functor0<ApplicationModuleDescription> moduleDescFunc = lib->getSymbol<ApplicationModuleDescription>("getDescription");
				Functor0<void> shutdownFunc = lib->getSymbol<void>("pluginUnload");

				if(!moduleDescFunc.isValid() || !initFunc.isValid())
				{
					_LogError("Failed to initialize plugin \"%s\"",(*iter).c_str());
					
					if(!moduleDescFunc.isValid())
					{
						_LogError("Plugin \"%s\" has no \"getDescription\" function.",(*iter).c_str());
					}

					if(!initFunc.isValid())
					{
						_LogError("Plugin \"%s\" has no \"pluginLoad\" function.",(*iter).c_str());
					}

					break;
				}

				//initialize plugin
				initFunc();


				//get the plugins module description to check what kind of module we're loading
				ApplicationModuleDescription description = moduleDescFunc();
				
				_LogSuccess("Plugin information:\n\tAuthor:\t%s\n\tVersion:\t%u.%u\n\tEngine Version:\t%u",description.Author.c_str(),description.MajorVersion,description.MinorVersion,description.CompiledWithEngineVersion);

				static ApplicationParameterList pluginSettings;

				//Filter plugin parameter by group name
				for(StringSet::iterator groupIter = description.GroupFilter.begin();groupIter != description.GroupFilter.end();++groupIter)
				{
					_LogNormal("Filtering plugin parameter (by group name \"%s\")...",(*groupIter).c_str());
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
					_LogNormal("...Done filtering plugin parameter. (%i parameter found)",counter);
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

		_LogNormal("Destroying InputManager...");
		K15_DELETE_T(m_InputManager);

		_LogNormal("Destroying RenderWindow...");
		K15_DELETE_T(m_RenderWindow);

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
	void Application::loadGameDirFile()
	{
		FileStream gameDirFile(GameDirFileName);

		if(gameDirFile.is_open())
		{
			char* buffer = (char*)_malloca(K15_GAMEDIR_BUFFER_SIZE);
			buffer = '\0';
			gameDirFile.getline(buffer,K15_GAMEDIR_BUFFER_SIZE);
			m_GameRootDir = buffer;
			gameDirFile.close();
		}
		else
		{
			_LogWarning("Could not find gamedir file \"%s\"",GameDirFileName.c_str());
		}
	}
	/*********************************************************************************/
	void Application::processSettings()
	{
		ApplicationParameter currentParam;

		for(ApplicationParameterList::iterator iter = m_ApplicationParameter.begin();iter != m_ApplicationParameter.end();++iter)
		{
			currentParam = (*iter);
			K15_SET_NUMERICAL_SETTING(currentParam,MaxFPS,uint16);

			if(currentParam.Name == "Resolution")
			{
				int width = atoi(currentParam.Value.substr(0,currentParam.Value.find_first_of('x')).c_str());
				int height = atoi(currentParam.Value.substr(currentParam.Value.find_first_of('x') + 1).c_str());

				Resolution r;
				r.width = width;
				r.height = height;
				m_RenderWindow->setResolution(r);
			}
		}
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace