/**
 * @file K15_Matrix4x4.cpp
 * @author  Tobias Funke <tobyfunke@web.de>
 * @version 1.0
 * @date 2012/10/20
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
#include "K15_Matrix4x4.h"
using namespace K15_Math;


const Matrix4x4<float> Matrix4x4<float>::IdentityMatrix = Matrix4x4<float>(MF_IDENT);
const Matrix4x4<double> Matrix4x4<double>::IdentityMatrix = Matrix4x4<double>(MF_IDENT);