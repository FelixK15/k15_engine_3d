/**
 * @file K15_ColorRGB.h
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

#ifndef __K15_COLORRGB__
#define __K15_COLORRGB__

namespace K15_Renderer
{
	struct ColorRGB
	{
		union 
		{
			struct  
			{
				float R;
				float G;
				float B;
			};

			float C[3];
		};

		ColorRGB();
		ColorRGB(float R,float G,float B);
		ColorRGB(const ColorRGB &clr);

		const ColorRGB &operator=(const ColorRGB &clr);
		const ColorRGB &operator+(const ColorRGB &clr);
		const ColorRGB &operator-(const ColorRGB &clr);
		const ColorRGB &operator*(const ColorRGB &clr);
		const ColorRGB &operator/(const ColorRGB &clr);

		const ColorRGB &operator*(float fScalar);
		const ColorRGB &operator/(float fScalar);

		void operator+=(const ColorRGB &clr);
		void operator-=(const ColorRGB &clr);
		void operator*=(const ColorRGB &clr);
		void operator/=(const ColorRGB &clr);

		void operator*=(float fScalar);
		void operator/=(float fScalar);

		bool operator==(const ColorRGB &clr);
		bool operator!=(const ColorRGB &clr);

	private:
		void _ClampValues();
	};
}

#endif //__K15_COLORRGB__