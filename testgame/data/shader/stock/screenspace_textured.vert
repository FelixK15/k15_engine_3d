float4 TransformVertex(in float2 pos : POSITION,
		       in float2 uv : TEXCOORD1,
		       in float4x4 gui_proj : GUIPROJECTIONMATRIX,
		       out float2 out_uv : TEXCOORD1)
{
	float4 p = float4(pos, 0.f, 1.0f);
	float4 transformedPos = gui_proj * p;
	out_uv = uv;
	return transformedPos;
}

