/**
 * @file K15_Renderer.cpp
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
 */

#include "K15_System.h"
#include "K15_Renderer.h"
#include "K15_UsefulMacros.h"
#include "K15_Geometry.h"
#include "K15_VertexLayout.h"
#include "K15_Camera.h"
#include "K15_ShaderVariable.h"

#include "K15_Light.h"

#include "D3DX11.h"
#include "DxErr.h"

#include "K15_LightShaderVariable.h"

#include "xnamath.h"

using namespace K15_Renderer;

DXGI_FORMAT arrFormat[] =
{
	DXGI_FORMAT_R8_SINT,
	DXGI_FORMAT_R8_UINT,
	DXGI_FORMAT_R8G8_SINT,
	DXGI_FORMAT_R8G8_UINT,
	DXGI_FORMAT_R8G8B8A8_SINT,
	DXGI_FORMAT_R8G8B8A8_UINT,
	
	DXGI_FORMAT_R32_SINT,
	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32G32_SINT,
	DXGI_FORMAT_R32G32_UINT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32B32_SINT,
	DXGI_FORMAT_R32G32B32_UINT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_SINT,
	DXGI_FORMAT_R32G32B32A32_UINT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_UNKNOWN
};

D3D11_PRIMITIVE_TOPOLOGY arrTopology[] = 
{
	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
};

D3D11_BIND_FLAG arrBindFlags[] = 
{
	D3D11_BIND_INDEX_BUFFER,
	D3D11_BIND_VERTEX_BUFFER,
	D3D11_BIND_DEPTH_STENCIL,
	D3D11_BIND_CONSTANT_BUFFER
};

D3D11_CPU_ACCESS_FLAG arrAccessFlags[] = 
{
	D3D11_CPU_ACCESS_WRITE,
	D3D11_CPU_ACCESS_READ
};

D3D11_USAGE arrUsage[] = 
{
	D3D11_USAGE_DEFAULT,
	D3D11_USAGE_DYNAMIC,
	D3D11_USAGE_IMMUTABLE
};

const char *arrSemanticName[] = 
{
	"POSITION",
	"NORMAL",
	"COLOR",
	"TEXCOORD",
};

const char *arrVSProfile[] = 
{
	"vs_1_0",
	"vs_2_0",
	"vs_3_0",
	"vs_4_0",
	"vs_5_0"
};

const char *arrPSProfile[] = 
{
	"ps_1_0",
	"ps_2_0",
	"ps_3_0",
	"ps_4_0",
	"ps_5_0"
};

ID3D10Blob *CompileShaderFromFile(const char *pFile,const char *pFunction,const char *pProfile,const char *&pErrorMessage)
{
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrorBuffer = NULL;

	U32 iFlags = 0;

#ifdef DEBUG
	iFlags |= D3D10_SHADER_DEBUG;
	iFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	if(D3DX11CompileFromFile(pFile,NULL,NULL,pFunction,pProfile,iFlags,0,NULL,&pCompiledShader,&pErrorBuffer,NULL) != S_OK){
		pErrorMessage = K15_NEW char[pErrorBuffer->GetBufferSize()];
		Memory::MemCopy((void*)pErrorMessage,pErrorBuffer->GetBufferPointer(),pErrorBuffer->GetBufferSize());

		RELEASE_COM(pErrorBuffer);
		return NULL;
	}

	return pCompiledShader;
}

Renderer::Renderer()
	: m_clBackBufferColor(1.f,1.f,1.f,1.f),
	  m_ptCurrentTopology(Geometry::PT_UNDEFINED),
	  m_pDevice(NULL),
	  m_pDeviceContext(NULL),
	  m_pTargetView(NULL),
	  m_pSwapChain(NULL),
	  m_pDepthStencilView(NULL),
	  m_pCamera(NULL),
	  m_pRasterizerState(NULL),
	  m_pBlendState(NULL),
	  m_iCurrentIndexBuffer(0xffffff),
	  m_iCurrentVertexBuffer(0xffffff),
	  m_iCurrentPixelShader(0xffffff),
	  m_iCurrentVertexShader(0xffffff),
	  m_iCurrentVertexLayout(0xffffff),
	  m_iLastDrawnObject(0xffffff)
{
	
}

Renderer::~Renderer()
{

}

