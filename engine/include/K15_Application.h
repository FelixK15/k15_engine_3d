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

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_Singleton.h"
#	include "K15_StringUtil.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_StackAllocator.h"
#include "K15_ApplicationParameter.h"
#include "K15_GameTime.h"
#include "K15_FrameStatistic.h"
#include "K15_Serializer.h"

#if defined (K15_OS_WINDOWS)
#	include "Win32\K15_ApplicationOSLayer_Win32.h"
#endif //K15_OS_WINDOWS

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  typedef List(ApplicationModule*) ApplicationModuleList;
  typedef List(ApplicationParameter) ApplicationParameterList;
  /*********************************************************************************/
  class K15_CORE_API Application : public Singleton<Application>,
								   public SerializableObject<Application>,
								   public CoreAllocatedObject
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

	  void setWindowTitle(const String& p_WindowTitle);

	  void initialize(int p_CommandCount,char** p_Commands);
	  void initialize();

	  void run();

	  void tick();

	  void shutdown();

	  void onPreTick();
	  void onPostTick();

	  void onBeforeRender();
	  void onAfterRender();

	  RendererBase* getRenderer() const;

	  INLINE void addApplicationModule(ApplicationModule* p_Module);

	  INLINE uint32 getFrameCount() const;

	  INLINE RenderTask* getRenderTask() const;
	  INLINE PhysicsTask* getPhysicsTask() const;

	  INLINE TaskManager* getTaskManager() const;
	  INLINE ProfilingManager* getProfileManager() const;
	  INLINE EventManager* getEventManager() const;
	  INLINE DynamicLibraryManager* getDynamicLibraryManager() const;
	  INLINE RenderWindowBase* getRenderWindow() const;
	  INLINE LogManager* getLogManager() const;
	  INLINE InputManager* getInputManager() const;
	  INLINE const StackAllocator* getFrameAllocator() const;
	  INLINE const ApplicationOSLayerType& getOSLayer() const;

	  INLINE double getRunningTime() const;

	  INLINE void setRunning(bool p_Running);
	  INLINE bool getRunning() const;

	  INLINE String getLastError() const;
	  INLINE double getTime() const;

	  INLINE const GameTime& getGameTime() const;
	  INLINE const double getDeltaTime() const;
	  INLINE const double getRawDeltaTime() const;

	  INLINE void setMaxFPS(uint32 p_MaxFPS);
	  INLINE uint32 getMaxFPS() const;

	  INLINE const FrameStatistic& getFrameStatistic(uint32 p_FrameNumber) const;
	  INLINE FrameStatistic& getCurrentFrameStatistic();

	  INLINE uint32 getFramesPerSecond() const;

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
	expose_read double m_AvgFrameTime;
				double m_RunningTime;
				double m_TimeLastFrame;

				StackAllocator* m_FrameAllocator; //allocator which will get reset each frame.
				FrameStatistic m_FrameStatistics[FrameStatisticCount];
				GameTime m_GameTime;

	expose_read String m_GameRootDir;

	expose_read StringSet m_Plugins;
    
				StringSet m_Commands;
				ApplicationParameterList m_ApplicationParameter;
				ApplicationModuleList m_LoadedModules;

				ApplicationOSLayerType m_OSLayer;

				RenderWindowBase* m_RenderWindow;
				TaskManager* m_TaskManager;
				ProfilingManager* m_ProfileManager;
				EventManager* m_EventManager;
				LogManager* m_LogManager;
				DynamicLibraryManager* m_DynamicLibraryManager;
				InputManager* m_InputManager;
				ThreadWorker* m_ThreadWorker;

				RenderTask* m_RenderTask;
				PhysicsTask* m_PhysicsTask;
				EventTask* m_EventTask;
#				if defined K15_DEBUG
				MemoryProfilingTask* m_MemoryProfilingTask;
#				endif
  }; //end of Application class definition
#include "K15_Application.inl"
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Application_h_
