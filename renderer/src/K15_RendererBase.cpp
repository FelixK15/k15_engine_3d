/**
 * @file K15_RendererBase.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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

#include "K15_RendererBase.h"

namespace K15_Engine { namespace Rendering { 
	/*********************************************************************************/
	uint8 RendererBase::PixelFormatSize[PF_COUNT]= {
		24,	//PF_RGB_8_I
		24, //F_RGB_8_UI,
		48, //PF_RGB_16_I
		48, //PF_RGB_16_U
		48, //PF_RGB_16_F
		96, //PF_RGB_32_I
		96, //PF_RGB_32_U
		96, //PF_RGB_32_F
		32, //PF_RGBA_8_I
		32, //PF_RGBA_8_U
		64, //PF_RGBA_16_I
		64, //PF_RGBA_16_U
		64, //PF_RGBA_16_F
		128, //PF_RGBA_32_I
		128, //PF_RGBA_32_U
		128  //PF_RGBA_32_F
	}; //PixelFormatSize
	/*********************************************************************************/
	uint8 RendererBase::DepthFormatSize[DBF_COUNT] = {
		16, //DBF_COMPONENT_16_I
		24, //DBF_COMPONENT_24_I
		32, //DBF_COMPONENT_32_I
		32  //DBF_COMPONENT_32_F
	}; //DepthFormatSize
	/*********************************************************************************/
	uint8 RendererBase::StencilFormatSize[SBF_COUNT] = {
		1, //SBF_COMPONENT_1_I
		4, //SBF_COMPONENT_4_I
		8, //SBF_COMPONENT_8_I
		16 //SBF_COMPONENT_16_I
	}; //StencilFormatSize
	/*********************************************************************************/
}}//end of K15_Engine::Rendering namespace