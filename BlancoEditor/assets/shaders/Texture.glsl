#type vertex

#version 330
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexSlot;
layout(location = 4) in float a_TilingFactor;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexSlot;
out float v_TilingFactor;
uniform mat4 u_ViewProjection;

void main(){
    gl_Position = u_ViewProjection * vec4(a_Position,1.0f);
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexSlot = a_TexSlot;
    v_TilingFactor = a_TilingFactor;
}

#type fragment

#version 330
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexSlot;
in float v_TilingFactor;

uniform sampler2D u_Texture[32];

void main(){
      color=texture(u_Texture[int(v_TexSlot)],v_TexCoord * v_TilingFactor) * v_Color;
}