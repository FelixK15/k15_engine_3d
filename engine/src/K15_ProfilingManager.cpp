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

namespace K15_Engine { namespace System { 
	/*********************************************************************************/
	ProfilingManager::ProfilingManager()
		: m_Root(0),
		  m_LastNode(0),
		  m_Nodes()
	{

	}
	/*********************************************************************************/
	ProfilingManager::~ProfilingManager()
	{
		m_Nodes.clear();
	}
	/*********************************************************************************/
	void ProfilingManager::addProfileNode(ProfilingNode p_Node)
	{
		m_Nodes.push_back(p_Node);
		ProfilingNode* lastNode = &m_Nodes[m_Nodes.size() - 1];
		if(!m_Root)
		{
			m_Root = &m_Nodes[0];
			m_LastNode = m_Root;
		}
		else
		{
// 			if(p_Node.m_AppendMode == AM_APPEND_LAST_NODE)
// 			{
// 				m_LastNode->m_Children.push_back(lastNode);
// 			}
// 			else if(p_Node.m_AppendMode == AM_NEW_BRANCH)
// 			{
// 				m_LastNode = lastNode;
// 			}
		}
	}
	/*********************************************************************************/
	void ProfilingManager::clear()
	{
		m_Nodes.clear();
		m_Root = 0;
		m_LastNode = 0;
	}
	/*********************************************************************************/
}}//end of K15_Engine::System namespace