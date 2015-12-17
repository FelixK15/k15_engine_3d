float4 ShadeFragment(in float2 uv : TEXCOORD1, 
					in sampler2d DiffuseSampler : DATA,
					in texture2d DiffuseTexture : DATA)
{
	float4 c = sampleTex2D(DiffuseSampler, DiffuseTexture, uv);
	return float4(c.r, c.g, c.b, 1.f);
}