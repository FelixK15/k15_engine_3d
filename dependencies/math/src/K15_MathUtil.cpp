/**
 * @file K15_Language.h
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
 */

#include "K15_MathUtil.h"

namespace K15_Engine { namespace Math {
	/*********************************************************************************/
	const Real MathUtil::Pi = 4.0f*atan(1.0f);
	const Real MathUtil::HalfPi = Pi * 0.5f;
	const Real MathUtil::QuarterPi = Pi * 0.25f;
	const Real MathUtil::TwoPi = Pi + Pi;
	const Real MathUtil::DegreeToRadian = Pi / 180.f;
	const Real MathUtil::RadianToDegree = 180.f / Pi;
	/*********************************************************************************/
}}//end of K15_Engine::Math namespace