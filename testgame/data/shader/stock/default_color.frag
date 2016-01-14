#include "shaderlib.sfx"

float4 ShadeFragment(in float3 color : COLOR1)
{
	float3 gammaColor = K15_LinearToGamma(color);
	return float4(gammaColor, 1.0f);
}

