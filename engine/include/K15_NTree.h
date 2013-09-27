/**
 * @file K15_NTree.h
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
#pragma once

#ifndef __K15_NTREE__
#define __K15_NTREE__

#include "K15_StdInclude.h"

#include "K15_NTreeNode.h"

namespace K15_EngineV2
{
	template<class T,uint8 N>
	class NTree
	{

	public:
		NTree();
		~NTree();

		NTreeNode<T,N> *GetRoot() const;

		bool IsRoot(NTreeNode<T,N> *pNode) const;
		bool IsLeaf(NTreeNode<T,N> *pNode) const;

		uint32 Size() const;

		bool Insert(const T& tValue,NTreeNode<T,N> *pParent);
		void Remove(NTreeNode<T,N> *pNode);

		void Clear();

	private:
		NTreeNode<T,N> *_CreateNode(const T& tValue,NTreeNode<T,N> *pParent);
		void _ClearNode(NTreeNode<T,N> *pNode);

	private:
		NTreeNode<T,N> *m_pRoot;
		uint32 m_iSize;
	};

	#include "../../engine/src/K15_NTree.inl"
}

#endif //__K15_NTREE__