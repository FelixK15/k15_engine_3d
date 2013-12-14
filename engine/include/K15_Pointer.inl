/**
 * @file K15_Pointer.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/07
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
template<class T>
Pointer<T>::Pointer()
{
	m_pObject = 0;
}
/*********************************************************************************/
template<class T>
Pointer<T>::Pointer(T *pObject)
{
	m_pObject = pObject;
	if(pObject != 0){
		pObject->increaseReferences();
	}
}
/*********************************************************************************/
template<class T>
Pointer<T>::Pointer(const Pointer<T> &pPointer)
{
	_getAndIncreasePointer(pPointer);
}
/*********************************************************************************/
template<class T>
Pointer<T>::~Pointer()
{
	m_pObject->decreaseReferences();
}
/*********************************************************************************/
template<class T>
const Pointer<T> &Pointer<T>::operator=(const Pointer<T> &pPointer)
{
	_getAndIncreasePointer(pPointer);
	return *this;
}
/*********************************************************************************/
template<class T>
const Pointer<T> &Pointer<T>::operator=(T* pObject)
{
	if(pObject != m_pObject){
		if(m_pObject){
			m_pObject->decreaseReferences();
			if(m_pObject->GetReferenceCount() == 0)
			{
				K15_SAFE_DELETE(m_pObject);
			}
		}
		m_pObject = pObject;
		if(pObject){
			pObject->increaseReferences();
		}
	}

	return *this;
}
/*********************************************************************************/
template<class T>
Pointer<T>::operator T*() const
{
	return m_pObject;
}
/*********************************************************************************/
template<class T>
T* Pointer<T>::operator->() const
{
	return m_pObject;
}
/*********************************************************************************/
template<class T>
T& Pointer<T>::operator*() const
{
	return *m_pObject;
}
/*********************************************************************************/
template<class T>
bool Pointer<T>::operator==(T* pObject)
{
	return m_pObject == pObject;
}
/*********************************************************************************/
template<class T>
bool Pointer<T>::operator==(const Pointer<T> &pPointer)
{
	return m_pObject == pPointer.m_pObject;
}
/*********************************************************************************/
template<class T>
bool Pointer<T>::operator!=(T* pObject)
{
	return m_pObject != pObject;
}
/*********************************************************************************/
template<class T>
bool Pointer<T>::operator!=(const Pointer<T> &pPointer)
{
	return m_pObject != pPointer.m_pObject;
}
/*********************************************************************************/
template<class T>
void Pointer<T>::_getAndIncreasePointer(const Pointer<T> &pPointer)
{
	if(m_pObject != 0)
	{
		m_pObject->decreaseReferences();
		if(m_pObject->getReferenceCount() == 0)
		{
			K15_SAFE_DELETE(m_pObject);
		}
	}
	if(pPointer.m_pObject != 0){
		m_pObject = pPointer.m_pObject;
		m_pObject->increaseReferences();
	}
}
/*********************************************************************************/