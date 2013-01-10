/**
 * @file K15_GameTime.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/10
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
 */


const inline double GameTime::GetDeltaTime() const
{
	return m_dDeltaTime * m_dMulitplicator;
}

const inline double GameTime::GetRawDeltaTime() const
{
	return m_dDeltaTime;
}

const inline double GameTime::GetTimeMultiplicator() const
{
	return m_dMulitplicator;
}