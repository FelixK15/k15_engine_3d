/**
 * @file K15_Singleton.h
 * @author  Felix Klinge <f.klinge@k15games.de>
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
#ifndef _K15Engine_Core_Singleton_h_
#define _K15Engine_Core_Singleton_h_

namespace K15_Engine { namespace Core { 

	template<class T>
	class Singleton
	{
	public:
		static T* getInstance()
		{
			if(m_Instance == 0){
				m_Instance = ::new T();
			}

			return m_Instance; 
		}

	private:
		static T* m_Instance;

	protected:
		Singleton()
		{
			m_Instance = (T*)this;
		}
	};
}} //end of K15_Engine::Core namespace

template<class T> T* K15_Engine::Core::Singleton<T>::m_Instance = 0;

#endif //_K15Engine_Core_Singleton_h_