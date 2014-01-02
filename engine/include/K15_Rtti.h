/**
 * @file K15_Rtti.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/07
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

#ifndef _K15Engine_Core_RTTI_h_
#define _K15Engine_Core_RTTI_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#define K15_DECLARE_RTTI \
public:	\
  static const K15_Engine::Core::Rtti TYPE;	\
  virtual const K15_Engine::Core::Rtti &getType() const{ return TYPE; }	

#define K15_IMPLEMENT_RTTI_BASE(namespace,classtype,basetype)	\
  const K15_Engine::Core::Rtti classtype::TYPE(#namespace"."#classtype,&basetype::TYPE);

#define K15_IMPLEMENT_RTTI(namespace,classtype)	\
  const K15_Engine::Core::Rtti classtype::TYPE(#namespace"."#classtype,0);

#define K15_IMPLEMENT_RTTI_TEMPLATE_BASE(namespace,classtype,basetype) \
  template<> \
  const K15_Engine::Core::Rtti classtype::TYPE(#namespace"."#classtype,&basetype::TYPE);

#define K15_IMPLEMENT_RTTI_TEMPLATE(namespace,classtype) \
  template<> \
  const K15_Engine::Core::Rtti classtype::TYPE(#namespace"."#classtype,0);

namespace K15_Engine { namespace Core { 

	class K15_CORE_API Rtti
	{
	public:
		Rtti(const TypeName &p_Name,const Rtti *p_BaseType);
		~Rtti();

		INLINE const TypeName &getName() const;

		INLINE bool isInstanceOf(const Rtti &p_OtherType) const;
		bool isDerivedFrom(const Rtti &p_OtherType) const;

	private:
		TypeName m_Name;
		const Rtti *m_BaseType;
	};//end of Rtti class
  /*********************************************************************************/
	#include "K15_Rtti.inl"
  /*********************************************************************************/
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_RTTI_h_