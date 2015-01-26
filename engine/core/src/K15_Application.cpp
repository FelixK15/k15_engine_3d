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
#include "K15_RenderTask.h"
#include "K15_ResourceManager.h"
#include "K15_GameStateManager.h"
#include "K15_DebugRenderer.h"
#include "K15_MeshManager.h"
#include "K15_IniFileParser.h"
#include "K15_Texture.h"
#include "K15_Functor.h"
#include "K15_PhysicsTask.h"
#include "K15_ThreadWorker.h"
#include "K15_EventTask.h"
#include "K15_MemoryProfilingTask.h"
#include "K15_ApplicationModule.h"
#include "K15_ApplicationModuleDescription.h"

#include "K15_Mouse.h"
#include "K15_Keyboard.h"

#include "K15_IOUtil.h"

#ifdef K15_DEBUG
    #ifdef K15_OS_WINDOWS
        #include "win32/K15_VisualStudioLog_Win32.h"
        #include "win32/K15_TextConsoleLog_Win32.h"
    #elif defined K15_OS_ANDROID
        #include "android/K15_Logcat_Android.h"
    #elif defined K15_OS_LINUX
        #include "linux/K15_TextConsoleLog_Linux.h"
    #endif //K15_OS_WINDOWS
#endif //K15_DEBUG

#include "K15_RendererBase.h"
#include "K15_RenderProcessBase.h"
#include "K15_GameStateBase.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	const String Application::SettingsFileName = "settings.ini";
	const String Application::PluginFileName = "plugins.ini";
	const String Application::InputFileName = "input.ini";
	const String Application::GameDirFileName = "gamedir.ini";
	/*********************************************************************************/
	Application::Application()
		: m_Commands(),
		m_DynamicLibraryManager(0),
		m_EventManager(0),
		m_ProfileManager(0),
		m_TaskManager(0),
		m_LogManager(0),
		m_AvgFramesPerSecond(0),
		m_MemoryProfilingTask(0),
		m_ResourceManager(0),
		m_Plugins(),
		m_RunningTime(0.0),
		m_ApplicationParameter(),
		m_ThreadWorker(0),
		m_GameRootDir(),
		m_GameTime(0.0,1.0),
		m_FrameCounter(0),
		m_MaxFPS(60),
		m_AvgFrameTime(0.033f),
		m_FrameAllocator(0),
		m_GameStateManager(0),
		m_DebugRenderer(0),
		m_HomeDir()
	{
		memset(m_FrameStatistics,0,sizeof(FrameStatistic) * FrameStatisticCount);

		m_FrameAllocator = K15_NEW_T(Allocators[AC_CORE],StackAllocator) StackAllocator(1*MEGABYTE,"FrameAllocator",Allocators[AC_CORE]);

		//creating the Log Manager is first thing we'll do.
		m_LogManager = K15_NEW LogManager();

#if defined (K15_DEBUG)
    #if defined K15_OS_WINDOWS
		m_LogManager->addLog(K15_NEW TextConsoleLog_Win32(),true,LogManager::LP_ALL);
		m_LogManager->addLog(K15_NEW VisualStudioLog(),false,LogManager::LP_ALL);
    #elif defined(K15_OS_ANDROID)
		m_LogManager->addLog(K15_NEW Logcat_Android(),false,LogManager::LP_ALL);
    #elif defined (K15_OS_LINUX)
        m_LogManager->addLog(K15_NEW TextConsoleLog_Linux(), true, LogManager::LP_ALL);
    #endif //K15_OS_WINDOWS
#endif //K15_DEBUG
	}
	/*********************************************************************************/
	Application::~Application()
	{
		m_Commands.clear();
	}
	/*********************************************************************************/
	RendererBase* Application::getRenderer() const
	{
		RendererBase* renderer = 0;

		if(m_RenderTask)
		{
			renderer = m_RenderTask->getRenderer();
		}

		return renderer;
	}
	/*********************************************************************************/
	void Application::setRenderer(RendererBase* p_Renderer)
	{
		K15_ASSERT(p_Renderer,"Passed NULL renderer.");
		K15_ASSERT(m_RenderTask,"Renderer passed before Application::initialize.");

		m_RenderTask->setRenderer(p_Renderer);
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
            m_HomeDir = appPath.substr(0, pos+1);
		}
        else if((pos = appPath.find_last_of('/')) != String::npos)
        {
            m_HomeDir = appPath.substr(0, pos+1);
        }

		for(int i = 1;i < p_CommandCount;++i)
		{
			m_Commands.insert(String(p_Commands[i]));
		}
	}
	/*********************************************************************************/
	void Application::createApplicationParameterList()
	{
		for(StringSet::const_iterator iter = m_Commands.begin();iter != m_Commands.end();++iter)
		{
			const String& command = StringUtil::removeWhitespaces(*iter);
			ApplicationParameter param;
			param.Group = "";
			param.Name = command.substr(0,command.find_first_of('='));
			param.Value = command.substr(command.find_first_of('=') + 1);
			m_ApplicationParameter.push_back(param);
		}
	}
	/*********************************************************************************/
	void Application::loadSettingsFile()
	{
		static String settingsFileName;
		settingsFileName = m_GameRootDir + SettingsFileName;
		K15_LOG_NORMAL("Trying to open settings file %s",SettingsFileName.c_str());

		IniFileParser settingsFile(settingsFileName);
		
		if(settingsFile.isValid())
		{
			K15_LOG_SUCCESS("Successfully opened settings file!");
			IniFileGroup group;
			if(settingsFile.getGroupEntries("",&group))
			{
				for(IniFileGroup::IniEntryList::iterator iter = group.entries.begin();iter != group.entries.end();++iter)
				{
					ApplicationParameter param;
					param.Name = iter->key;
					param.Value = iter->value;
					param.Group = group.name;

					m_ApplicationParameter.push_back(param);
				}
			}	
		}
		else
		{
			K15_LOG_ERROR("Could not open settings file. Error:%s",settingsFile.getError().c_str());
		}
	}
	/*********************************************************************************/
