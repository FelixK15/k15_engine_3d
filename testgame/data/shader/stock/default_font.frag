float4 ShadeFragment(	in float2 uv : TEXCOORD1, 
		     	in float3 color : COLOR1,
		     	in sampler2d sampler : DATA,
		     	in texture2d tex : DATA)
{
	float4 textAlpha = sampleTex2D(sampler, tex, uv);
	return float4(color, textAlpha.r);
}