bool Renderer::Initialize(U32 iPosX,U32 iPosY,U32 iWidth,U32 iHeight,bool bFullscreen)
{
	if(!_CreateDevice()){
		return false;
	}

	if(!_CreateSwapChain(iWidth,iHeight,bFullscreen)){
		return false;
	}

	if(!_CreateRenderView()){
		return false;
	}

	if(!_CreateDepthStencilView(iWidth,iHeight)){
		return false;
	}

	m_pCamera = K15_NEW Camera(iHeight,iWidth,1.f,1000.f,0.25f * 3.1415,true);
	_CreateViewport(iWidth,iHeight,iPosX,iPosY);
	_CreateRasterizerState();

	m_pLightShader = CreatePixelShader("lightShaders.hlsl","PS",PixelShader::SP_PS_4_0);

	LightShaderVariable *pVariable = K15_NEW LightShaderVariable();

	pVariable->SetUpdateFrequenzy(ShaderVariable::UF_MANUAL);

	pVariable->SetPackElementCount(53);

	m_pLightShader->AddShaderVariable("LightBuffer",pVariable);

	///////////////////////////////

	D3D11_BLEND_DESC blendDesc = {0};
	blendDesc.AlphaToCoverageEnable = TRUE;
	blendDesc.IndependentBlendEnable = FALSE;

	blendDesc.RenderTarget->BlendEnable = TRUE;
	blendDesc.RenderTarget->SrcBlend = D3D11_BLEND_SRC_COLOR;
	blendDesc.RenderTarget->DestBlend = D3D11_BLEND_INV_SRC_COLOR;
	blendDesc.RenderTarget->BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget->SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget->DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget->BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pDevice->CreateBlendState(&blendDesc,&m_pBlendState);
}

void Renderer::Shutdown()
{
	RELEASE_COM(m_pDepthStencilView);
	RELEASE_COM(m_pDevice);
	RELEASE_COM(m_pDeviceContext);
	RELEASE_COM(m_pSwapChain);
	RELEASE_COM(m_pTargetView);
	RELEASE_COM(m_pRasterizerState);

	K15_DELETE m_pCamera;
	m_pCamera = NULL;

	for(U32 i = 0;i < m_arrBuffers.Size();++i){
		RELEASE_COM(m_arrBuffers[i]);
	}

	for(U32 i = 0;i < m_arrShaders.Size();++i){
		RELEASE_COM(m_arrShaders[i]->ByteShader);
		RELEASE_COM(m_arrShaders[i]->Shader);
	}

	for(U32 i = 0;i < m_arrVertexLayouts.Size();++i){
		RELEASE_COM(m_arrVertexLayouts[i]);
	}
	m_arrBuffers.Clear();
	m_arrShaders.Clear();
	m_arrVertexLayouts.Clear();
}

void Renderer::BeginDraw()
{
	m_pDeviceContext->ClearRenderTargetView(m_pTargetView,(float*)m_clBackBufferColor.C);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,D3D11_CLEAR_DEPTH,1.f,0);
}

void Renderer::EndDraw()
{
	m_pSwapChain->Present(0,0);
}

void Renderer::Draw( Geometry *pGeometry )
{
	//@todo check each vertex in pGeometry if it fits into the projection plane.
	m_lNodesToDraw.PushBack(pGeometry);
}

