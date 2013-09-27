/**
 * @file K15_MemberFunctor.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/26
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
template<class ObjectType,class ReturnType>
MemberFunctor0<ObjectType,ReturnType>::MemberFunctor0()
{
	m_Function = 0;
	m_Object = 0;
}
/*********************************************************************************/
template<class ObjectType,class ReturnType>
MemberFunctor0<ObjectType,ReturnType>::MemberFunctor0(FunctionType p_Function,ObjectType* p_Object)
{
	m_Function = p_Function;
	m_Object = p_Object;
}
/*********************************************************************************/
template<class ObjectType,class ReturnType>
MemberFunctor0<ObjectType,ReturnType>::~MemberFunctor0()
{

}
/*********************************************************************************/
template<class ObjectType,class ReturnType>
void MemberFunctor0<ObjectType,ReturnType>::setFunction(FunctionType p_Function)
{
	m_Function = p_Function;
}
/*********************************************************************************/
template<class ObjectType,class ReturnType>
void MemberFunctor0<ObjectType,ReturnType>::setObject(ObjectType* p_Object)
{
	m_Object = p_Object;
}
/*********************************************************************************/
template<class ObjectType,class ReturnType>
bool MemberFunctor0<ObjectType,ReturnType>::isValid()
{
	return m_Object != 0 && m_Function != 0;
}
/*********************************************************************************/
template<class ObjectType,class ReturnType>
ReturnType MemberFunctor0<ObjectType,ReturnType>::operator()()
{
	return m_Object->(*m_Function)();
}
/*********************************************************************************/
template<class ObjectType,class ReturnType>
const MemberFunctor0<ObjectType,ReturnType>& MemberFunctor0<ObjectType,ReturnType>::operator=(const MemberFunctor0<ObjectType,ReturnType>& p_Other)
{
	m_Object = p_Other.m_Object;
	m_Function = p_Other.m_Function;

	return *this;
}
/*********************************************************************************/



/*********************************************************************************/
template<class ObjectType,class ReturnType,class ArgumentType>
MemberFunctor1<ObjectType,ReturnType,ArgumentType>::MemberFunctor0()
{
	m_Function = 0;
	m_Object = 0;
}
/*********************************************************************************/
template<class ObjectType,class ReturnType,class ArgumentType>
MemberFunctor1<ObjectType,ReturnType,ArgumentType>::MemberFunctor0(FunctionType p_Function,ObjectType* p_Object)
{
	m_Function = p_Function;
	m_Object = p_Object;
}
/*********************************************************************************/
template<class ObjectType,class ReturnType,class ArgumentType>
MemberFunctor1<ObjectType,ReturnType,ArgumentType>::~MemberFunctor0()
{

}
/*********************************************************************************/
template<class ObjectType,class ReturnType,class ArgumentType>
void MemberFunctor1<ObjectType,ReturnType,ArgumentType>::setFunction(FunctionType p_Function)
{
	m_Function = p_Function;
}
/*********************************************************************************/
template<class ObjectType,class ReturnType,class ArgumentType>
void MemberFunctor1<ObjectType,ReturnType,ArgumentType>::setObject(ObjectType* p_Object)
{
	m_Object = p_Object;
}
/*********************************************************************************/
template<class ObjectType,class ReturnType,class ArgumentType>
bool MemberFunctor1<ObjectType,ReturnType,ArgumentType>::isValid()
{
	return m_Object != 0 && m_Function != 0;
}
/*********************************************************************************/
template<class ObjectType,class ReturnType,class ArgumentType>
ReturnType MemberFunctor1<ObjectType,ReturnType,ArgumentType>::operator()(ArgumentType p_Argument)
{
	return m_Object->(*m_Function)(p_Argument);
}
/*********************************************************************************/
template<class ObjectType,class ReturnType,class ArgumentType>
const MemberFunctor1<ObjectType,ReturnType,ArgumentType>& MemberFunctor1<ObjectType,ReturnType,ArgumentType>::operator=
	(const MemberFunctor1<ObjectType,ReturnType,ArgumentType>& p_Other)
{
	m_Object = p_Other.m_Object;
	m_Function = p_Other.m_Function;

	return *this;
}
/*********************************************************************************/