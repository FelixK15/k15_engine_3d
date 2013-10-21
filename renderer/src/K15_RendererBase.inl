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
inline RenderWindowBase* RendererBase::getRenderWindow() const
{
	return m_RenderWindow;
}
/*********************************************************************************/
inline RenderTarget* RendererBase::getRenderTarget() const
{
	return m_RenderTarget;
}
/*********************************************************************************/
inline Camera* RendererBase::getActiveCamera() const
{
	return m_ActiveCamera;
}
/*********************************************************************************/
inline Enum RendererBase::getCullingMode() const
{
	return m_CullingMode;
}
/*********************************************************************************/
inline Enum RendererBase::getDepthTestMode() const
{
	return m_DepthFunction;
}
/*********************************************************************************/
inline Enum RendererBase::getTopology() const
{
	return m_Topology;
}
/*********************************************************************************/
inline bool RendererBase::getDepthTestEnabled() const
{
	return m_DepthTestEnabled;
}
/*********************************************************************************/
inline bool RendererBase::getBackFaceCullingEnabled() const
{
	return m_BackFaceCullingEnabled;
}
/*********************************************************************************/
inline Enum RendererBase::getFrameBufferPixelFormat() const
{
	return m_FrameBufferFormat;
}
/*********************************************************************************/
inline Enum RendererBase::getDepthBufferFormat() const
{
	return m_DepthBufferFormat;
}
/*********************************************************************************/
inline Enum RendererBase::getStencilBufferFormat() const
{
	return m_StencilBufferFormat;
}
/*********************************************************************************/
inline bool RendererBase::getLightningEnabled() const
{
	return m_LightningEnabled;
}
/*********************************************************************************/
inline void RendererBase::bindGpuProgram(GpuProgram* p_GpuProgram)
{

}
/*********************************************************************************/
inline GpuProgram* RendererBase::getBoundGpuProgram(Enum p_GpuProgramType) const
{
	return m_GpuPrograms[p_GpuProgramType];
}
/*********************************************************************************/
inline bool RendererBase::isBoundGpuProgram(Enum p_GpuProgramType) const
{
	return m_GpuPrograms[p_GpuProgramType] != 0;
}
/*********************************************************************************/