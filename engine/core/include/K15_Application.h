/**
 * @file K15_Application.h
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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
 *
 * @section DESCRIPTION
 *
 */

#ifndef _K15Engine_Core_Application_h_
#define _K15Engine_Core_Application_h_

#include "K15_Prerequisites.h"
#include "K15_Singleton.h"
#include "K15_StringUtil.h"
#include "K15_AllocatedObject.h"
#include "K15_StackAllocator.h"
#include "K15_ApplicationParameter.h"
#include "K15_GameTime.h"
#include "K15_FrameStatistic.h"
#include "K15_Serializer.h"

#if defined (K15_OS_WINDOWS)
    #include "win32/K15_OSLayer_Win32.h"
#elif defined (K15_OS_ANDROID)
    #include "android/K15_OSLayer_Android.h"
#elif defined (K15_OS_LINUX)
    #include "linux/K15_OSLayer_Linux.h"
#endif //K15_OS_WINDOWS

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	typedef DynamicArray(ApplicationModule*) ApplicationModuleList;
	typedef DynamicArray(ApplicationParameter) ApplicationParameterList;
	/*********************************************************************************/
	class K15_CORE_API Application : public SerializableObject<Application>,
									 public CoreAllocatedObject,
									 public Singleton<Application>
	{
	public:
		/*********************************************************************************/
		static const String SettingsFileName;
		static const String PluginFileName;
		static const String GameDirFileName;
		static const String InputFileName;
		static const uint32 FrameStatisticCount = 1024;
		/*********************************************************************************/
	public:
		Application();
		~Application();
    
		INLINE void setGameRootDir(const String& p_GameRootDir);

		INLINE const String& getGameRootDir() const;
		INLINE const StringSet& getPluginList() const;
		INLINE const StringSet& getCommandList() const;
		INLINE const ApplicationParameterList& getApplicationParameter() const;

    #if defined K15_OS_ANDROID
		void initialize(android_app* p_App);
    #endif //K15_OS_ANDROID
		void initialize(int p_CommandCount,char** p_Commands);
		void initialize();
		
		void tryInitializeRenderer();

		void run();

		void tick();

		void shutdown();

		void onPreTick();
		void onPostTick();

		void onBeforeRender();
		void onAfterRender();

		RendererBase* getRenderer() const;
		void setRenderer(RendererBase* p_Renderer);

		INLINE void addApplicationModule(ApplicationModule* p_Module);

		INLINE uint32 getFrameCount() const;

		INLINE RenderTask* getRenderTask() const;
		INLINE PhysicsTask* getPhysicsTask() const;

		INLINE TaskManager* getTaskManager() const;
		INLINE ProfilingManager* getProfileManager() const;
		INLINE EventManager* getEventManager() const;
		INLINE DynamicLibraryManager* getDynamicLibraryManager() const;
		INLINE LogManager* getLogManager() const;
		INLINE InputManager* getInputManager() const;
		INLINE StackAllocator* getFrameAllocator();

		INLINE float getRunningTime() const;

		INLINE void setRunning(bool p_Running);
		INLINE bool getRunning() const;

		INLINE String getLastError();
		INLINE float getTime();

		INLINE const GameTime& getGameTime() const;
		INLINE const float getDeltaTime() const;
		INLINE const float getRawDeltaTime() const;

		INLINE void setMaxFPS(uint32 p_MaxFPS);
		INLINE uint32 getMaxFPS() const;

		INLINE const FrameStatistic& getFrameStatistic(uint32 p_FrameNumber) const;
		INLINE FrameStatistic& getCurrentFrameStatistic();

		INLINE uint32 getFramesPerSecond() const;

		void setGameState(GameStateBase* p_GameState);
		INLINE GameStateBase* getGameState() const;

	private:
		void createCommandList(int p_CommandCount,char** p_Commands);
		void createApplicationParameterList();
		void loadSettingsFile();
		void loadPluginsFile();
		void loadInputFile();
		void initializePlugins(const StringSet& p_PluginNames);
		void loadGameDirFile();
		void processSettings();
	  
	private:
				bool m_Running;

	expose		uint32 m_MaxFPS;
	expose_read uint32 m_FrameCounter;
	expose_read uint32 m_AvgFramesPerSecond;
	expose_read float m_AvgFrameTime;
				float m_RunningTime;
				float m_TimeLastFrame;

				StackAllocator* m_FrameAllocator; //allocator which will get reset each frame.
				FrameStatistic m_FrameStatistics[FrameStatisticCount];
				GameTime m_GameTime;

	expose_read String m_GameRootDir;

	expose_read StringSet m_Plugins;
    
				StringSet m_Commands;
				ApplicationParameterList m_ApplicationParameter;
				ApplicationModuleList m_LoadedModules;

				TaskManager* m_TaskManager;
				ResourceManager* m_ResourceManager;
				ProfilingManager* m_ProfileManager;
				EventManager* m_EventManager;
				LogManager* m_LogManager;
				DynamicLibraryManager* m_DynamicLibraryManager;
				InputManager* m_InputManager;
				ThreadWorker* m_ThreadWorker;

				RenderTask* m_RenderTask;
				PhysicsTask* m_PhysicsTask;
				EventTask* m_EventTask;
				
				GameStateBase* m_GameState;
#			if defined K15_DEBUG
				MemoryProfilingTask* m_MemoryProfilingTask;
#			endif
	}; //end of Application class definition
#	include "K15_Application.inl"
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Application_h_