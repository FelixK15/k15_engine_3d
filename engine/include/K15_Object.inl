/**
 * @file K15_Object.inl
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
void Object::increaseReferences()
{
	++m_References;
}
/*********************************************************************************/
void Object::decreaseReferences()
{
	--m_References;
}
/*********************************************************************************/
bool Object::isInstanceOf(const Rtti& p_Type) const
{
	return TYPE.isInstanceOf(p_Type);
}
/*********************************************************************************/
bool Object::isInstanceOf(const Object* p_Object) const
{
	return p_Object && TYPE.isInstanceOf(p_Object->getType());
}
/*********************************************************************************/
bool Object::isDerivedFrom(const Rtti& p_Type) const
{
	return TYPE.isDerivedFrom(p_Type);
}
/*********************************************************************************/
bool Object::isDerivedFrom(const Object* p_Object) const
{
	return p_Object && TYPE.isDerivedFrom(p_Object->getType());
}
/*********************************************************************************/
uint32 Object::getReferenceCount() const
{
	return m_References;
}
/*********************************************************************************/
const TypeName& Object::getName() const
{
	return m_Name;
}
/*********************************************************************************/
void Object::setName(const TypeName& p_Name)
{
	m_Name = p_Name;
}
/*********************************************************************************/