void Renderer::Update()
{
	BeginDraw();
	for(ListNode<Geometry*> *pNode = m_lNodesToDraw.GetFirstElement();pNode;pNode = pNode->GetNext()){
		Geometry *pGeometry = pNode->GetElement();
		//m_pDeviceContext->OMSetBlendState(NULL,NULL,0xffffffff);
		//@todo implement multiple passes.
		//U32 iNumPasses = pGeometry->GetAmountPasses();
		U32 iNumPasses = 1;
		_SetBuffer(pGeometry->GetVertexBuffer());
		_SetBuffer(pGeometry->GetIndexBuffer());

		_SetShader(pGeometry->GetVertexShader());
		_SetShader(pGeometry->GetPixelShader());

		_UpdateShader(pGeometry->GetVertexShader(),pGeometry);
		_UpdateShader(pGeometry->GetPixelShader(),pGeometry);

		_SetVertexLayout(pGeometry->GetVertexShader()->GetVertexLayout());
		_SetPrimitiveTopology(pGeometry->GetPrimitiveTopology());

		U32 iAmountIndeces = pGeometry->GetIndexBuffer()->GetBufferSize() / pGeometry->GetIndexBuffer()->GetStride();

		_Draw(iAmountIndeces);
		
		m_iLastDrawnObject = pGeometry->GetUniqueID();
		m_pDeviceContext->OMSetBlendState(m_pBlendState,NULL,0xffffffff);
		//Light drawing.
		_SetShader(m_pLightShader);

		LightShaderVariable *pLightShaderVariable = (LightShaderVariable*)m_pLightShader->GetShaderVariable("LightBuffer");
		pLightShaderVariable->Update(pGeometry);
		ID3D11Buffer *pD3DBuffer = m_arrBuffers[pLightShaderVariable->GetBuffer()->GetHandle()];
		m_pDeviceContext->PSSetConstantBuffers(pLightShaderVariable->GetRegisterNumber(),1,&pD3DBuffer);
		m_pDeviceContext->OMSetBlendState(NULL,NULL,0xffffffff);
		

		_Draw(iAmountIndeces);
	}

	m_lNodesToDraw.Clear();
	EndDraw();

	m_iLastDrawnObject = 0xffffff;
}

// BufferPtr Renderer::CreateBuffer( void *pData,U32 iSizeData,U32 iSizeSingleElement,FormatType ftType,
// 								 Buffer::BufferUsage buUsage,Buffer::BufferAccess baAccess,Buffer::BufferBinding bbBinding )
// {
// 	assert(iSizeData > 0 && iSizeSingleElement > 0); //@todo error message console
// 
// 	ID3D11Buffer *pBuffer = NULL;
// 
// 	D3D11_BUFFER_DESC bufferDesc;
// 	bufferDesc.StructureByteStride = 0;// iSizeSingleElement;
// 	bufferDesc.ByteWidth = iSizeData;
// 	bufferDesc.Usage = arrUsage[buUsage];
// 
// 	if(baAccess == Buffer::BA_NO_ACCESS){
// 		bufferDesc.CPUAccessFlags = 0;
// 	}else{
// 		bufferDesc.CPUAccessFlags = arrAccessFlags[baAccess];
// 	}
// 
// 	bufferDesc.BindFlags = arrBindFlags[bbBinding];
// 	bufferDesc.MiscFlags = 0;
// 
// 	if(pData != NULL){
// 		D3D11_SUBRESOURCE_DATA resourceData;
// 		resourceData.pSysMem = pData;
// 		resourceData.SysMemPitch = 0;
// 		resourceData.SysMemSlicePitch = 0;
// 
// 		HR(m_pDevice->CreateBuffer(&bufferDesc,&resourceData,&pBuffer));
// 	}else{
// 		HR(m_pDevice->CreateBuffer(&bufferDesc,NULL,&pBuffer));
// 	}
// 
// 	m_arrBuffers.PushBack(pBuffer);
// 
// 	BufferPtr pReturnBuffer = K15_NEW Buffer(bbBinding,buUsage,baAccess,ftType,iSizeData,iSizeSingleElement);
// 	pReturnBuffer->m_iHandle = m_arrBuffers.Size() - 1;
// 	pReturnBuffer->SetData(pData);
// 	return pReturnBuffer;
// }

// void Renderer::UpdateBuffer( BufferPtr pBuffer,void *pData,bool bDiscardOldData )
// {
// 	assert(pBuffer->GetBufferAccess() == Buffer::BA_WRITE);
// 	assert(pBuffer->GetBufferUsage() == Buffer::BU_DYNAMIC);
// 	assert(pBuffer->GetHandle() < m_arrBuffers.Size());
// 	assert(m_arrBuffers[pBuffer->GetHandle()] != NULL);
// 
// 	D3D11_MAPPED_SUBRESOURCE subResource;
// 	D3D11_MAP mapType = D3D11_MAP_WRITE;
// 
// 	if(bDiscardOldData){
// 		mapType = D3D11_MAP_WRITE_DISCARD;
// 	}
// 
// 	ID3D11Buffer *pD3DBuffer = m_arrBuffers[pBuffer->GetHandle()];
// 	m_pDeviceContext->Map(pD3DBuffer,0,mapType,0,&subResource);
// 
// 	Memory::MemCopy(subResource.pData,pData,pBuffer->GetBufferSize());
// 	pBuffer->m_pData = pData;
// 	subResource.DepthPitch = pBuffer->GetBufferSize();
// 	subResource.RowPitch = pBuffer->GetBufferSize();
// 
// 	m_pDeviceContext->Unmap(pD3DBuffer,0);
// }

