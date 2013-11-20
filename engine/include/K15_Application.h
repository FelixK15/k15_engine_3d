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

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_Singleton.h"
#	include "K15_StringUtil.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_StackAllocator.h"
#include "K15_ApplicationParameter.h"
#include "K15_GameTime.h"
#include "K15_FrameStatistic.h"

#if defined (K15_OS_WINDOWS)
#	include "Win32\K15_ApplicationOSLayer_Win32.h"
#endif //K15_OS_WINDOWS

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  typedef List(ApplicationModule*) ApplicationModuleList;
  typedef List(ApplicationParameter) ApplicationParameterList;
  /*********************************************************************************/
  exposed_class K15_CORE_API Application : public Singleton<Application>,
											 public StackAllocator
  {
  public:
	  /*********************************************************************************/
	  static const String SettingsFileName;
	  static const String PluginFileName;
	  static const String GameDirFileName;
	  static const uint32 FrameStatisticCount = 1024;
	  static const uint32 FrameAllocatorSize = MEGABYTE;
	  /*********************************************************************************/
  public:
    Application();
    ~Application();
    
	inline void setGameRootDir(const String& p_GameRootDir);

    inline const String& getGameRootDir() const;
    inline const StringSet& getPluginList() const;
    inline const StringSet& getCommandList() const;
    inline const ApplicationParameterList& getApplicationParameter() const;

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

	inline uint32 getFrameCount() const;

	inline RenderTask* getRenderTask() const;
	inline PhysicsTask* getPhysicsTask() const;

    inline TaskManager* getTaskManager() const;
    inline ProfilingManager* getProfileManager() const;
    inline EventManager* getEventManager() const;
    inline DynamicLibraryManager* getDynamicLibraryManager() const;
    inline RenderWindowBase* getRenderWindow() const;
    inline LogManager* getLogManager() const;
	inline InputManager* getInputManager() const;
	inline const StackAllocator& getFrameAllocator() const;
	inline const ApplicationOSLayerType& getOSLayer() const;

	inline double getRunningTime() const;

	inline void setRunning(bool p_Running);
	inline bool getRunning() const;

	inline String getLastError() const;
	inline double getTime() const;

    inline const GameTime& getGameTime() const;
    inline const double getDeltaTime() const;
    inline const double getRawDeltaTime() const;

	inline void setMaxFPS(uint16 p_MaxFPS);
	inline uint16 getMaxFPS() const;

	inline const FrameStatistic& getFrameStatistic(uint32 p_FrameNumber) const;
  
  private:
    void createCommandList(int p_CommandCount,char** p_Commands);
    void createApplicationParameterList();
    void addSingleApplicationParameter(const String& p_Parameter,const String& p_Group);
    void loadSettingsFile();
    void loadPluginsFile();
    void initializePlugins(const StringSet& p_PluginNames);
	void loadGameDirFile();
	void processSettings();

  private:
				bool m_Running;
	
	expose		uint16 m_MaxFPS;
	expose_read uint32 m_FrameCounter;
	expose_read double m_AvgFrameTime;
				double m_RunningTime;
				double m_TimeLastFrame;

				StackAllocator m_FrameAllocator; //allocator which will get reset each frame.
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

				RenderTask* m_RenderTask;
				PhysicsTask* m_PhysicsTask;
				EventTask* m_EventTask;
  }; //end of Application class definition
#include "K15_Application.inl"
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Application_h_
