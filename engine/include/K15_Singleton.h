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
#pragma once

#ifndef __K15_SINGLETON__
#define __K15_SINGLETON__

namespace K15_EngineV2
{
	template<class T>
	class Singleton
	{
	public:
		static T* GetInstance()
		{
			if(m_pInstance == NULL){
				m_pInstance = ::new T();
			}

			return m_pInstance; 
		}

	private:
		static T* m_pInstance;

	protected:
		Singleton()
		{
			m_pInstance = (T*)this;
		}
	};
}

template<class T> T* K15_EngineV2::Singleton<T>::m_pInstance = NULL;

#endif //__K15_SINGLETON__