// VertexLayoutPtr Renderer::CreateVertexLayout( U8 iNumAttributes,VertexShaderPtr pVertexShader,... )
// {
// 	//@todo look for a VertexLayout that already matches the attributes given as parameter.
// 	va_list args;
// 	va_start(args,pVertexShader);
// 	VertexLayoutPtr pVertexLayout = K15_NEW VertexLayout(iNumAttributes,args);
// 	va_end(args);
// 
// 	U32 iOffset = 0;
// 	D3D11_INPUT_ELEMENT_DESC *pElements = K15_NEW D3D11_INPUT_ELEMENT_DESC[iNumAttributes];
// 	VertexLayout::SingleLayout *pLayout = pVertexLayout->GetLayout();
// 	for(U8 i = 0;i < iNumAttributes;++i){
// 		pElements[i].SemanticName = arrSemanticName[pLayout[i].Semantic];
// 		pElements[i].Format = arrFormat[pLayout[i].Format];
// 
// 		pElements[i].SemanticIndex = pLayout[i].Slot;
// 		pElements[i].AlignedByteOffset = iOffset;
// 		pElements[i].InstanceDataStepRate = 0;
// 		pElements[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
// 
// 		iOffset += pLayout[i].Size;
// 	}
// 
// 	ShaderResource *pShaderResource = m_arrShaders[pVertexShader->GetHandle()];
// 	ID3D10Blob *pCompiledShader = pShaderResource->ByteShader;
// 	ID3D11InputLayout *pInputLayout = NULL;
// 	HR(m_pDevice->CreateInputLayout(pElements,iNumAttributes,pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),&pInputLayout));
// 
// 	m_arrVertexLayouts.PushBack(pInputLayout);
// 	pVertexLayout->m_iHandle = m_arrVertexLayouts.Size() - 1;
// 	pVertexShader->m_pVertexLayout = pVertexLayout;
// 
// 	return pVertexLayout;
// }



bool Renderer::_CreateDevice()
{
	IDXGIFactory *pFactory = NULL;

	HR(CreateDXGIFactory(__uuidof(IDXGIFactory),(void**)&pFactory));
	IDXGIAdapter *arrAdapters[3];
	U32 iAdapterCounter = 0;

	while(pFactory->EnumAdapters(iAdapterCounter,&arrAdapters[iAdapterCounter]) != DXGI_ERROR_NOT_FOUND){
		++iAdapterCounter;
	}

	RELEASE_COM(pFactory);
	
	U32 iFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED;
	
	D3D_FEATURE_LEVEL flFeaturedLevel;

	HR(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,iFlags,NULL,0,D3D11_SDK_VERSION,&m_pDevice,&flFeaturedLevel,&m_pDeviceContext));


	//@todo : Log supported DX version
// 	if(flFeaturedLevel == D3D_FEATURE_LEVEL_9_1){
// 
// 	}else if(flFeaturedLevel == D3D_FEATURE_LEVEL_9_2){
// 
// 	}else if(flFeaturedLevel == D3D_FEATURE_LEVEL_9_3){
// 
// 	}else if(flFeaturedLevel == D3D_FEATURE_LEVEL_10_0){
// 
// 	}else if(flFeaturedLevel == D3D_FEATURE_LEVEL_10_1){
// 
// 	}else{
// 
// 	}

	return m_pDevice != NULL;
}

