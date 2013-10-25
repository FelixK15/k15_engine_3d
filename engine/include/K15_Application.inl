/**
 * @file K15_Application.inl
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
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
/*********************************************************************************/
inline void Application::setGameRootDir(const String& p_GameRootDir)
{
	m_GameRootDir = p_GameRootDir;
}
/*********************************************************************************/
inline const String& Application::getGameRootDir() const
{
	return m_GameRootDir;
}
/*********************************************************************************/
inline const StringSet& Application::getPluginList() const
{
	return m_Plugins;
}
/*********************************************************************************/
inline const StringSet& Application::getCommandList() const
{
	return m_Commands;
}
/*********************************************************************************/
inline const ApplicationParameterList& Application::getApplicationParameter() const
{
	return m_ApplicationParameter;
}
/*********************************************************************************/
inline RenderTask* Application::getRenderTask() const
{
	return m_RenderTask;
}
/*********************************************************************************/
inline PhysicsTask* Application::getPhysicsTask() const
{
	return m_PhysicsTask;
}
/*********************************************************************************/
inline TaskManager* Application::getTaskManager() const
{
	return m_TaskManager;
}
/*********************************************************************************/
inline ProfilingManager* Application::getProfileManager() const
{
	return m_ProfileManager;
}
/*********************************************************************************/
inline EventManager* Application::getEventManager() const
{
	return m_EventManager;
}
/*********************************************************************************/
inline DynamicLibraryManager* Application::getDynamicLibraryManager() const
{
	return m_DynamicLibraryManager;
}
/*********************************************************************************/
inline RenderWindowBase* Application::getRenderWindow() const
{
	return m_RenderWindow;
}
/*********************************************************************************/
inline LogManager* Application::getLogManager() const
{
	return m_LogManager;
}
/*********************************************************************************/
inline const StackAllocator& Application::getFrameAllocator() const
{
	return m_FrameAllocator;
}
/*********************************************************************************/
inline double Application::getRunningTime() const
{
	return m_RunningTime;
}
/*********************************************************************************/
inline const GameTime& Application::getGameTime() const
{
  return m_GameTime;
}
/*********************************************************************************/
inline const double Application::getDeltaTime() const
{
  return m_GameTime.getDeltaTime();
}
/*********************************************************************************/
inline const double Application::getRawDeltaTime() const
{
  return m_GameTime.getRawDeltaTime();
}
/*********************************************************************************/
inline String Application::getLastError() const
{
	return m_OSLayer.getError();
}
/*********************************************************************************/
inline double Application::getTime() const
{
	return m_OSLayer.getTime();
}
/*********************************************************************************/
inline void Application::setMaxFPS(uint16 p_MaxFPS)
{
	m_MaxFPS = p_MaxFPS;
	m_AvgFrameTime = 1.0 / p_MaxFPS;
}
/*********************************************************************************/
inline uint16 Application::getMaxFPS() const
{
	return m_MaxFPS;
}
/*********************************************************************************/
inline void Application::setRunning(bool p_Running) 
{
	m_Running = p_Running;
}
/*********************************************************************************/
inline bool Application::getRunning() const
{
	return m_Running;
}
/*********************************************************************************/
inline const FrameStatistic& Application::getFrameStatistic(uint32 p_FrameNumber) const
{
	K15_ASSERT(p_FrameNumber < FrameStatisticCount,
		StringUtil::format("Requestes frame statistics from frame %i, but only statistics from the last %i frames get saved.",
		p_FrameNumber,FrameStatisticCount));
	return m_FrameStatistics[p_FrameNumber];
}
/*********************************************************************************/