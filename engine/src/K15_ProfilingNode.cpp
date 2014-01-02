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

#include "K15_PrecompiledHeader.h"

#include "K15_ProfilingNode.h"
#include "K15_ProfilingManager.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	AutoProfilingNode::AutoProfilingNode(ProfilingNode* p_Node)
		: m_Node(p_Node)
	{
		g_ProfileManager->startProfiling(p_Node);
	}
	/*********************************************************************************/
	AutoProfilingNode::~AutoProfilingNode()
	{
		g_ProfileManager->stopProfiling(m_Node);
	}
	/*********************************************************************************/

	/*********************************************************************************/
	ProfilingNode::ProfilingNode(const String& p_Name)
		: Name(p_Name),
		  EndTime(0.0),
		  Time(0.0),
		  Parent(0),
      CountChildren(0),
      ThreadID(g_CurrentThread::get_id().hash()),
      FrameIndex(g_Application->getFrameCount()),
      StartTime(g_Application->getTime())
	{
    memset(Children,0,sizeof(Children));
	}
	/*********************************************************************************/
	ProfilingNode::~ProfilingNode()
	{

	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace