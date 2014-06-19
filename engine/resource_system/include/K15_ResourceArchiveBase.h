/**
 * @file K15_ResourceArchiveBase.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
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

#ifndef _K15Engine_Core_ResourceArchiveBase_h_
#define _K15Engine_Core_ResourceArchiveBase_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_Object.h"
#endif //K15_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Core { 
	class K15_CORE_API ResourceArchiveBase : public Object
	{
		K15_DECLARE_RTTI;

	public:
		bool open();
		bool close();

		virtual bool getResource(const String&, RawData*) = 0;
		virtual bool hasResource(const String&) = 0;

		INLINE bool isOpen() const;
		INLINE const String& getFileName() const;
		INLINE const String& getError() const;

	protected:
		ResourceArchiveBase(const String& p_FileName);
		virtual ~ResourceArchiveBase();
		
		virtual bool _open() = 0;
		virtual bool _close() = 0;

		INLINE void setError(const String& p_Error);

	private:
		String m_Error;
		String m_FileName;
		bool m_isOpen;
	}; //end of ResourceArchiveBase class declaration
#	include "K15_ResourceArchiveBase.inl"
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ResourceArchiveBase_h_