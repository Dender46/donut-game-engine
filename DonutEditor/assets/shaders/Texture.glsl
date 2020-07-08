#type vertex
#version 330 core

layout(location = 0) in vec3	a_Position;
layout(location = 1) in vec4	a_Color;
layout(location = 2) in vec2	a_TexCoords;
layout(location = 3) in float	a_TexIndex;
layout(location = 4) in float	a_TilingAmount;

out vec4	v_Color;
out vec2	v_TexCoords;
out float	v_TexIndex;
out float	v_TilingAmount;

uniform mat4 u_ViewProjection;

void main()
{
	v_Color = a_Color;
	v_TexCoords = a_TexCoords;
	v_TexIndex = a_TexIndex;
	v_TilingAmount = a_TilingAmount;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}



#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4		v_Color;
in vec2		v_TexCoords;
in float	v_TexIndex;
in float	v_TilingAmount;

uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[int(v_TexIndex)], v_TexCoords * v_TilingAmount) * v_Color;
}