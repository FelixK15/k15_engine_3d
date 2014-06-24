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
TextureSampler* RendererBase::getBoundSampler(Enum p_Slot) const
{
	return m_BoundSamplers[p_Slot];
}
/*********************************************************************************/
Texture* RendererBase::getBoundTexture(Enum p_Slot) const
{
	return m_BoundTextures[p_Slot];
}
/*********************************************************************************/
RenderTarget* RendererBase::getRenderTarget() const
{
	return m_RenderTarget;
}
/*********************************************************************************/
CameraComponent* RendererBase::getActiveCamera() const
{
	return m_ActiveCamera;
}
/*********************************************************************************/
Enum RendererBase::getCullingMode() const
{
	return m_CullingMode;
}
/*********************************************************************************/
Enum RendererBase::getTopology() const
{
	return m_Topology;
}
/*********************************************************************************/
Enum RendererBase::getFillMode() const
{
  return m_FillMode;
}
/*********************************************************************************/
bool RendererBase::getBackFaceCullingEnabled() const
{
	return m_BackFaceCullingEnabled;
}
/*********************************************************************************/
bool RendererBase::errorOccured() const
{
	return !m_LastError.empty();
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
AlphaState& RendererBase::getAlphaState()
{
	return m_AlphaState;
}
/*********************************************************************************/
DepthState& RendererBase::getDepthState()
{
  return m_DepthState;
}
/*********************************************************************************/
ColorRGBA& RendererBase::getClearColor()
{
	return m_ClearColor;
}
/*********************************************************************************/
GpuProgram* RendererBase::getBoundGpuProgram(Enum p_GpuProgramType) const
{
	return m_GpuPrograms[p_GpuProgramType];
}
/*********************************************************************************/
GpuProgramBatch* RendererBase::getBoundGpuProgramBatch() const
{
	return m_GpuProgramBatch;
}
/*********************************************************************************/
VertexDeclaration* RendererBase::getVertexDeclaration() const
{
  return m_VertexDeclaration;
}
/*********************************************************************************/
bool RendererBase::isBoundGpuProgram(Enum p_GpuProgramType) const
{
	return m_GpuPrograms[p_GpuProgramType] != 0;
}
/*********************************************************************************/
const String& RendererBase::getLastError()
{
	static String errString;
	errString = m_LastError;

	m_LastError.clear();

	return errString;
}
/*********************************************************************************/
void RendererBase::setLastError(const String& p_String)
{
	m_LastError = p_String;
}
/*********************************************************************************/
bool RendererBase::isInitialized() const
{
	return m_Initialized;
}
/*********************************************************************************/