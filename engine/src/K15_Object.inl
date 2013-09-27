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

inline void Object::increaseReferences()
{
	++m_References;
}

inline void Object::decreaseReferences()
{
	--m_References;
}

inline bool Object::isInstanceOf(const Rtti& p_Type) const
{
	return TYPE.IsInstanceOf(p_Type);
}

inline bool Object::isInstanceOf(const Object* p_Object) const
{
	return p_Object && TYPE.IsInstanceOf(p_Object->getType());
}

inline bool Object::isDerivedFrom(const Rtti& p_Type) const
{
	return TYPE.IsDerivedFrom(p_Type);
}

inline bool Object::isDerivedFrom(const Object* p_Object) const
{
	return p_Object && TYPE.IsDerivedFrom(p_Object->getType());
}

inline uint32 Object::getReferenceCount() const
{
	return m_References;
}

inline const TypeName& Object::getName() const
{
	return m_Name;
}

inline void Object::setName(const TypeName& p_Name)
{
	m_Name = p_Name;
}