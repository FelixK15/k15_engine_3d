#include "K15_PrecompiledHeader.h"

#include "Android\K15_RenderWindow_Android.h"
#include "Android\K15_ApplicationLayerOS_Android.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	RenderWindow_Android::RenderWindow_Android()
		: RenderWindowBase(),
		m_Window(0)
	{
		
	}
	/*********************************************************************************/
	RenderWindow_Android::~RenderWindow_Android()
	{
		
	}
	/*********************************************************************************/
	bool RenderWindow_Android::initialize()
	{
		if((m_Window = ((ApplicationOSLayerType)g_Application->getOSLayer()).getAndroidApp()->window) == 0)
		{
			_LogError("android_app::window is NULL");
			return false;
		}
		else
		{
			_LogSuccess("Successfully retrieved android_app::window.");
		}
		return true;
	}
	/*********************************************************************************/
	void RenderWindow_Android::shutdown()
	{
		
	}
	/*********************************************************************************/
	void RenderWindow_Android::setWindowTitle(const String& p_WindowTitle)
	{

	}
	/*********************************************************************************/
	void RenderWindow_Android::setResolution(const Resolution& p_Resolution, bool p_ForceChange)
	{
		RenderWindowBase::setResolution(p_Resolution,p_ForceChange);
	}
	/*********************************************************************************/
	void RenderWindow_Android::setIsFullscreen(bool p_Fullscreen)
	{
		RenderWindowBase::setIsFullscreen(p_Fullscreen);
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace