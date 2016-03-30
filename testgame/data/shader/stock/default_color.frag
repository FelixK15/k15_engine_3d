#include "shaderlib.sfx"

float4 ShadeFragment(in float4 color : COLOR1)
{
	float3 gammaColor = K15_LinearToGamma(color.xyz);
	return float4(gammaColor, color.w);
}

