/**
 * @file K15_Game.h
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
 *
 * @section DESCRIPTION
 *
 */
#pragma once

#include "K15_StdInclude.h"

#ifndef __K15_GAME__
#define __K15_GAME__

namespace K15_EngineV2
{
	class ResourceFile;

	class K15ENGINE2_API Game
	{
	public:
		virtual bool Initialize() = 0;
		virtual void Update(const GameTime &gtTime) = 0;
		virtual void Shutdown() = 0;

// 		virtual void OnKeyDown(U8 cKey) = 0;
// 		virtual void OnKeyUp(U8 cKey) = 0;
// 
// 		virtual void OnMouseMove(U32 iX,U32 iY) = 0;
// 		virtual void OnMousePress(U32 iX,U32 iY,U8 cButton) = 0;
// 		virtual void OnMouseRelease(U32 iX,U32 iY,U8 cButton) = 0;

/*		virtual ResourceFile *GetResourceFile() = 0;*/
	};
}

#endif //__K15_GAME__