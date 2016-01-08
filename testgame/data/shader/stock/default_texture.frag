float4 ShadeFragment(in float2 uv : TEXCOORD1, 
		     in sampler2d sampler : DATA,
		     in texture2d tex : DATA)
{
	float4 c = sampleTex2D(sampler, tex, uv);
	return float4(c.r, c.g, c.b, 1.0f);
}