bool Renderer::_CreateSwapChain(U32 iWidth,U32 iHeight,bool bFullscreen)
{
	HWND pHwnd = (HWND)g_pSystem->QuerySystemObject(K15_EngineV2::System::SOD_WINDOWS_FORM_HANDLE);

	DXGI_SWAP_CHAIN_DESC scdSwapChainDesc;
	scdSwapChainDesc.BufferCount = 1;
	scdSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scdSwapChainDesc.Flags = 0;
	scdSwapChainDesc.OutputWindow = pHwnd;
	scdSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scdSwapChainDesc.Windowed = bFullscreen ? FALSE : TRUE;

	scdSwapChainDesc.SampleDesc.Count = 1;
	scdSwapChainDesc.SampleDesc.Quality = 0;

	scdSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scdSwapChainDesc.BufferDesc.Height = iHeight;
	scdSwapChainDesc.BufferDesc.Width = iWidth;
	scdSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	scdSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	scdSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scdSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	IDXGIFactory *pFactory = NULL;
	IDXGIDevice *pDevice = NULL;
	IDXGIAdapter *pAdapter = NULL;

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice),(void**)&pDevice);
	pDevice->GetParent(__uuidof(IDXGIAdapter),(void**)&pAdapter);
	pAdapter->GetParent(__uuidof(IDXGIFactory),(void**)&pFactory);

	pFactory->CreateSwapChain(pDevice,&scdSwapChainDesc,&m_pSwapChain);

	RELEASE_COM(pFactory);
	RELEASE_COM(pDevice);
	RELEASE_COM(pAdapter);

	return m_pSwapChain != NULL;
}

bool Renderer::_CreateRenderView()
{
	ID3D11Texture2D *pBackBuffer = NULL;

	HR(m_pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&pBackBuffer));
	HR(m_pDevice->CreateRenderTargetView(pBackBuffer,0,&m_pTargetView));
	
	RELEASE_COM(pBackBuffer);

	return m_pTargetView != NULL;
}

bool Renderer::_CreateDepthStencilView(U32 iWidth,U32 iHeight)
{
	D3D11_TEXTURE2D_DESC t2dDesc;
	
	t2dDesc.Height = iHeight;
	t2dDesc.Width = iWidth;
	t2dDesc.MipLevels = 1;
	t2dDesc.ArraySize = 1;
	t2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	t2dDesc.Usage = D3D11_USAGE_DEFAULT;
	t2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	t2dDesc.CPUAccessFlags = 0;
	t2dDesc.MiscFlags = 0;

	t2dDesc.SampleDesc.Count = 1;
	t2dDesc.SampleDesc.Quality = 0;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.StencilEnable = TRUE;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc =  D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	HR(m_pDevice->CreateTexture2D(&t2dDesc,NULL,&m_pDepthStencilBuffer));
	HR(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer,NULL,&m_pDepthStencilView));
	HR(m_pDevice->CreateDepthStencilState(&depthStencilDesc,&m_pDepthStencilState));
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState,1);
	RELEASE_COM(m_pDepthStencilBuffer);

	if(m_pDepthStencilView){
		m_pDeviceContext->OMSetRenderTargets(1,&m_pTargetView,m_pDepthStencilView);
	}
	
	return m_pDepthStencilView != NULL;
}

void Renderer::_CreateViewport( U32 iWidth,U32 iHeight,U32 iPosX,U32 iPosY)
{
	D3D11_VIEWPORT vwDesc;
	vwDesc.Height = iHeight;
	vwDesc.Width = iWidth;
	vwDesc.TopLeftX = iPosX;
	vwDesc.TopLeftY = iPosY;
	vwDesc.MaxDepth = 1.0;
	vwDesc.MinDepth = 0.0;

	m_pDeviceContext->RSSetViewports(1,&vwDesc);
}

void Renderer::_CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd,sizeof(rd));
	rd.CullMode = D3D11_CULL_NONE;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.FrontCounterClockwise = TRUE;
	rd.DepthClipEnable = TRUE;

	HR(m_pDevice->CreateRasterizerState(&rd,&m_pRasterizerState));
	m_pDeviceContext->RSSetState(m_pRasterizerState);
}

void Renderer::Release( BindablePtr pBindable )
{
	if(pBindable->IsInstanceOf(Buffer::TYPE)){
		_ReleaseBuffer(pBindable->GetHandle());
	}else if(pBindable->IsInstanceOf(VertexLayout::TYPE)){
		_ReleaseVertexLayout(pBindable->GetHandle());
	}else if(pBindable->IsInstanceOf(VertexShader::TYPE) || pBindable->IsInstanceOf(PixelShader::TYPE)){
		_ReleaseShader(pBindable->GetHandle());
	}
}

