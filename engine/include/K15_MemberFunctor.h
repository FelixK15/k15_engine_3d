/**
 * @file K15_MemberFunctor.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_System_MemberFunctor_h_
#define _K15Engine_System_MemberFunctor_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace System {
	/*********************************************************************************/
	template<class ObjectType,class ReturnType>
	class K15_API_EXPORT MemberFunctor0
	{
	public:
		/*********************************************************************************/
		typedef ReturnType(ObjectType::*FunctionType)(void);
		/*********************************************************************************/
	public:
		MemberFunctor0();
		MemberFunctor0(FunctionType p_Function,ObjectType* p_Object);

		virtual ~MemberFunctor0();

		void setFunction(FunctionType p_Function);
		void setObject(ObjectType* p_Object);

		bool isValid();

		ReturnType operator()(void);
		const MemberFunctor0<class ObjectType,class ReturnType>& operator=(const MemberFunctor0<class ObjectType,class ReturnType>& p_Other);
	protected:
		FunctionType m_Function;
		ObjectType* m_Object;
	};//end of MemberFunctor0 class
	/*********************************************************************************/
	template<class ObjectType,class ReturnType,class ArgumentType>
	class K15_API_EXPORT MemberFunctor1
	{
	public:
		/*********************************************************************************/
		typedef ReturnType(ObjectType::*FunctionType)(ArgumentType);
		/*********************************************************************************/
	public:
		MemberFunctor0();
		MemberFunctor0(FunctionType p_Function,ObjectType* p_Object);

		virtual ~MemberFunctor0();

		void setFunction(FunctionType p_Function);
		void setObject(ObjectType* p_Object);

		bool isValid();

		ReturnType operator()(ArgumentType p_Argument);
		const MemberFunctor0<class ObjectType,class ReturnType>& operator=(const MemberFunctor0<class ObjectType,class ReturnType>& p_Other);
	protected:
		FunctionType m_Function;
		ObjectType* m_Object;
	};//end of MemberFunctor1 class
	/*********************************************************************************/
#include "K15_MemberFunctor.inl"
}}//end of K15_Engine::System namespace

#endif //_K15Engine_System_MemberFunctor_h_