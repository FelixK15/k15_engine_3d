/**
 * @file K15_Rtti.inl
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/08/09
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

/*********************************************************************************/
const TypeName& Rtti::getName() const
{
	return m_Name;
}
/*********************************************************************************/
bool Rtti::isInstanceOf(const Rtti &p_OtherType) const
{
	return &p_OtherType == this;
}
/*********************************************************************************/