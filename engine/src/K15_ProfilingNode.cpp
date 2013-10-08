/**
 * @file K15_ProfileNode.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/10
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
#include "K15_ProfilingNode.h"
#include "K15_ProfilingManager.h"

namespace K15_Engine { namespace System { 

	ProfilingNode::ProfilingNode(const ProfilingName& p_Name,Enum	p_AppendMode)
		: m_Name(p_Name),
		  m_StartTime(0.0),
		  m_EndTime(0.0),
		  m_Time(0.0),
		  m_Parent(0),
		  m_AppendMode(p_AppendMode),
		  m_Children()
	{
		//Todo
//		m_StartTime = Application::getInstance()->getCurrentTime();
	}

	ProfilingNode::~ProfilingNode()
	{
// 		m_EndTime = Application::getInstance()->getCurrentTime();
// 		m_Time = m_EndTime - m_StartTime;
// 
// 		//add a reference of this to the profilemanager. the manager will create a copy.
// 		ProfilingManager::getInstance()->addProfileNode(*this);
	}

}}//end of K15_Engine::System namespace