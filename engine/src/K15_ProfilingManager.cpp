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
		clear();
	}
	/*********************************************************************************/
	void ProfilingManager::startProfiling(ProfilingNode* p_Node)
	{
		p_Node->Parent = m_LastNode;

		if(!m_Root)
		{
			m_Root = p_Node;
		}
		else
		{
			m_LastNode->Children[m_LastNode->CountChildren++] = p_Node;
		}
		
		m_LastNode = p_Node;
	}
	/*********************************************************************************/
	void ProfilingManager::stopProfiling(ProfilingNode* p_Node)
	{
		p_Node->EndTime = g_Application->getTime();
		p_Node->Time = p_Node->EndTime - p_Node->StartTime;

		m_LastNode = p_Node->Parent;

		if(p_Node == m_Root)
		{
			m_Nodes.push_back(p_Node);
			m_Root = 0;
		}
	}
	/*********************************************************************************/
	void ProfilingManager::clear()
	{
		m_Root = 0;
		m_LastNode = 0;

		for(ProfilingNodeList::iterator iter = m_Nodes.begin();iter != m_Nodes.end();++iter)
		{
			_eraseNode_r((*iter));
		}

		m_Nodes.clear();
	}
	/*********************************************************************************/
	void ProfilingManager::eraseProfilingForFrame(uint32 p_FrameIndex)
	{
		if(m_Nodes.size() > 0)
		{
			ProfilingNode* node = (*m_Nodes.begin());
			K15_ASSERT(node->FrameIndex == p_FrameIndex,"Trying to erase ProflingData outside of the GameLoop.");

			_eraseNode_r(node);

			m_Nodes.erase(m_Nodes.begin());
		}
	}
	/*********************************************************************************/
	void ProfilingManager::_eraseNode_r(ProfilingNode* p_Node)
	{
		if(p_Node->CountChildren > 0)
		{
			for(int i = 0;i < p_Node->CountChildren;++i)
			{
				_eraseNode_r(p_Node->Children[i]);
			}
		}

		K15_DELETE p_Node;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace