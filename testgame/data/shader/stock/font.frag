float4 ShadeFragment(in float2 uv : TEXCOORD1, 
						  in sampler2d sampler : DATA,
						  in texture2d tex : DATA)
{
	float4 color = sampleTex2D(sampler, tex, uv);
	return float4(0.f, 0.f, 0.f, color.r);
}