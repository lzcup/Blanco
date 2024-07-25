// Basic Circle Shader

#type vertex

#version 450
layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;

out vec3 v_Position;
out vec4 v_Color;
out float v_Thickness;
out float v_Fade;
out flat int v_EntityID;
uniform mat4 u_ViewProjection;

void main(){
    gl_Position = u_ViewProjection * vec4(a_WorldPosition,1.0f);
	v_Position = a_LocalPosition;
    v_Color = a_Color;
    v_Thickness = a_Thickness;
    v_Fade = a_Fade;
    v_EntityID = a_EntityID;
}

#type fragment

#version 450
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

in vec3 v_Position;
in vec4 v_Color;
in float v_Thickness;
in float v_Fade;
in flat int v_EntityID;

void main()
{
    float distance = 1.0-length(v_Position);
  
    float circle = smoothstep(0.0,v_Fade,distance);
    circle *= smoothstep(v_Thickness+v_Fade,v_Thickness,distance);

    if(circle == 0.0)
       discard;

    o_Color = v_Color;
    o_Color.a *= circle;

	o_EntityID = v_EntityID;
}