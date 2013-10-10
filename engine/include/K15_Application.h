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

#ifndef _K15Engine_System_Application_h_
#define _K15Engine_System_Application_h_

#include "K15_Prerequisites.h"
#include "K15_Singleton.h"
#include "K15_StackAllocator.h"
#include "K15_ApplicationParameter.h"

#if defined (K15_OS_WINDOWS)
#	include "K15_ApplicationOSLayer_Win32.h"
#endif //K15_OS_WINDOWS

namespace K15_Engine { namespace System {
  /*********************************************************************************/
  typedef K15_Set(String) StringSet;
  typedef K15_List(ApplicationModule*) ApplicationModuleList;
  typedef K15_List(ApplicationParameter) ApplicationParameterList;
  /*********************************************************************************/
  class K15_API_EXPORT Application : public Singleton<Application>,
                                     public StackAllocator
  {
  public:
	  /*********************************************************************************/
	  static const String SettingsFileName;
	  static const String PluginFileName;
	  /*********************************************************************************/
  public:
    Application();
    Application(int p_CommandCount,char** p_Commands);
    ~Application();
    
	  inline void setGameRootDir(const String& p_GameRootDir);

    inline const String& getGameRootDir() const;
    inline const StringSet& getPluginList() const;
    inline const StringSet& getCommandList() const;
    inline const ApplicationParameterList& getApplicationParameter() const;

    void setWindowTitle(const String& p_WindowTitle);

    void initialize();

    void run();

    void tick();

    void shutdown();

    void onPreTick();
    void onPostTick();

    void onBeforeRender();
    void onAfterRender();

    inline TaskManager* getTaskManager() const;
    inline ProfilingManager* getProfileManager() const;
    inline EventManager* getEventManager() const;
    inline DynamicLibraryManager* getDynamicLibraryManager() const;
    inline RenderWindowBase* getRenderWindow() const;
    inline LogManager* getLogManager() const;
	inline const ApplicationOSLayerType& getOSLayer() const;

    inline const GameTime& getGameTime() const;
    inline const double getDeltaTime() const;
    inline const double getRawDeltaTime() const;

  private:
    void createCommandList(int p_CommandCount,char** p_Commands);
    void createApplicationParameterList();
    void addSingleApplicationParameter(const String& p_Parameter,const String& p_Group);
    void loadSettingsFile();
    void loadPluginsFile();
    void initializePlugins(const StringSet& p_PluginNames);

  private:
	bool m_Running;
    
    GameTime m_GameTime;

	  String m_GameRootDir;

	StringSet m_Plugins;
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
  }; //end of Application class definition
#include "K15_Application.inl"
}} //end of K15_Engine::System namespace

#endif //_K15Engine_System_Application_h_
