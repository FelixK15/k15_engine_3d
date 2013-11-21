/**
 * @file K15_RendererBase.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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
RenderWindowBase* RendererBase::getRenderWindow() const
{
	return m_RenderWindow;
}
/*********************************************************************************/
RenderTarget* RendererBase::getRenderTarget() const
{
	return m_RenderTarget;
}
/*********************************************************************************/
Camera* RendererBase::getActiveCamera() const
{
	return m_ActiveCamera;
}
/*********************************************************************************/
Enum RendererBase::getCullingMode() const
{
	return m_CullingMode;
}
/*********************************************************************************/
Enum RendererBase::getDepthTestMode() const
{
	return m_DepthTestFunction;
}
/*********************************************************************************/
Enum RendererBase::getTopology() const
{
	return m_Topology;
}
/*********************************************************************************/
bool RendererBase::getDepthTestEnabled() const
{
	return m_DepthTestEnabled;
}
/*********************************************************************************/
bool RendererBase::getBackFaceCullingEnabled() const
{
	return m_BackFaceCullingEnabled;
}
/*********************************************************************************/
Enum RendererBase::getFrameBufferPixelFormat() const
{
	return m_FrameBufferFormat;
}
/*********************************************************************************/
Enum RendererBase::getDepthBufferFormat() const
{
	return m_DepthBufferFormat;
}
/*********************************************************************************/
Enum RendererBase::getStencilBufferFormat() const
{
	return m_StencilBufferFormat;
}
/*********************************************************************************/
void RendererBase::setLightningEnabled(bool p_Enabled)
{
	m_LightningEnabled = p_Enabled;
}
/*********************************************************************************/
bool RendererBase::getLightningEnabled() const
{
	return m_LightningEnabled;
}
/*********************************************************************************/
GpuProgram* RendererBase::getBoundGpuProgram(Enum p_GpuProgramType) const
{
	return m_GpuPrograms[p_GpuProgramType];
}
/*********************************************************************************/
bool RendererBase::isBoundGpuProgram(Enum p_GpuProgramType) const
{
	return m_GpuPrograms[p_GpuProgramType] != 0;
}
/*********************************************************************************/