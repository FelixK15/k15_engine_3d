/**
 * @file K15_Vector2.h
 * @author	Tobias Funke <t.funke@k15games.de>
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
 *							<--- PLEASE PUT A DESCRIPTION HERE
 */
#pragma once

#ifndef __K15_VECTOR2__
#define __K15_VECTOR2__

namespace K15_Math
{
	class Vector2
	{
	public:
		Vector2();
		Vector2(float vec[2]);
		Vector2(float x,float y);
		Vector2(const Vector2 &vec2);

		~Vector2();

		void Normalize();
		float Magnitude() const;
		float Dot(const Vector2 &vec) const;

		Vector2 &operator*(float scalar);

		float operator*(const Vector2 &vec) const;

		Vector2 &operator+(const Vector2 &vec);
		Vector2 &operator-(const Vector2 &vec);

		Vector2 &operator+=(const Vector2 &vec);
		Vector2 &operator-=(const Vector2 &vec);

		bool operator<(const Vector2 &vec);
		bool operator>(const Vector2 &vec);

		bool operator<=(const Vector2 &vec);
		bool operator>=(const Vector2 &vec);

		bool operator==(const Vector2 &vec);
		bool operator!=(const Vector2 &vec);

	public:

		union{
			struct{
				float x,y;
			};
			float vec[2];
		};

	private:
		Vector2& _Add(const Vector2 &vec);
	};
}

#endif //__K15_VECTOR2__