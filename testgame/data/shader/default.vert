float4 TransformVertex(in float2 pos : POSITION,
						in float2 uv : TEXCOORD1,
						out float2 out_uv : TEXCOORD1)
{
	out_uv = uv;
	return float4(pos, 1.0, 1.0);
}