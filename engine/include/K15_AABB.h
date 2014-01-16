/**
 * @file K15_AABB.h
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
    (Max)
		  -------------
	   /|	         /|
	  /	|	        / |
	 /  |        /  |
  /   /-------/---/ 
 /-----------|---/
 |  /        |  /
 | /		     | /
 |/	         |/
 |-----------| (Min)
 */

#ifndef _K15Engine_Rendering_AABB_h_
#define _K15Engine_Rendering_AABB_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_Vector4.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API AABB
	{
	public:
		/*********************************************************************************/
		enum eCornerTypes
		{
			CT_FAR_LEFT_TOP = 0,
			CT_FAR_RIGHT_TOP,
			CT_FAR_LEFT_BOTTOM,
			CT_FAR_RIGHT_BOTTOM,

			CT_NEAR_LEFT_TOP,
			CT_NEAR_RIGHT_TOP,
			CT_NEAR_LEFT_BOTTOM,
			CT_NEAR_RIGHT_BOTTOM,

			CT_COUNT
		};//CornerTypes
		typedef FixedArray(Vector4,CT_COUNT) CornerArray;
	public:
		AABB();
		AABB(const Math::Vector4& p_Min, const Math::Vector4& p_Max);
		~AABB();

		INLINE const Math::Vector4& getFarLeftTop() const;
		INLINE const Math::Vector4& getFarLeftBottom() const;
		INLINE const Math::Vector4& getFarRightTop() const;
		INLINE const Math::Vector4& getFarRightBottom() const;

		INLINE const Math::Vector4& getNearLeftTop() const;
		INLINE const Math::Vector4& getNearLeftBottom() const;
		INLINE const Math::Vector4& getNearRightTop() const;
		INLINE const Math::Vector4& getNearRightBottom() const;

		INLINE void setMin(const Math::Vector4& p_Min);
		INLINE void setMax(const Math::Vector4& p_Max);

		INLINE const Math::Vector4& getMin() const;
		INLINE const Math::Vector4& getMax() const;

	private:
		void _calculateCorners();

	private:
		Math::Vector4 m_Min;
		Math::Vector4 m_Max;
		CornerArray m_Corners;
	};// end of AABB class declaration
#include "K15_AABB.inl"
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_AABB_h_