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
#include "K15_IniFileParser.h"
#include "K15_Texture.h"
#include "K15_PhysicsTask.h"
#include "K15_ThreadWorker.h"
#include "K15_EventTask.h"
#include "K15_ApplicationModule.h"
#include "K15_ApplicationModuleDescription.h"

#include "K15_MemoryProfiler.h"

#include "K15_Mouse.h"
#include "K15_Keyboard.h"

#ifdef K15_DEBUG
#	include "K15_TextConsoleLog.h"
# ifdef _MSC_VER
#   include "Win32\K15_VisualStudioLog_Win32.h"
#endif
#endif //K15_DEBUG

#ifdef K15_OS_WINDOWS
#	include "Win32\K15_RenderWindow_Win32.h"
#endif //K15_OS_WINDOWS

#include "K15_RendererBase.h"
#include "K15_RenderProcessBase.h"

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
		m_RenderWindow(0),
		m_ProfileManager(0),
		m_TaskManager(0),
		m_LogManager(0),
		m_AvgFramesPerSecond(0),
		m_Plugins(),
		m_RunningTime(0.0),
		m_ApplicationParameter(),
		m_ThreadWorker(0),
		m_GameRootDir(),
		m_GameTime(0.0,1.0),
		m_FrameCounter(0),
		m_MaxFPS(30),
		m_AvgFrameTime(0.033),
		m_FrameAllocator(0)
	{
		memset(m_FrameStatistics,0,sizeof(FrameStatistic) * FrameStatisticCount);

		m_FrameAllocator = K15_NEW_T(Allocators[AC_CORE],StackAllocator) StackAllocator(1*MEGABYTE,"FrameAllocator",Allocators[AC_CORE]);

		//creating the Log Manager is first thing we'll do.
		m_LogManager = K15_NEW LogManager();

#if defined (K15_DEBUG)
		m_LogManager->addLog(K15_NEW TextConsoleLog(),true,LogManager::LP_ALL);
#	if defined(_MSC_VER)
		m_LogManager->addLog(K15_NEW VisualStudioLog(),false,LogManager::LP_ALL);
#	endif //_MSC_VER
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
		_LogNormal("Trying to open settings file %s",SettingsFileName.c_str());

		IniFileParser settingsFile(settingsFileName);
		
		if(settingsFile.isValid())
		{
			_LogSuccess("Successfully opened settings file!");
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
			_LogError("Could not open settings file. Error:%s",settingsFile.getError().c_str());
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
		m_ProfileManager = K15_NEW ProfilingManager();;

		_LogNormal("Initializing TaskManager...");
		m_TaskManager = K15_NEW TaskManager;

		_LogNormal("Initializing InputManager...");
		m_InputManager = K15_NEW InputManager;

		_LogNormal("Initializing ThreadWorker...");
		m_ThreadWorker = K15_NEW ThreadWorker;

		_LogNormal("Tasks will get created and added to the task manager.");
		_LogNormal("Creating and adding event task...");
		m_EventTask = K15_NEW EventTask();
		m_TaskManager->addTask(m_EventTask);

		_LogNormal("Creating and adding render task...");
		m_RenderTask = K15_NEW RenderTask();
		m_TaskManager->addTask(m_RenderTask);

		_LogNormal("Creating and adding physics task...");
		m_PhysicsTask = K15_NEW PhysicsTask();
		m_TaskManager->addTask(m_PhysicsTask);

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

		//load input file
		loadInputFile();

		if(!m_RenderTask->getRenderer())
		{
			_LogError("No renderer defined!");
		}
		else
		{
			m_RenderTask->getRenderer()->setRenderWindow(m_RenderWindow);
			if(!m_RenderTask->getRenderer()->initialize())
			{
				_LogError("Could not initialize renderer.");
			}
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
		static double FPSTime = 0.0; //counting to 1 second and then restarts
		static uint32 FPSFrameCounter = 0;
		//clear the frame allocator on the start of each frame
		m_FrameAllocator->clear();

		startFrameTime = getTime();

		K15_PROFILE_BLOCK("Application::onPreTick",
				onPreTick();
		);
		
		K15_PROFILE_BLOCK("TaskManager::update",
		  m_TaskManager->update(m_GameTime);
		);
	
		K15_PROFILE_BLOCK("Application::onPostTick",
		  onPostTick();
		);

		endFrameTime = getTime();

		//so is there any frame time left?
		diffTime = endFrameTime - startFrameTime;

		if(diffTime < m_AvgFrameTime)
		{
			//update running time
			m_RunningTime += m_AvgFrameTime;
			FPSTime += m_AvgFrameTime;

			static double sleepTime = 0.0;
			sleepTime = m_AvgFrameTime - diffTime;

			K15_PROFILE_BLOCK("Application::sleep",
					m_OSLayer.sleep(sleepTime);
			);
		}
		else if(diffTime > m_AvgFrameTime)
		{
			//frame took longer than expected. fire a warning
			//and then clip diffTime to the max frame time
			_LogWarning("Frame %i took %.3f seconds to render! (%.3f seconds is average)",m_FrameCounter,diffTime,m_AvgFrameTime);

			//update running time
			m_RunningTime += diffTime;
			FPSTime += diffTime;

			diffTime = m_AvgFrameTime;
		}

		//update game time
		m_GameTime.setDeltaTime(diffTime);

		if(FPSTime >= 1.0)
		{
			m_AvgFramesPerSecond = (uint32)(FPSFrameCounter / FPSTime);
			FPSTime = 0.0;
			FPSFrameCounter = 0;
		}
    

		//increase frame counter
		if(++m_FrameCounter == UINT_MAX) m_FrameCounter = 0;
		++FPSFrameCounter;
		//save statistic for this frame
		static uint32 FrameStatisticIndex = 0;
		FrameStatisticIndex = m_FrameCounter % (FrameStatisticCount - 1);
		m_FrameStatistics[FrameStatisticIndex].Time = diffTime;
		m_FrameStatistics[FrameStatisticIndex].FrameNumber = m_FrameCounter;
		m_FrameStatistics[FrameStatisticIndex].ProfilingNode = g_ProfileManager->getRootNode();
		m_RenderWindow->setWindowTitle(StringUtil::format("msec: %.3f - FPS:%u - Frame Index: %i",diffTime * 1000,m_AvgFramesPerSecond,m_FrameCounter));

		if(m_FrameCounter > FrameStatisticCount)
		{
			  g_ProfileManager->eraseProfilingForFrame((m_FrameCounter - 1) - FrameStatisticCount);
		}
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
	void Application::loadInputFile()
	{
		_LogNormal("Trying to open input file \"%s\".",InputFileName.c_str());

		static ObjectName actionName;
		static String bindingComplete;
		static String binding;
		static String device;
		static String deviceInput;
		static String inputFileName;
		inputFileName = m_GameRootDir + InputFileName;

		IniFileParser inputFile(inputFileName);

		if(inputFile.isValid())
		{
			IniFileGroup group;
			if(inputFile.getGroupEntries("",&group))
			{
				for(IniFileGroup::IniEntryList::iterator iter = group.entries.begin();iter != group.entries.end();++iter)
				{
					actionName = (*iter).key;
					bindingComplete = (*iter).value;
					String::size_type pos = 0;
					do 
					{
						pos = bindingComplete.find_first_of(',');
						binding = bindingComplete.substr(0,pos == String::npos ? bindingComplete.size() : pos);
						if(pos != String::npos)
						{
							bindingComplete = bindingComplete.substr(pos+1);
						}
						else
						{
							bindingComplete.clear();
						}
						
						if(!binding.empty())
						{
							pos = binding.find_first_of('.');
							if(pos != String::npos)
							{
								device = StringUtil::toLowerString(binding.substr(0,pos));
								deviceInput = binding.substr(pos+1);
								
								if(device == "keyboard")
								{
									InputDevices::Keyboard::InputStringToEnumMap::iterator input_iter = 
										InputDevices::Keyboard::InputStringToEnum.find(ObjectName(deviceInput));

									if(input_iter != InputDevices::Keyboard::InputStringToEnum.end())
									{
										g_InputManager->addInputBinding(actionName,
											K15_NEW InputDevices::Keyboard::InputTrigger(input_iter->second));
									}
									else
									{
										_LogError("Could not find input \"%s\".",binding.c_str());
									}
									
								}
								else if(device == "gamepad")
								{

								}
								else if(device == "mouse")
								{
									InputDevices::Mouse::InputStringToEnumMap::iterator input_iter = 
										InputDevices::Mouse::InputStringToEnum.find(ObjectName(deviceInput));

									if(input_iter != InputDevices::Mouse::InputStringToEnum.end())
									{
										g_InputManager->addInputBinding(actionName,
											K15_NEW InputDevices::Mouse::InputTrigger(input_iter->second));
									}
									else
									{
										_LogError("Could not find input \"%s\".",binding.c_str());
									}
								}
								else
								{
									_LogError("Invalid input device \"%s\"",device.c_str());
								}
							}
							else
							{
								_LogError("invalid binding \"%s\"",bindingComplete.c_str());
							}
						}
					} while(!bindingComplete.empty());
				}
			}
		}
		else
		{
			_LogError("Could not open input file.");
		}
	}
	/*********************************************************************************/
	void Application::loadPluginsFile()
	{
		_LogNormal("Trying to open plugin file \"%s\".",PluginFileName.c_str());

		static String pluginsFileName;
		pluginsFileName = m_GameRootDir + PluginFileName;

		StringSet pluginsToLoad;
		IniFileParser pluginFile(pluginsFileName);

		if(pluginFile.isValid())
		{
			_LogSuccess("Opened plugin file \"%s\".",PluginFileName.c_str());

			IniFileGroup group;
			if(pluginFile.getGroupEntries("",&group))
			{
				for(IniFileGroup::IniEntryList::iterator iter = group.entries.begin();iter != group.entries.end();++iter)
				{
					pluginsToLoad.insert(iter->value);
				}

				initializePlugins(pluginsToLoad);
			}
		}
		else
		{
			_LogError("Could not open plugin file \"%s\". Error:%s",pluginsFileName.c_str(),pluginFile.getError().c_str());
		}
	}
	/*********************************************************************************/
	void Application::initializePlugins(const StringSet& p_PluginNames)
	{
		DynamicLibraryBase* lib = 0;
		char* messageBuffer = (char*)alloca(K15_PLUGIN_INFO_BUFFER_SIZE);
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
		m_ProfileManager->clear();

		_LogNormal("Destroying RenderWindow...");
		K15_DELETE m_RenderWindow;

		_LogNormal("Destroying ThreadWorker...");
		K15_DELETE m_ThreadWorker;

		_LogNormal("Destroying InputManager...");
		K15_DELETE m_InputManager;

		_LogNormal("Destroying TaskMananger...");
		K15_DELETE m_TaskManager;

		_LogNormal("Destroying ProfilingManager...");
		K15_DELETE m_ProfileManager;

		_LogNormal("Destroying EventManager...");
		K15_DELETE m_EventManager;

		_LogNormal("Destroying DynamicLibraryManager...");
		K15_DELETE m_DynamicLibraryManager;

		_LogNormal("Destroying LogManager...");
		K15_DELETE m_LogManager;

		m_FrameAllocator->~StackAllocator();
		K15_DELETE_SIZE(Allocators[AC_CORE],m_FrameAllocator,sizeof(StackAllocator));

		K15_DELETE this; //evil

		BaseAllocatedObject::removeAllocators();

		//K15_DELETE g_MemoryProfiler;
	}
	/*********************************************************************************/
	void Application::loadGameDirFile()
	{
		FileStream gameDirFile(GameDirFileName);

		if(gameDirFile.is_open())
		{
			char* buffer = (char*)alloca(K15_GAMEDIR_BUFFER_SIZE);
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

			if(currentParam.Name == "Renderer")
			{
				String rendererLibrary = StringUtil::format("%s.%s",currentParam.Value.c_str(),m_OSLayer.PluginExtension.c_str());
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
		}
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace