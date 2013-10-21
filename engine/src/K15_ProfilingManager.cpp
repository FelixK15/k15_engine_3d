/**
 * @file K15_ProfileManager.h
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
 */

#include "K15_PrecompiledHeader.h"

#include "K15_ProfilingManager.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	ProfilingManager::ProfilingManager()
		: AllocatedObject(),
		  m_Root(0),
		  m_LastNode(0)
	{

	}
	/*********************************************************************************/
	ProfilingManager::~ProfilingManager()
	{
		
	}
	/*********************************************************************************/
	void ProfilingManager::addNode(ProfilingNode* p_Node)
	{
		//ProfilingNode* newNode = K15_NEW ProfilingNode(*p_Node);
		if(!m_Root)
		{
			m_Root = p_Node;
		}
		
		if(m_LastNode)
		{
			m_LastNode->m_Children.push_back(p_Node);
		}
		p_Node->m_Parent = m_LastNode;
		m_LastNode = p_Node;
	}
	/*********************************************************************************/
	void ProfilingManager::clear()
	{
		m_Root = 0;
		m_LastNode = 0;
	}
	/*********************************************************************************/
}}//end of K15_Engine::System namespace