Shader *Renderer::_CreateShader( const char *pFile,const char *pFunction,const char *pProfile,bool bVertexShader )
{
	const char *pErrorMessage = NULL;
	ID3D10Blob *pCompiledShader = CompileShaderFromFile(pFile,pFunction,pProfile,pErrorMessage);

	if(!pCompiledShader){
		g_pSystem->ShowMessageBox(pErrorMessage,"Shader Error");
		assert(false);
	}

	ID3D11DeviceChild *pD3DShader = NULL;
	if(bVertexShader){
		ID3D11VertexShader *pVertexShader = NULL;
		HR(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&pVertexShader));
		pD3DShader = pVertexShader;
	}else{
		ID3D11PixelShader *pPixelShader = NULL;
		HR(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&pPixelShader));
		pD3DShader = pPixelShader;
	}

	Shader *pShader = NULL;

	if(pD3DShader != NULL){
		ShaderResource *pNewShaderResource = K15_NEW ShaderResource();
		pNewShaderResource->ByteShader = pCompiledShader;
		pNewShaderResource->Shader = pD3DShader;

		m_arrShaders.PushBack(pNewShaderResource);
		if(bVertexShader){
			pShader = K15_NEW VertexShader();
		}else{
			pShader = K15_NEW PixelShader();
		}
		
		pShader->m_iHandle = m_arrShaders.Size() - 1;
	}

	return pShader;
}

void Renderer::_SetBuffer( Buffer *pBuffer )
{
	BindableHandle iHandle = pBuffer->GetHandle();
	assert(iHandle < m_arrBuffers.Size());
	assert(m_arrBuffers[iHandle] != NULL);

	Buffer::BufferBinding bbBinding = pBuffer->GetBufferBinding();
	U32 iCurrentBuffer = 0;
	if(bbBinding == Buffer::BB_INDEX_BUFFER){
		iCurrentBuffer = m_iCurrentIndexBuffer;
	}else if(bbBinding == Buffer::BB_VERTEX_BUFFER){
		iCurrentBuffer = m_iCurrentVertexBuffer;
	}else{
		//Consant Buffer
	}

	if(iHandle != iCurrentBuffer){
		ID3D11Buffer *pD3DBuffer = m_arrBuffers[iHandle];
		U32 iStride = pBuffer->GetStride();
		U32 iOffset = 0;
		FormatType ftType = pBuffer->GetFormat();
		DXGI_FORMAT dxFormat = arrFormat[ftType];

		if(bbBinding == Buffer::BB_INDEX_BUFFER){
			m_pDeviceContext->IASetIndexBuffer(pD3DBuffer,dxFormat,0);
			m_iCurrentIndexBuffer = iHandle;
		}else if(bbBinding == Buffer::BB_VERTEX_BUFFER){
			m_pDeviceContext->IASetVertexBuffers(0,1,&pD3DBuffer,&iStride,&iOffset);
			m_iCurrentVertexBuffer = iHandle;
		}else{
			//Constant buffer
		}
	}
}

void Renderer::_SetShader( Shader *pShader )
{
	BindableHandle iHandle = pShader->GetHandle();
	assert(iHandle < m_arrShaders.Size());
	assert(m_arrShaders[iHandle] != NULL);

	bool bVertexShader = false;
	U32 iCurrentShader = m_iCurrentPixelShader;

	if(pShader->IsInstanceOf(VertexShader::TYPE)){
		bVertexShader = true;
		iCurrentShader = m_iCurrentVertexShader;
	}

	if(iHandle != iCurrentShader){
		ShaderResource *pShaderResource = m_arrShaders[iHandle];
		ID3D11DeviceChild *pD3DShader = pShaderResource->Shader;
		if(bVertexShader){
			ID3D11VertexShader *pVertexShader = (ID3D11VertexShader*) pD3DShader;
			m_pDeviceContext->VSSetShader(pVertexShader,NULL,0);
			m_iCurrentVertexShader = iHandle;
		}else{
			ID3D11PixelShader *pPixelShader = (ID3D11PixelShader*) pD3DShader;
			m_pDeviceContext->PSSetShader(pPixelShader,NULL,0);
			m_iCurrentPixelShader = iHandle;
		}
	}
}

void Renderer::_SetPrimitiveTopology( Geometry::PrimitiveTopolgy ptTopology)
{
	if(ptTopology != m_ptCurrentTopology){
		m_pDeviceContext->IASetPrimitiveTopology(arrTopology[ptTopology]);
		m_ptCurrentTopology = ptTopology;
	}
}

