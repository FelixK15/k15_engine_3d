#include "K15_PrecompiledHeader.h"

#include "Android\K15_RenderWindow_Android.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	RenderWindow_Android::RenderWindow_Android()
		: RenderWindowBase(),
		m_Surface(EGL_NO_SURFACE),
		m_Display(EGL_NO_DISPLAY),
		m_Context(EGL_NO_CONTEXT)
	{
		
	}
	/*********************************************************************************/
	RenderWindow_Android::~RenderWindow_Android()
	{
		
	}
	/*********************************************************************************/
	bool RenderWindow_Android::initialize()
	{
		m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		EGLint ver_Major = 0;
		EGLint ver_Minor = 0;

		if(eglInitialize(m_Display,&ver_Major,&ver_Minor) == EGL_FALSE)
		{
			_LogError("Could not initialize GLES Display.");
			return false;
		}

		_LogSuccess("Successfully initialize GLES Display.");
		_LogSuccess("GLES Version: %i.%i",ver_Major,ver_Minor);

		EGLConfig config = 0;
		EGLint configCount = 0;
		EGLint format = 0;

		const EGLint attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
		};

		if(eglChooseConfig(m_Display,attribs,&config,1,&configCount) == EGL_FALSE)
		{
			_LogError("Could not get GLES config.");
			return false;
		}

		if(eglGetConfigAttrib(m_Display,config,EGL_NATIVE_VISUAL_ID,&format) == EGL_FALSE)
		{
			_LogError("Could not get config attributes 'EGL_NATIVE_VISUAL_ID'.");
			return false;
		}

		ANativeWindow_setBuffersGeometry(m_Window,0,0,format);

		if((m_Surface = eglCreateWindowSurface(m_Display,config,m_Window,0)) == 0)
		{
			_LogError("Could not create GLES window surface.");
			return false;
		}

		if((m_Context = eglCreateContext(m_Display,config,0,0)) == 0)
		{
			_LogError("Could not create GLES context.");
			return false;
		}

		if(eglMakeCurrent(m_Display,m_Surface,m_Surface,m_Context) == EGL_FALSE)
		{
			_LogError("Could not set GLES context as current context.");
			return false;
		}

		GLint width,height;
		eglQuerySurface(m_Display,m_Surface,EGL_WIDTH,&width);
		eglQuerySurface(m_Display,m_Surface,EGL_HEIGHT,&height);

		m_CurrentResolution.width = width;
		m_CurrentResolution.height = height;

		return true;
	}
	/*********************************************************************************/
	void RenderWindow_Android::shutdown()
	{
		if(m_Display)
		{
			eglMakeCurrent(m_Display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);

			if(m_Surface)
			{
				eglDestroySurface(m_Display,m_Surface);
				m_Surface = 0;
			}

			if(m_Context)
			{
				eglDestroyContext(m_Display,m_Context);
				m_Context = 0;
			}

			if(m_Display)
			{
				eglTerminate(m_Display);
				m_Display = 0;
			}
		}
		
	}
	/*********************************************************************************/
	void RenderWindow_Android::setWindowTitle( const String& p_WindowTitle )
	{

	}
	/*********************************************************************************/
	void RenderWindow_Android::setResolution( const Resolution& p_Resolution, bool p_ForceChange )
	{

	}
	/*********************************************************************************/
	void RenderWindow_Android::setFullscreen( bool p_Fullscreen )
	{

	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace