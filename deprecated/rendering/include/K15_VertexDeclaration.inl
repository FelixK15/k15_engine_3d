/**
 * @file K15_VertexDeclaration.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

/*********************************************************************************/
VertexDeclarationImplBase* VertexDeclaration::getImpl() const
{
  return m_Impl;
}
/*********************************************************************************/
void VertexDeclarationImplBase::setVertexDeclaration(VertexDeclaration* p_Declaration)
{
	m_Declaration = p_Declaration;
}
/*********************************************************************************/

/*********************************************************************************/
const ObjectName& VertexDeclaration::getDeclarationName() const
{
	return m_Declaration;
}
/*********************************************************************************/
VertexDeclaration* VertexDeclarationImplBase::getVertexDeclaration() const
{
  return m_Declaration;
}
/*********************************************************************************/

