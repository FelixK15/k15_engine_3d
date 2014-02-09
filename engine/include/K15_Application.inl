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
uint32 Application::getFrameCount() const
{
  return m_FrameCounter;
}
/*********************************************************************************/
void Application::setGameRootDir(const String& p_GameRootDir)
{
	m_GameRootDir = p_GameRootDir;
}
/*********************************************************************************/
const String& Application::getGameRootDir() const
{
	return m_GameRootDir;
}
/*********************************************************************************/
const StringSet& Application::getPluginList() const
{
	return m_Plugins;
}
/*********************************************************************************/
const StringSet& Application::getCommandList() const
{
	return m_Commands;
}
/*********************************************************************************/
const ApplicationParameterList& Application::getApplicationParameter() const
{
	return m_ApplicationParameter;
}
/*********************************************************************************/
RenderTask* Application::getRenderTask() const
{
	return m_RenderTask;
}
/*********************************************************************************/
PhysicsTask* Application::getPhysicsTask() const
{
	return m_PhysicsTask;
}
/*********************************************************************************/
TaskManager* Application::getTaskManager() const
{
	return m_TaskManager;
}
/*********************************************************************************/
ProfilingManager* Application::getProfileManager() const
{
	return m_ProfileManager;
}
/*********************************************************************************/
EventManager* Application::getEventManager() const
{
	return m_EventManager;
}
/*********************************************************************************/
DynamicLibraryManager* Application::getDynamicLibraryManager() const
{
	return m_DynamicLibraryManager;
}
/*********************************************************************************/
RenderWindowBase* Application::getRenderWindow() const
{
	return m_RenderWindow;
}
/*********************************************************************************/
LogManager* Application::getLogManager() const
{
	return m_LogManager;
}
/*********************************************************************************/
InputManager* Application::getInputManager() const
{
	return m_InputManager;
}
/*********************************************************************************/
const StackAllocator* Application::getFrameAllocator() const
{
	return m_FrameAllocator;
}
/*********************************************************************************/
double Application::getRunningTime() const
{
	return m_RunningTime;
}
/*********************************************************************************/
const GameTime& Application::getGameTime() const
{
  return m_GameTime;
}
/*********************************************************************************/
const double Application::getDeltaTime() const
{
  return m_GameTime.getDeltaTime();
}
/*********************************************************************************/
const double Application::getRawDeltaTime() const
{
  return m_GameTime.getRawDeltaTime();
}
/*********************************************************************************/
String Application::getLastError() const
{
	return m_OSLayer.getError();
}
/*********************************************************************************/
double Application::getTime() const
{
	return m_OSLayer.getTime();
}
/*********************************************************************************/
void Application::setMaxFPS(uint32 p_MaxFPS)
{
	m_MaxFPS = p_MaxFPS;
	m_AvgFrameTime = 1.0 / p_MaxFPS;
}
/*********************************************************************************/
uint32 Application::getMaxFPS() const
{
	return m_MaxFPS;
}
/*********************************************************************************/
void Application::setRunning(bool p_Running) 
{
	m_Running = p_Running;
}
/*********************************************************************************/
bool Application::getRunning() const
{
	return m_Running;
}
/*********************************************************************************/
const FrameStatistic& Application::getFrameStatistic(uint32 p_FrameNumber) const
{
	K15_ASSERT(p_FrameNumber < FrameStatisticCount,
		StringUtil::format("Requestes frame statistics from frame %i, but only statistics from the last %i frames get saved.",
		p_FrameNumber,FrameStatisticCount));
	return m_FrameStatistics[p_FrameNumber];
}
/*********************************************************************************/
const ApplicationOSLayerType& Application::getOSLayer() const
{
  return m_OSLayer;
}
/*********************************************************************************/
FrameStatistic& Application::getCurrentFrameStatistic()
{
  return m_FrameStatistics[m_FrameCounter % (FrameStatisticCount - 1)];
}
/*********************************************************************************/
uint32 Application::getFramesPerSecond() const
{
  return m_AvgFramesPerSecond;
}
/*********************************************************************************/
void Application::addApplicationModule(ApplicationModule* p_Module)
{
	m_LoadedModules.push_back(p_Module);
}
/*********************************************************************************/