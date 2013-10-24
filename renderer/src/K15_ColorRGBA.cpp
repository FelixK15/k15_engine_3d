/**
 * @file K15_ColorRGBA.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#include "K15_RendererPrecompiledHeaders.h"
#include "K15_ColorRGBA.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	const ColorRGBA ColorRGBA::Transparent = ColorRGBA(1.0f,1.0f,1.0f,0.0f);
	const ColorRGBA ColorRGBA::White = ColorRGBA(1.0f,1.0f,1.0f);
	const ColorRGBA ColorRGBA::Black = ColorRGBA(0.0f,0.0f,0.0f);
	const ColorRGBA ColorRGBA::Red = ColorRGBA(1.0f,0.0f,0.0f);
	const ColorRGBA ColorRGBA::Green = ColorRGBA(0.0f,1.0f,0.0f);
	const ColorRGBA ColorRGBA::Blue = ColorRGBA(1.0f,0.0f,1.0f);
	const ColorRGBA ColorRGBA::Yellow = ColorRGBA(1.0f,1.0f,0.0f);
	const ColorRGBA ColorRGBA::Grey = ColorRGBA(0.5f,0.5f,0.5f);
	const ColorRGBA ColorRGBA::Brown = ColorRGBA(0.65f,0.27f,0.0f);
	const ColorRGBA ColorRGBA::Purple = ColorGBA(0.5f,0.0f,0.45f);
	const ColorRGBA ColorRGBA::Pink = ColorRGBA(1.0f,0.5f,0.65f);
	const ColorRGBA ColorRGBA::DarkRed = ColorRGBA(0.5f,0.0f,0.0f);
	const ColorRGBA ColorRGBA::DarkBlue = ColorRGBA(0.0f,0.0f,0.5f);
	const ColorRGBA ColorRGBA::DarkGreen = ColorRGBA(0.0f,0.5f,0.0f);
	const ColorRGBA ColorRGBA::DarkYellow = ColorRGBA(0.5f,0.5f,0.0f);
	const ColorRGBA ColorRGBA::DarkGrey = ColorRGBA(0.25f,0.25f,0.25f);
	const ColorRGBA ColorRGBA::DarkBrown = ColorRGBA(0.43f,0.15f,0.0f);
	/*********************************************************************************/

	/*********************************************************************************/
	ColorRGBA::ColorRGBA()
		: m_Red(0.0f),
		  m_Green(0.0f),
		  m_Blue(0.0f),
		  m_Alpha(1.0f)
	{

	}
	/*********************************************************************************/
	ColorRGBA::ColorRGBA(float p_Red, float p_Green, float p_Blue,float p_Alpha)
		: m_Red(p_Red),
		  m_Green(p_Green),
		  m_Blue(p_Blue),
		  m_Alpha(p_Alpha)
	{
		clamp();
	}
	/*********************************************************************************/
	void ColorRGBA::setRed(float p_Red)
	{
		m_Red = p_Red;
		clamp();
	}
	/*********************************************************************************/
	void ColorRGBA::setGreen(float p_Green)
	{
		m_Green = p_Green;
		clamp();
	}
	/*********************************************************************************/
	void ColorRGBA::setBlue(float p_Blue)
	{
		m_Blue = p_Blue;
		clamp();
	}
	/*********************************************************************************/
	void ColorRGBA::setAlpha(float p_Alpha)
	{
		m_Alpha = p_Alpha;
		clamp();
	}
	/*********************************************************************************/
	void ColorRGBA::clamp()
	{
		if(m_Red > 1.0f) m_Red = 1.0f;
		if(m_Green > 1.0f) m_Green = 1.0f;
		if(m_Blue > 1.0f) m_Blue = 1.0f;
		if(m_Alpha > 1.0f) m_Alpha = 1.0f;

		if(m_Red < 0.0f) m_Red = 0.0f;
		if(m_Green < 0.0f) m_Green = 0.0f;
		if(m_Blue < 0.0f) m_Blue = 0.0f;
		if(m_Alpha < 0.0f) m_Alpha = 0.0f;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Rendering namespace