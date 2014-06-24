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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Core_Functor_h_
#define _K15Engine_Core_Functor_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	template<class ReturnType>
	class Functor0
	{
	public:
		/*********************************************************************************/
		typedef ReturnType(*FunctionType)(void);
		/*********************************************************************************/
	public:
		Functor0();
		Functor0(FunctionType p_Function);
	
		virtual ~Functor0();

		void setFunction(FunctionType p_Function);
		bool isValid();

		ReturnType operator()(void);
		const Functor0<ReturnType>& operator=(const Functor0<ReturnType>& p_Other);

	protected:
		FunctionType m_Function;
	}; //end of Functor0 class declaration
	/*********************************************************************************/
	template<class ReturnType,class ArgumentType>
	class Functor1
	{
	public:
		/*********************************************************************************/
		typedef ReturnType(*FunctionType)(ArgumentType);
		/*********************************************************************************/
	public:
		Functor1();
		Functor1(FunctionType p_Function);
		
		virtual ~Functor1();

		void setFunction(FunctionType p_Function);
		bool isValid();

		ReturnType operator()(ArgumentType p_Argument);
		const Functor1<ReturnType,ArgumentType>& operator=(const Functor1<ReturnType,ArgumentType>& p_Other);

	protected:
		FunctionType m_Function;
	}; //end of Functor1 class declaration
	/*********************************************************************************/
	template<class ReturnType,class ArgumentType,class ArgumentType2>
	class Functor2
	{
	public:
		/*********************************************************************************/
		typedef ReturnType(*FunctionType)(ArgumentType,ArgumentType2);
		/*********************************************************************************/
	public:
		Functor2();
		Functor2(FunctionType p_Function);
		Functor2(const Functor2<ReturnType,ArgumentType,ArgumentType2>& p_Other);

		virtual ~Functor2();

		void setFunction(FunctionType p_Function);
		bool isValid();

		ReturnType operator()(ArgumentType p_Argument,ArgumentType2 p_Argument2);
		const Functor2<ReturnType,ArgumentType,ArgumentType2>& operator=(const Functor2<ReturnType,ArgumentType,ArgumentType2>& p_Other);

	protected:
		FunctionType m_Function;
	}; //end of Functor2 class declaration
#include "K15_Functor.inl"
}}//end of namespace K15_Engine::Core

#endif //_K15Engine_Core_Functor_h_