in vec2 v_UV;
uniform sampler2D g_Texture_1;

void main(void)
{
	vec4 color = texture2D(g_Texture_1,v_UV);

	float distAlphaMask = color.a;

	//soft edges
	color.a *= smoothstep(1.0f, 4.0f, distAlphaMask);

	gl_FragColor = color;
}
