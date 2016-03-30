float4 TransformVertex(in float2 pos : POSITION,
		       in float4 color : COLOR1,
		       in float4x4 gui_proj : PROJECTIONMATRIX,
		       out float4 out_color : COLOR1)
{
	float4 p = float4(pos, 1.f, 1.0f);
	float4 transformedPos = gui_proj * p;
	out_color = color;
	return transformedPos;
}

