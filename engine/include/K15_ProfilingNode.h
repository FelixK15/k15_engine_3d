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

#ifndef _K15Engine_Core_ProfileNode_h_
#define _K15Engine_Core_ProfileNode_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_ProfilingManager.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core { 

	class K15_API_EXPORT ProfilingNode/* : public ProfilingManagerAllocatedObject*/
	{
	public:
		typedef K15_List(ProfilingNode*) ProfilingNodeList;

	public:
		ProfilingNode(const ProfilingNode& p_Other);
		ProfilingNode(const ObjectName& p_Name);
		~ProfilingNode();

		ObjectName m_Name;

		ProfilingNode* m_Parent;
		ProfilingNodeList m_Children;

		double m_Time;
		double m_EndTime;
		double m_StartTime;
	};
}} //end of K15_Engine::System namespace

#endif //_K15Engine_Core_ProfileNode_h_