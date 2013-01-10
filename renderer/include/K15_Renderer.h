/**
 * @file K15_Renderer.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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

#ifndef __K15_RENDERER__
#define __K15_RENDERER__

#include "K15_StdInclude.h"
#include "K15_List.h"
#include "K15_DynamicArray.h"
#include "K15_SceneNode.h"
#include "K15_ColorRGBA.h"
#include "K15_Singleton.h"
#include "K15_Buffer.h"
#include "K15_VertexLayout.h"
#include "K15_Geometry.h"

#define g_pRenderer K15_Renderer::Renderer::GetInstance()

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11RasterizerState;
struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D11DeviceChild;
struct ID3D10Blob;
struct ID3D11BlendState;
struct ID3D11DepthStencilState;

namespace K15_Renderer
{
	typedef U32 CameraHandle;

	class Geometry;
	class Camera;
	class Light;

	class Texture2DImpl;
	class ShaderImpl;

	using namespace K15_EngineV2;

	class Renderer : public Singleton<Renderer>
	{
	public:
		Renderer();
		~Renderer();

		bool Initialize(U32 iPosX,U32 iPosY,U32 iWidth,U32 iHeight,bool bFullscreen);
		void Shutdown();

		void BeginDraw();
		void EndDraw();

		void Draw(Geometry *pGeometry);
		void Update();

// 		void AddLight(Light *pLight){
// 			m_arrLights.PushBack(pLight); //TEMPORARY
// 		}
// 
// 		DynamicArray<Light*> &GetLights(){
// 			return m_arrLights;
// 		}
		
		virtual Texture2DImpl *CreateTexture2DImpl();
		virtual ShaderImpl *CreateShaderImpl();

		Camera *GetCamera() //SetActiveCamera or something else.
		{
			return m_pCamera; //TEMPORARY
		}

	private:
		bool _CreateDevice();
		bool _CreateSwapChain(U32 iWidth,U32 iHeight,bool bFullscreen);
		bool _CreateRenderView();
		bool _CreateDepthStencilView(U32 iWidth,U32 iHeight);
		void _CreateViewport(U32 iWidth,U32 iHeight,U32 iPosX,U32 iPosY);
		void _CreateRasterizerState();

	protected:
		

	private:
		List<Geometry*> m_lNodesToDraw;

		Geometry::PrimitiveTopolgy m_ptCurrentTopology;

		ObjectID m_iLastDrawnObject;

		ColorRGBA m_clBackBufferColor;

		Camera *m_pCamera;
		ID3D11Device *m_pDevice;
		ID3D11DeviceContext *m_pDeviceContext;
		ID3D11RenderTargetView *m_pTargetView;
		ID3D11DepthStencilView *m_pDepthStencilView;
		ID3D11Texture2D *m_pDepthStencilBuffer;
		ID3D11RasterizerState *m_pRasterizerState;
		IDXGISwapChain *m_pSwapChain;
		ID3D11DepthStencilState *m_pDepthStencilState;

		ID3D11BlendState *m_pBlendState; // TEMPORARY;
	};
}

#endif //__K15_RENDERER__