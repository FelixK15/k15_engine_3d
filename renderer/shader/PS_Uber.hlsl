#include "Shader_Structs.hlsl"
#include "Shader_Buffer.hlsl"

#ifdef HAS_TEXTURE
	#define TEXTURE_ARGUMENTS float2 UV : TEXCOORD
#endif

#ifdef HAS_COLOR
	#define COLOR_ARGUMENTS float4 Color : COLOR
#endif

#ifndef ARGUMENTS
	#ifdef TEXTURE_ARGUMENTS && COLOR_ARGUMENTS
		#define ARGUMENTS TEXTURE_ARGUMENTS,COLOR_ARGUMENTS
	#elif defined TEXTURE_ARGUMENTS
		#define ARGUMENTS TEXTURE_ARGUMENTS
	#elif defined COLOR_ARGUMENTS
		#define ARGUMENTS COLOR_ARGUMENTS
	#endif
#endif

float4 main(float3 PositionW : POSITION,
			   float3 NormalW : NORMAL) : SV_TARGET
{
	
}