/**
 * @file K15_RenderWindowBase.inl
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
float Resolution::getAspectRatio() const
{
	if(height == 0) return 0.0f;

	return (float)((float)width / (float)height);
}
/*********************************************************************************/

/*********************************************************************************/
const String& RenderWindow::getWindowTitle()
{
	return ms_WindowTitle;
}
/*********************************************************************************/
void RenderWindow::setResolution(uint32 width, uint32 height)
{
	Resolution r;
	r.width = width;
	r.height = height;
	setResolution(r);
}
/*********************************************************************************/
const Resolution& RenderWindow::getResolution()
{
	return ms_CurrentResolution;
}
/*********************************************************************************/
bool RenderWindow::isFullscreen()
{
	return ms_IsFullscreen;
}
/*********************************************************************************/
bool RenderWindow::getHasFocus()
{
	return ms_HasFocus;
}
/*********************************************************************************/
uint32 RenderWindow::getHeight()
{
	return ms_CurrentResolution.height;
}
/*********************************************************************************/
uint32 RenderWindow::getWidth()
{
	return ms_CurrentResolution.width;
}
/*********************************************************************************/
float RenderWindow::getAspectRatio()
{
	return ms_CurrentResolution.getAspectRatio();
}