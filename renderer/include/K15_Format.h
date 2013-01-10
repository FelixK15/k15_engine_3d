/**
 * @file K15_Format.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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
#pragma once

#ifndef __K15_FORMAT__
#define __K15_FORMAT__

namespace K15_Renderer
{
	enum FormatType{
		FT_8R_SINT = 0,
		FT_8R_UINT,
		FT_8R8G_SINT,
		FT_8R8G_UINT,
		FT_8R8G8B8A_SINT,
		FT_8R8R8B8A_UINT,

		FT_32R_SINT,
		FT_32R_UINT,
		FT_32R_FLOAT,
		FT_32R32G_SINT,
		FT_32R32G_UINT,
		FT_32R32G_FLOAT,
		FT_32R32G32B_SINT,
		FT_32R32G32B_UINT,
		FT_32R32G32B_FLOAT,
		FT_32R32G32B32A_SINT,
		FT_32R32G32B32A_UINT,
		FT_32R32G32B32A_FLOAT,
		FT_UNKNOWN
	};
}

#endif