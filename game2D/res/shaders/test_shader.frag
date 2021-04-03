#version 300 es
precision mediump float;
precision mediump int;

in vec2 v_norm;

uniform sampler2D s_texture;


layout(location = 0) out vec4 o_frag_color;

void main()
{
    o_frag_color = texture2D(s_texture, v_norm);
    if(o_frag_color.rgb == vec3(0.0,0.0,0.0))
	discard;
}
