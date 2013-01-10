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

inline void Object::IncreaseReferences()
{
	++m_iReferences;
}

inline void Object::DecreaseReferences()
{
	if(--m_iReferences == 0){
		delete this;
	}
}

inline bool Object::IsInstanceOf(const Rtti &rtType) const
{
	return GetType().IsInstanceOf(rtType);
}

inline bool Object::IsInstanceOf(const Object *pObject) const
{
	return pObject && TYPE.IsInstanceOf(pObject->GetType());
}

inline bool Object::IsDerivedFrom(const Rtti &rtType) const
{
	return TYPE.IsDerivedFrom(rtType);
}

inline bool Object::IsDerivedFrom(const Object *pObject) const
{
	return pObject && TYPE.IsDerivedFrom(pObject->GetType());
}

inline U32 Object::GetReferenceCount() const
{
	return m_iReferences;
}

inline ObjectID Object::GetUniqueID() const
{
	return m_iUniqueID;
}

inline const String& Object::GetName() const
{
	return m_sName;
}

inline void Object::SetName(const String& sName)
{
	m_sName = sName;
}

inline Object *Object::GetObjectByID(ObjectID iID)
{
	//return ms_hmObjectsInUse.Get(iID)->GetValue();
	return NULL;
}

inline const Rtti& Object::GetType() const
{
	return TYPE;
}

inline void Object::_SetID()
{
	m_iUniqueID = ms_iNextID++;
}

inline void Object::_RegisterObject()
{
	//ms_hmObjectsInUse.Insert(m_iUniqueID,this);
}

inline void Object::_UnregisterObject()
{
	//ms_hmObjectsInUse.Erase(m_iUniqueID);
}