void Renderer::_SetVertexLayout( VertexLayout *pVertexLayout )
{
	BindableHandle iHandle = pVertexLayout->GetHandle();
	assert(iHandle < m_arrVertexLayouts.Size());
	assert(m_arrVertexLayouts[iHandle] != NULL);

	if(iHandle != m_iCurrentVertexLayout){
		ID3D11InputLayout *pInputLayout = m_arrVertexLayouts[iHandle];
		m_pDeviceContext->IASetInputLayout(pInputLayout);
		m_iCurrentVertexLayout = iHandle;
	}
}

void Renderer::_Draw( U32 iIndexCount )
{
	m_pDeviceContext->DrawIndexed(iIndexCount,0,0);
}

void Renderer::_UpdateShader( Shader *pShader,Geometry *pGeometry )
{
	const HashMap<const char*,ShaderVariable*> &hmShaderVariables = pShader->GetShaderVariables();
	for(U32 i = 0;i < hmShaderVariables.BucketSize();++i){
		for(HashItem<const char*,ShaderVariable*> *pItem = hmShaderVariables.GetBucketItem(i);pItem;pItem = pItem->GetNext()){
			ShaderVariable *pShaderVariable = pItem->GetValue();
			if(pShaderVariable->GetUpdateFrequenzy() == ShaderVariable::UF_PER_FRAME){
				pShaderVariable->Update(pGeometry);
				BufferPtr pBuffer = pShaderVariable->GetBuffer();
				ID3D11Buffer *pD3DBuffer = m_arrBuffers[pBuffer->GetHandle()];
				if(pShader->IsInstanceOf(VertexShader::TYPE)){
					m_pDeviceContext->VSSetConstantBuffers(pShaderVariable->GetRegisterNumber(),1,&pD3DBuffer);
				}else{
					m_pDeviceContext->PSSetConstantBuffers(pShaderVariable->GetRegisterNumber(),1,&pD3DBuffer);
				}
				
			}else if(pShaderVariable->GetUpdateFrequenzy() == ShaderVariable::UF_PER_OBJECT){
				if(pGeometry->GetUniqueID() != m_iLastDrawnObject){
					pShaderVariable->Update(pGeometry);
					BufferPtr pBuffer = pShaderVariable->GetBuffer();
					ID3D11Buffer *pD3DBuffer = m_arrBuffers[pBuffer->GetHandle()];
					if(pShader->IsInstanceOf(VertexShader::TYPE)){
						m_pDeviceContext->VSSetConstantBuffers(pShaderVariable->GetRegisterNumber(),1,&pD3DBuffer);
					}else{
						m_pDeviceContext->PSSetConstantBuffers(pShaderVariable->GetRegisterNumber(),1,&pD3DBuffer);
					}
				}
			}
		}
	}
}

void Renderer::_ReleaseBuffer( BindableHandle iHandle )
{
	assert(iHandle < m_arrBuffers.Size());
	assert(m_arrBuffers[iHandle] != NULL);

	RELEASE_COM(m_arrBuffers[iHandle]);
	m_arrBuffers[iHandle] = NULL;
}

void Renderer::_ReleaseVertexLayout( BindableHandle iHandle )
{
	assert(iHandle < m_arrVertexLayouts.Size());
	assert(m_arrVertexLayouts[iHandle] != NULL);

	RELEASE_COM(m_arrVertexLayouts[iHandle]);
	m_arrVertexLayouts[iHandle] = NULL;
}

void Renderer::_ReleaseShader( BindableHandle iHandle)
{
	assert(iHandle < m_arrShaders.Size());
	assert(m_arrShaders[iHandle] != NULL);

	ShaderResource *pShaderResource = m_arrShaders[iHandle];

	RELEASE_COM(pShaderResource->ByteShader);
	RELEASE_COM(pShaderResource->Shader);
	K15_DELETE pShaderResource;

	m_arrShaders[iHandle] = NULL;
}

Texture2DImpl *Renderer::CreateTexture2DImpl()
{
	return K15_NEW DX11_Texture2DImpl(m_pDevice);
}

ShaderImpl *Renderer::CreateShaderImpl()
{
	return K15_NEW DX11_ShaderImpl();
}
