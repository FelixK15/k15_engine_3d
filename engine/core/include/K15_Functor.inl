/**
 * @file K15_Functor.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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
template<class ReturnType>
Functor0<ReturnType>::Functor0()
{
	m_Function = 0;
}
/*********************************************************************************/
template<class ReturnType>
Functor0<ReturnType>::Functor0(FunctionType p_Function)
{
	m_Function = p_Function;
}
/*********************************************************************************/
template<class ReturnType>
Functor0<ReturnType>::~Functor0()
{

}
/*********************************************************************************/
template<class ReturnType>
void Functor0<ReturnType>::setFunction(FunctionType p_Function)
{
	m_Function = p_Function;
}
/*********************************************************************************/
template<class ReturnType>
bool Functor0<ReturnType>::isValid()
{
	return m_Function != 0;
}
/*********************************************************************************/
template<class ReturnType>
ReturnType Functor0<ReturnType>::operator()(void)
{
	K15_ASSERT(m_Function != 0,"Functor has no underlying function variable!");

	return m_Function();
}
/*********************************************************************************/
template<class ReturnType>
const Functor0<ReturnType>& Functor0<ReturnType>::operator =(const Functor0<ReturnType>& p_Other)
{
	m_Function = p_Other.m_Function;

	return *this;
}
/*********************************************************************************/



/*********************************************************************************/
template<class ReturnType,class ArgumentType>
Functor1<ReturnType,ArgumentType>::Functor1()
{
	m_Function = 0;
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType>
Functor1<ReturnType,ArgumentType>::Functor1(FunctionType p_Function)
{
	m_Function = p_Function;
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType>
Functor1<ReturnType,ArgumentType>::~Functor1()
{

}
/*********************************************************************************/
template<class ReturnType,class ArgumentType>
void Functor1<ReturnType,ArgumentType>::setFunction(FunctionType p_Function)
{
	m_Function = p_Function;
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType>
bool Functor1<ReturnType,ArgumentType>::isValid()
{
	return m_Function != 0;
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType>
ReturnType Functor1<ReturnType,ArgumentType>::operator()(ArgumentType p_Argument)
{
	K15_ASSERT(m_Function != 0,"Functor has no underlying function variable!");

	return m_Function(p_Argument);
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType>
const Functor1<ReturnType,ArgumentType>& Functor1<ReturnType,ArgumentType>::operator=(const Functor1<ReturnType,ArgumentType>& p_Other)
{
	m_Function = p_Other.m_Function;

	return *this;
}
/*********************************************************************************/

/*********************************************************************************/
template<class ReturnType,class ArgumentType,class ArgumentType2>
Functor2<ReturnType,ArgumentType,ArgumentType2>::Functor2()
{
	m_Function = 0;
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType,class ArgumentType2>
Functor2<ReturnType,ArgumentType,ArgumentType2>::Functor2(const Functor2<ReturnType,ArgumentType,ArgumentType2>& p_Other)
{
	m_Function = p_Other.m_Function;
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType,class ArgumentType2>
Functor2<ReturnType,ArgumentType,ArgumentType2>::Functor2(FunctionType p_Function)
{
	m_Function = p_Function;
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType,class ArgumentType2>
Functor2<ReturnType,ArgumentType,ArgumentType2>::~Functor2()
{

}
/*********************************************************************************/
template<class ReturnType,class ArgumentType,class ArgumentType2>
void Functor2<ReturnType,ArgumentType,ArgumentType2>::setFunction(FunctionType p_Function)
{
	m_Function = p_Function;
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType,class ArgumentType2>
bool Functor2<ReturnType,ArgumentType,ArgumentType2>::isValid()
{
	return m_Function != 0;
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType,class ArgumentType2>
ReturnType Functor2<ReturnType,ArgumentType,ArgumentType2>::operator()(ArgumentType p_Argument,ArgumentType2 p_Argument2)
{
	K15_ASSERT(m_Function != 0,"Functor has no underlying function variable!");

	return m_Function(p_Argument,p_Argument2);
}
/*********************************************************************************/
template<class ReturnType,class ArgumentType,class ArgumentType2>
const Functor2<ReturnType,ArgumentType,ArgumentType2>& Functor2<ReturnType,ArgumentType,ArgumentType2>::operator=(const Functor2<ReturnType,ArgumentType,ArgumentType2>& p_Other)
{
	m_Function = p_Other.m_Function;

	return *this;
}
/*********************************************************************************/