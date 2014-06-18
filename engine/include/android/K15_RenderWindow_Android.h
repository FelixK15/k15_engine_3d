/**
 * @file K15_RenderWindow_Android.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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
 * 
 */

#ifndef _K15Engine_Core_RenderWindow_Android_h_
#define _K15Engine_Core_RenderWindow_Android_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_RenderWindowBase.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API RenderWindow_Android : public RenderWindowBase
	{
	public:
		RenderWindow_Android();
		~RenderWindow_Android();

		virtual bool initialize() OVERRIDE;
		virtual void shutdown() OVERRIDE;

		virtual void setWindowTitle(const String& p_WindowTitle) OVERRIDE;
		virtual void setResolution(const Resolution& p_Resolution, bool p_ForceChange) OVERRIDE;
		virtual void setIsFullscreen(bool p_Fullscreen) OVERRIDE;

		INLINE void setNativeWindow(ANativeWindow* p_Window);
		INLINE ANativeWindow* getNativeWindow() const;
	private:
		ANativeWindow* m_Window;
	};// end of RenderWindow_Android class declaration
#	include "K15_RenderWindow_Android.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_RenderWindow_Android_h_