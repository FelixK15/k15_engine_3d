/**
 * @file K15_ColorRGBA.h
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

#ifndef __K15_COLORRGBA__
#define __K15_COLORRGBA__

namespace K15_Renderer
{
	struct ColorRGBA
	{
		union
		{
			struct
			{
				float R;
				float G;
				float B;
				float A;
			};

			float C[4];
		};

		ColorRGBA();
		ColorRGBA(float R,float G,float B,float A);
		ColorRGBA(const ColorRGBA &clr);

		const ColorRGBA &operator=(const ColorRGBA &clr);
		const ColorRGBA &operator+(const ColorRGBA &clr);
		const ColorRGBA &operator-(const ColorRGBA &clr);
		const ColorRGBA &operator*(const ColorRGBA &clr);
		const ColorRGBA &operator/(const ColorRGBA &clr);

		const ColorRGBA &operator*(float fScalar);
		const ColorRGBA &operator/(float fScalar);

		void operator+=(const ColorRGBA &clr);
		void operator-=(const ColorRGBA &clr);
		void operator*=(const ColorRGBA &clr);
		void operator/=(const ColorRGBA &clr);

		void operator*=(float fScalar);
		void operator/=(float fScalar);

		bool operator==(const ColorRGBA &clr);
		bool operator!=(const ColorRGBA &clr);

	private:
		void _ClampValues();
	};
}

#endif //__K15_COLORRGBA__