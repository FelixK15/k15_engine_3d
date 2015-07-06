/*********************************************************************************/
uint8 K15_RenderCommandClearScreen(K15_RenderCommandBuffer* p_RenderCommandBuffer)
{
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_CLEAR_SCREEN);
	return K15_EndRenderCommand(p_RenderCommandBuffer);
}
/*********************************************************************************/
uint8 K15_RenderCommandCreateBuffer(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_CREATE_BUFFER);
	K15_AddRenderBufferHandleParameter(p_RenderCommandBuffer, p_RenderBufferHandlePtr);
	K15_AddRenderBufferDescParameter(p_RenderCommandBuffer, p_RenderBufferDesc);
	return K15_EndRenderCommand(p_RenderCommandBuffer);
}
/*********************************************************************************/
uint8 K15_RenderCommandUpdateBuffer(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBufferUpdateDesc* p_RenderBufferUpdateDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_UPDATE_BUFFER);
	K15_AddRenderBufferHandleParameter(p_RenderCommandBuffer, p_RenderBufferHandlePtr);
	K15_AddRenderBufferUpdateDescParameter(p_RenderCommandBuffer, p_RenderBufferUpdateDesc);
	return K15_EndRenderCommand(p_RenderCommandBuffer);
}
/*********************************************************************************/
uint8 K15_RenderCommandDeleteBuffer(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_DELETE_BUFFER);
	K15_AddRenderBufferHandleParameter(p_RenderCommandBuffer, p_RenderBufferHandlePtr);
	return K15_EndRenderCommand(p_RenderCommandBuffer);
}
/*********************************************************************************/
uint8 K15_RenderCommandCreateTexture(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTextureDesc* p_RenderTextureDesc, K15_RenderTextureHandle* p_RenderTextureHandlePtr)
{
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_CREATE_TEXTURE);
	K15_AddRenderTextureHandleParameter(p_RenderCommandBuffer, p_RenderTextureHandlePtr);
	K15_AddRenderTextureDescParameter(p_RenderCommandBuffer, p_RenderTextureDesc);
	return K15_EndRenderCommand(p_RenderCommandBuffer);
}
/*********************************************************************************/
uint8 K15_RenderCommandCreateMesh(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_MeshFormat* p_MeshFormat, K15_RenderMeshHandle* p_RenderMeshHandlePtr)
{
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_CREATE_MESH);
	K15_AddRenderMeshHandleParameter(p_RenderCommandBuffer, p_RenderMeshHandlePtr);
	K15_AddMeshFormatParameter(p_RenderCommandBuffer, p_MeshFormat);
	return K15_EndRenderCommand(p_RenderCommandBuffer);
}
/*********************************************************************************/
uint8 K15_RenderCommandDrawMesh(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderMeshHandle* p_RenderMeshHandlePtr, K15_Matrix4* p_WorldMatrix)
{
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_DRAW_MESH);
	K15_AddRenderMeshHandleParameter(p_RenderCommandBuffer, p_RenderMeshHandlePtr);
	K15_AddMatrix4Parameter(p_RenderCommandBuffer, p_WorldMatrix);
	return K15_EndRenderCommand(p_RenderCommandBuffer);
}
/*********************************************************************************/
uint8 K15_RenderCommandCreateCamera(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderCameraHandle* p_RenderCameraHandlePtr, K15_RenderCameraDesc* p_RenderCameraDesc)
{
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_CREATE_CAMERA);
	K15_AddRenderCameraHandleParameter(p_RenderCommandBuffer, p_RenderCameraHandlePtr);
	K15_AddRenderCameraDescParameter(p_RenderCommandBuffer, p_RenderCameraDesc);
	return K15_EndRenderCommand(p_RenderCommandBuffer);
}
/*********************************************************************************/
uint8 K15_RenderCommandBindCamera(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderCameraHandle* p_RenderCameraHandlePtr)
{
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_BIND_CAMERA);
	K15_AddRenderCameraHandleParameter(p_RenderCommandBuffer, p_RenderCameraHandlePtr);
	return K15_EndRenderCommand(p_RenderCommandBuffer);
}
/*********************************************************************************/
uint8 K15_RenderCommandSetGUITexture(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTextureDesc* p_RenderTextureDesc)
{
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_SET_GUI_TEXTURE);
	K15_AddRenderTextureDescParameter(p_RenderCommandBuffer, p_RenderTextureDesc);
	return K15_EndRenderCommand(p_RenderCommandBuffer);
}
/*********************************************************************************/
const char* K15_ConvertRenderCommandToString(K15_RenderCommand p_RenderCommand)
{
	const char* renderCommandString = 0;

	switch(p_RenderCommand)
	{
		case K15_RENDER_COMMAND_CLEAR_SCREEN:
		{
			renderCommandString = "Clear Screen";
			break;
		}

		case K15_RENDER_COMMAND_CREATE_BUFFER:
		{
			renderCommandString = "Create Buffer";
			break;
		}

		case K15_RENDER_COMMAND_UPDATE_BUFFER:
		{
			renderCommandString = "Update Buffer";
			break;
		}

		case K15_RENDER_COMMAND_DELETE_BUFFER:
		{
			renderCommandString = "Delete Buffer";
			break;
		}

		case K15_RENDER_COMMAND_CREATE_PROGRAM:
		{
			renderCommandString = "Create Program";
			break;
		}

		case K15_RENDER_COMMAND_DELETE_PROGRAM:
		{
			renderCommandString = "Delete Program";
			break;
		}

		case K15_RENDER_COMMAND_DELETE_RENDER_TARGET:
		{
			renderCommandString = "Delete Render Target";
			break;
		}

		case K15_RENDER_COMMAND_DRAW_FULLSCREEN_QUAD:
		{
			renderCommandString = "Draw Fullscreen Quad";
			break;
		}

		case K15_RENDER_COMMAND_CREATE_SAMPLER:
		{
			renderCommandString = "Create Sampler";
			break;
		}

		case K15_RENDER_COMMAND_CREATE_TEXTURE:
		{
			renderCommandString = "Create Texture";
			break;
		}

		case K15_RENDER_COMMAND_UPDATE_TEXTURE:
		{
			renderCommandString =  "Update Texture";
			break;
		}

		case K15_RENDER_COMMAND_DELETE_TEXTURE:
		{
			renderCommandString = "Delete Texture";
			break;
		}

		case K15_RENDER_COMMAND_BIND_RENDER_TARGET:
		{
			renderCommandString = "Bind Render Target";
			break;
		}

		case K15_RENDER_COMMAND_DELETE_SAMPLER:
		{
			renderCommandString = "Delete Sampler";
			break;
		}

		case K15_RENDER_COMMAND_DRAW_MESH:
		{
			renderCommandString = "Draw Mesh";
			break;
		}

		case K15_RENDER_COMMAND_CREATE_CAMERA:
		{
			renderCommandString = "Create Camera";
			break;
		}

		case K15_RENDER_COMMAND_UPDATE_CAMERA:
		{
			renderCommandString = "Update Camera";
			break;
		}

		default:
		{
			renderCommandString = "Unsupported Command";
			break;
		}
	}

	return renderCommandString;
}
/*********************************************************************************/