/**
 * @file K15_GpuResource.inl
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
 *
 * @section DESCRIPTION
 */

/*********************************************************************************/
bool GpuResource::isBound() const
{
	return m_IsBound;
}
/*********************************************************************************/
void GpuResource::setIsBound(bool p_Bound)
{
	m_IsBound = p_Bound;
}
/*********************************************************************************/
RendererBase* GpuResource::getRenderer() const
{
	return m_Renderer;
}
/*********************************************************************************/
void GpuResource::setRenderer(RendererBase* p_Renderer)
{
	m_Renderer = p_Renderer;
}
/*********************************************************************************/