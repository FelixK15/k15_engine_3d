/**
 * @file K15_RendererPrerequisites.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_Prerequisites_h_
#define _K15Engine_Renderer_Prerequisites_h_

#include "K15_Prerequisites.h"

using namespace K15_Engine::Core;

namespace K15_Engine { namespace Rendering {
	class RenderTarget;
	class Texture;
  class TextureImplBase;
	class GpuBuffer;
  class GpuBufferImplBase;
	class Material;
	class RendererBase;
	class Mesh;
	class Camera;
	class SubMesh;
  class TextureSampler;
  class TextureSamplerImplBase;
	class GpuProgram;
  class GpuProgramImplBase;
  struct TextureCreationOptions;
}}// end of K15_Engine::Rendering namespace

typedef K15_Engine::Core::AllocatedObject<K15_Engine::Rendering::RendererBase> RendererAllocatedObject;

#endif //_K15Engine_Renderer_Prerequisites_h_