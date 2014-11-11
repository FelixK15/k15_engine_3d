/**
 * @file K15_DebugRenderer.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/10
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

#ifndef _K15Engine_Renderer_DebugRenderer_h_
#define _K15Engine_Renderer_DebugRenderer_h_

#include "K15_Prerequisites.h"
#include "K15_Singleton.h"
#include "K15_AllocatedObject.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API DebugRenderer : public RenderingAllocatedObject,
									   public Singleton<DebugRenderer>
	{
	private:
		static uint32 const DEBUG_VERTEX_CACHE;

	public:
		DebugRenderer();
		~DebugRenderer();

		void drawPoint(const Vector3& p_Position, float p_Radius, const ColorRGBA& p_Color);
		void drawCross(const Vector3& p_Position, float p_Length, const ColorRGBA& p_Color);
		void drawLine(const Vector3& p_P1, const Vector3& p_P2, const ColorRGBA& p_Color);
		void drawArrow(const Vector3& p_Start, const Vector3& p_End, const ColorRGBA& p_Color);
		void drawCube(const Vector3& p_Min, const Vector3& p_Max, const ColorRGBA& p_Color);
		void drawAABB(const AABB& p_AABB, const ColorRGBA& p_Color);
		void drawNormals(GameObject* p_GameObject, float p_Length, const ColorRGBA& p_Color);
		void drawSphere(const Vector3& p_Center, float p_Radius, const ColorRGBA& p_Color);
		void drawAxis(GameObject* p_GameObject, float p_Length);

		INLINE RenderQueue* getRenderQueue() const;
		INLINE bool isActive() const;
		INLINE void setActive(bool p_Active);

		void clearRenderQueue();

	private:
		void _writeVertex(const Vector3& p_Position, const ColorRGBA& p_Color);
		void _createDebugMaterial();

	private:
		uint32 m_BufferOffset;
		RenderQueue* m_RenderQueue;
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
		VertexDeclaration* m_VertexDeclaration;
		Material* m_DebugMaterial;
		uint16 m_VertexCounter;
		bool m_Active;
	};
	#include "K15_DebugRenderer.inl"
}} // end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_DebugRenderer_h_