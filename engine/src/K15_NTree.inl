/**
 * @file K15_NTreeNode.h
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

template<class T,U8 N>
NTree<T,N>::NTree()
{
	m_pRoot = NULL;
	m_iSize = 0;
}

template<class T,U8 N>
NTree<T,N>::~NTree()
{
	Clear();
}

template<class T,U8 N>
NTreeNode<T,N> *NTree<T,N>::GetRoot() const
{
	return m_pRoot;
}

template<class T,U8 N>
bool NTree<T,N>::IsRoot(NTreeNode<T,N> *pNode) const
{
	return pNode == m_pRoot;
}

template<class T,U8 N>
U32 NTree<T,N>::Size() const
{
	return m_iSize;
}

template<class T,U8 N>
bool NTree<T,N>::IsLeaf(NTreeNode<T,N> *pNode) const
{
	bool bResult = true;

	if(m_pRoot){
		if(!IsRoot(pNode)){
			for(U8 i = 0;i < N;++i){
				if(pNode->Children[i] != NULL){
					bResult = false;
					break;
				}
			}
		}
	}

	return bResult;
}

template<class T,U8 N>
bool NTree<T,N>::Insert(const T& tValue,NTreeNode<T,N> *pParent)
{
	if(!pParent){
		if(m_pRoot){
			return false;
		}else{
			m_pRoot = _CreateNode(tValue,pParent);
		}
	}else{
		if(pParent->AmountChildren < N){
			_CreateNode(tValue,pParent);
		}else{
			return false;
		}
	}

	++m_iSize;

	return true;
}

template<class T,U8 N>
void NTree<T,N>::Remove(NTreeNode<T,N> *pNode)
{
	_ClearNode(pNode);
}

template<class T,U8 N>
void NTree<T,N>::Clear()
{
	_ClearNode(m_pRoot);
	m_pRoot = NULL;
	m_iSize = 0;
}

template<class T,U8 N>
NTreeNode<T,N> *NTree<T,N>::_CreateNode(const T& tValue,NTreeNode<T,N> *pParent)
{
	NTreeNode<T,N> *pNewNode = K15_NEW NTreeNode<T,N>();
	pNewNode->Value = K15_NEW T(tValue);
	pNewNode->Parent = pParent;
	pNewNode->AmountChildren = 0;

	if(pParent){
		pParent->Children[pParent->AmountChildren++] = pNewNode;
	}

	for(U8 i = 0;i < N;++i){
		pNewNode->Children[i] = NULL;
	}

	return pNewNode;
}

template<class T,U8 N>
void NTree<T,N>::_ClearNode(NTreeNode<T,N> *pNode)
{
	if(pNode == NULL){
		return;
	}

	for(U8 i = 0;i < pNode->AmountChildren;++i){
		_ClearNode(pNode->Children[i]);
	}

	K15_DELETE pNode->Value;
	K15_DELETE pNode;
	--m_iSize;
}