K15_Vector4 TransformVertex(in K15_Vector2 pos : POSITION,
							in K15_Vector2 uv : TEXCOORD1,
							out K15_Vector2 out_uv : TEXCOORD1)
{
	out_uv = uv;
	return K15_Vector4(pos, 1.0, 1.0);
}