#if defined K15_OS_ANDROID
	void Application::initialize(android_app* p_App)
	{
		OSLayer::setAndroidApp(p_App);
		initialize();
	}
#endif //K15_OS_ANDROID
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
		K15_LOG_NORMAL("Initializing OS layer (OS:\"%s\")",OSLayer::OSName.c_str());

		//try to initialize the os layer
		if(!OSLayer::initialize())
		{
			K15_LOG_ERROR("Failed to initialize OS layer");
			return;
		}

		K15_LOG_SUCCESS("OS layer initialized!");

		loadGameDirFile();
		loadSettingsFile();
		
		K15_LOG_NORMAL("Initializing DynamicLibraryManager...");
		m_DynamicLibraryManager = K15_NEW DynamicLibraryManager();
		
		K15_LOG_NORMAL("Initializing EventManager...");
		m_EventManager = K15_NEW EventManager();
		
		K15_LOG_NORMAL("Initializing ProfilingManager...");
		m_ProfileManager = K15_NEW ProfilingManager();;

		K15_LOG_NORMAL("Initializing TaskManager...");
		m_TaskManager = K15_NEW TaskManager();

		K15_LOG_NORMAL("Initializing InputManager...");
		m_InputManager = K15_NEW InputManager();

		K15_LOG_NORMAL("Initializing ThreadWorker...");
		m_ThreadWorker = K15_NEW ThreadWorker();

		K15_LOG_NORMAL("Initializing ResourceManager...");
		m_ResourceManager = K15_NEW ResourceManager();

		K15_LOG_NORMAL("Initializing MeshManager...");
		m_MeshManager = K15_NEW MeshManager();

		K15_LOG_NORMAL("Initializing GameStateManager...");
		m_GameStateManager = K15_NEW GameStateManager();

		K15_LOG_NORMAL("Tasks will get created and added to the task manager.");
		K15_LOG_NORMAL("Creating and adding event task...");
		m_EventTask = K15_NEW EventTask();
		m_TaskManager->addTask(m_EventTask);

		K15_LOG_NORMAL("Creating and adding render task...");
		m_RenderTask = K15_NEW RenderTask();
		m_TaskManager->addTask(m_RenderTask);

		K15_LOG_NORMAL("Creating and adding physics task...");
		m_PhysicsTask = K15_NEW PhysicsTask();
		m_TaskManager->addTask(m_PhysicsTask);

		K15_LOG_NORMAL("Creating and adding memoryprofiler task...");
		m_MemoryProfilingTask = K15_NEW MemoryProfilingTask();
		m_TaskManager->addTask(m_MemoryProfilingTask);

		//load input file
		g_InputManager->parseInputConfig();

		//process settings
		processSettings();

        K15_ASSERT(RenderWindow::initialize(), "Could not initialize RenderWindow!");

		//Load plugins
		loadPluginsFile();

		//call callbacks on plugin modules
		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onInitialize();
		}

		tryInitializeRenderer();
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
			K15_PROFILE_BLOCK("Application::tick",
					tick();
			);
		}

		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onPostRun();
		}
	}
	/*********************************************************************************/
	void Application::onPreTick()
	{
		OSLayer::onPreTick();
		for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
		{
			(*iter)->onPreTick();
		}
	}
	/*********************************************************************************/
	void Application::tick()
	{
		static float newTime = 0.f;
		static float currentTime = getTime();
		static float diffTime = m_AvgFrameTime;
		static float FPSTime = 0.0; //counting to 1 second and then restarts
		static uint32 FPSFrameCounter = 0;

		//clear the frame allocator on the start of each frame
		m_FrameAllocator->clear();

		newTime = getTime();
		diffTime = newTime - currentTime;
		currentTime = newTime;
		//startFrameTime = getTime();

		K15_PROFILE_BLOCK("Application::onPreTick",
			onPreTick();
		);

		//capture input
		g_InputManager->captureInput();

		K15_PROFILE_BLOCK("TaskManager::update",
			m_TaskManager->update(m_GameTime);
		);

		//update current GameState
		K15_PROFILE_BLOCK("GameStateManager::update",
			m_GameStateManager->update(m_GameTime);
		);
	
		K15_PROFILE_BLOCK("Application::onPostTick",
			onPostTick();
		);

		//endFrameTime = getTime();

		//so is there any frame time left?
        if(getTime() >= currentTime)
        {
            //diffTime = endFrameTime - startFrameTime;
        }
        else
        {
            //unlogical..happens sometimes :(
            diffTime = m_AvgFrameTime;
        }


		if(diffTime < m_AvgFrameTime)
		{
			//update running time
			m_RunningTime += m_AvgFrameTime;
			FPSTime += m_AvgFrameTime;

			static float sleepTime = 0.0f;
			sleepTime = m_AvgFrameTime - diffTime;

			K15_PROFILE_BLOCK("Application::sleep",
					OSLayer::sleep(sleepTime);
			);
		}
		else if(diffTime > m_AvgFrameTime)
		{
			//frame took longer than expected. fire a warning
			//and then clip diffTime to the max frame time
			//K15_LOG_WARNING("Frame %i took %.3f seconds to render! (%.3f seconds is average)",m_FrameCounter,diffTime,m_AvgFrameTime);

            //update running time
            m_RunningTime += diffTime;
            FPSTime += diffTime;

            diffTime = m_AvgFrameTime;
        }

        //update game time
        m_GameTime.setDeltaTime(diffTime);
		m_GameTime.setTotalTime(m_GameTime.getTotalTime() + diffTime);

        if(FPSTime >= 1.0)
		{
			m_AvgFramesPerSecond = (uint32)(FPSFrameCounter / FPSTime);
			FPSTime = 0.0;
			FPSFrameCounter = 0;
		}

		//increase frame counter
		++FPSFrameCounter;
		++m_FrameCounter;

	//#if defined K15_DEBUG
		//save statistic for this frame
		static uint32 FrameStatisticIndex = 0;
		FrameStatisticIndex = m_FrameCounter % (FrameStatisticCount - 1);
		m_FrameStatistics[FrameStatisticIndex].Time = diffTime;
		m_FrameStatistics[FrameStatisticIndex].FrameNumber = m_FrameCounter;
		m_FrameStatistics[FrameStatisticIndex].ProfileNode = g_ProfileManager->getRootNode();
        RenderWindow::setWindowTitle(StringUtil::format("msec: %.3f - FPS:%u - Frame Index: %i",
                                                        diffTime * 1000,
                                                        m_AvgFramesPerSecond,
                                                        m_FrameCounter));

		if(m_FrameCounter > FrameStatisticCount)
		{
			  g_ProfileManager->eraseProfilingForFrame((m_FrameCounter - 1) - FrameStatisticCount);
		}
	//#endif //K15_DEBUG
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
		K15_LOG_NORMAL("Trying to open plugin file \"%s\".",PluginFileName.c_str());

		static String pluginsFileName;
		pluginsFileName = m_GameRootDir + PluginFileName;

		if(!IOUtil::fileExists(pluginsFileName))
		{
			pluginsFileName = m_HomeDir + PluginFileName;
		}

		StringSet pluginsToLoad;
		IniFileParser pluginFile(pluginsFileName);

		if(pluginFile.isValid())
		{
			K15_LOG_SUCCESS("Opened plugin file \"%s\".",PluginFileName.c_str());

			IniFileGroup group;
			if(pluginFile.getGroupEntries("",&group))
			{
				String pluginPath;

				for(IniFileGroup::IniEntryList::iterator iter = group.entries.begin();iter != group.entries.end();++iter)
				{
					pluginPath.clear();
					pluginPath = iter->value + "." + OSLayer::PluginExtension;
					pluginsToLoad.insert(pluginPath);
				}

				initializePlugins(pluginsToLoad);
			}
		}
		else
		{
			K15_LOG_ERROR("Could not open plugin file \"%s\". Error:%s",pluginsFileName.c_str(),pluginFile.getError().c_str());
		}
	}
	/*********************************************************************************/
	void Application::initializePlugins(const StringSet& p_PluginNames)
	{
		static uint32 PluginInfoBufferSize = 512;
		DynamicLibraryBase* lib = 0;
		char* messageBuffer = (char*)alloca(PluginInfoBufferSize);
		for(StringSet::const_iterator iter = p_PluginNames.begin();iter != p_PluginNames.end();++iter)
		{
			K15_LOG_NORMAL("Trying to initialize plugin \"%s\"",iter->c_str());
			
			if((lib = m_DynamicLibraryManager->load(*iter)) != 0)
			{
				Functor0<void> initFunc = lib->getSymbol<void>("pluginLoad");
				Functor0<ApplicationModuleDescription> moduleDescFunc = lib->getSymbol<ApplicationModuleDescription>("getDescription");
				Functor0<void> shutdownFunc = lib->getSymbol<void>("pluginUnload");

				if(!moduleDescFunc.isValid() || !initFunc.isValid())
				{
					K15_LOG_ERROR("Failed to initialize plugin \"%s\"",(*iter).c_str());
					
					if(!moduleDescFunc.isValid())
					{
						K15_LOG_ERROR("Plugin \"%s\" has no \"getDescription\" function.",(*iter).c_str());
					}

					if(!initFunc.isValid())
					{
						K15_LOG_ERROR("Plugin \"%s\" has no \"pluginLoad\" function.",(*iter).c_str());
					}

					break;
				}

				//initialize plugin
				initFunc();

				//get the plugins module description to check what kind of module we're loading
				ApplicationModuleDescription description = moduleDescFunc();
				
				K15_LOG_SUCCESS("Plugin information:\n\tAuthor:\t%s\n\tVersion:\t%u.%u\n\tEngine Version:\t%u",description.Author.c_str(),description.MajorVersion,description.MinorVersion,description.CompiledWithEngineVersion);

				static ApplicationParameterList pluginSettings;

				//Filter plugin parameter by group name
				for(StringSet::iterator groupIter = description.GroupFilter.begin();groupIter != description.GroupFilter.end();++groupIter)
				{
					K15_LOG_NORMAL("Filtering plugin parameter (by group name \"%s\")...",(*groupIter).c_str());
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
					K15_LOG_NORMAL("...Done filtering plugin parameter. (%i parameter found)",counter);
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

		g_InputManager->shutdown();

		if(m_DebugRenderer)
		{
			K15_LOG_NORMAL("Destroying DebugRenderer...");
			K15_DELETE m_DebugRenderer;
		}

		if(getRenderer())
		{
			getRenderer()->shutdown();
		}

		RenderWindow::shutdown();
		OSLayer::shutdown();
		m_ProfileManager->clear();

		K15_LOG_NORMAL("Destroying GameStateManager...");
		K15_DELETE m_GameStateManager;

		K15_LOG_NORMAL("Destroying MeshManager...");
		K15_DELETE m_MeshManager;

		K15_LOG_NORMAL("Destroying ResourceManager...");
		K15_DELETE m_ResourceManager;

		K15_LOG_NORMAL("Destroying ThreadWorker...");
		K15_DELETE m_ThreadWorker;

		K15_LOG_NORMAL("Destroying InputManager...");
		K15_DELETE m_InputManager;

		K15_LOG_NORMAL("Destroying TaskMananger...");
		K15_DELETE m_TaskManager;

		m_RenderTask = 0;

		K15_LOG_NORMAL("Destroying ProfilingManager...");
		K15_DELETE m_ProfileManager;

		K15_LOG_NORMAL("Destroying EventManager...");
		K15_DELETE m_EventManager;

		K15_LOG_NORMAL("Destroying DynamicLibraryManager...");
		K15_DELETE m_DynamicLibraryManager;

		K15_LOG_NORMAL("Destroying LogManager...");
		K15_DELETE m_LogManager;

		#ifdef K15_USE_MEMORY_MANAGEMENT
			m_FrameAllocator->~StackAllocator();
			K15_DELETE_SIZE(Allocators[AC_CORE],m_FrameAllocator,sizeof(StackAllocator));
		#endif

		K15_DELETE m_FrameAllocator;

		K15_DELETE this; //evil

		BaseAllocatedObject::removeAllocators();

		//K15_DELETE g_MemoryProfiler;
	}
	/*********************************************************************************/
	void Application::loadGameDirFile()
	{
		String gameDirFileName = m_HomeDir + GameDirFileName;
		if(IOUtil::fileExists(gameDirFileName))
		{
			String gameDir = IOUtil::readWholeFile(gameDirFileName);
			m_GameRootDir = m_HomeDir + gameDir;
			m_GameRootDir = IOUtil::convertToUnixFilePath(m_GameRootDir);
			
		}
		else
		{
			K15_LOG_WARNING("Could not find gamedir file \"%s\"",GameDirFileName.c_str());
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

				RenderWindow::setResolution(width, height);
			}

			if(currentParam.Name == "Renderer")
			{
				String rendererLibrary = StringUtil::format("%s.%s", currentParam.Value.c_str(), OSLayer::PluginExtension.c_str());
				DynamicLibraryBase* rendererLib = g_DynamicLibraryManager->load(rendererLibrary);

				if(rendererLib)
				{
					Functor0<void> pluginLoad = rendererLib->getSymbol<void>("pluginLoad");

					if(pluginLoad.isValid())
					{
						pluginLoad();
					}
				}
			}

			if(currentParam.Name == "Fullscreen")
			{
				bool value = StringUtil::toBool(currentParam.Value);

				RenderWindow::setIsFullscreen(value);
			}

			if(currentParam.Name == "LockMouse")
			{
				bool value = StringUtil::toBool(currentParam.Value);

				InputDevices::Mouse::lockMouse(value);
			}
		}
	}
	/*********************************************************************************/
	void Application::tryInitializeRenderer()
	{
		static RendererBase* renderer = 0;
		//try to initialize renderer
		if((renderer = getRenderer()) != 0)
		{
			if(renderer->initialize())
			{
				for(ApplicationModuleList::iterator iter = m_LoadedModules.begin();iter != m_LoadedModules.end();++iter)
				{
					(*iter)->onRendererInitialized(renderer);
				}

				m_DebugRenderer = K15_NEW DebugRenderer();
			}
		}
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace
