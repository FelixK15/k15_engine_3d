/**
 * @file K15_RenderQueue.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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

#include "K15_RenderQueue.h"
#include "K15_RenderOperation.h"

namespace K15_Engine { namespace Rendering {
  /*********************************************************************************/
  RenderQueue::RenderQueue()
    : m_RenderOperationCount(0),
	m_SortMode(0),
    m_RenderOperations(),
	m_Dirty(true)
  {

  }
  /*********************************************************************************/
  RenderQueue::~RenderQueue()
  {

  }
  /*********************************************************************************/
  void RenderQueue::sort()
  {
	  if(m_Dirty)
	  {
		  if(m_SortMode == SP_SORT_BY_MATERIAL)
		  {
			  Sort(m_RenderOperations.begin(),m_RenderOperations.end(),[](RenderOperation* rop1, RenderOperation* rop2)->bool
			  {
				  return (size_t)rop1->material < (size_t)rop2->material;
			  });
		  }

		  m_Dirty = false;
	  }
  }
  /*********************************************************************************/
  void RenderQueue::clear()
  {
	  for(uint32 i = 0;i < m_RenderOperations.size();++i)
	  {
		  K15_DELETE m_RenderOperations.at(i);
	  }
	  m_RenderOperations.clear();
  }
  /*********************************************************************************/
}}//end of K15_Engine::Rendering namespace