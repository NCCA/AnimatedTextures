#version 330 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;
uniform mat4 VP;
uniform vec3 camerapos;
uniform int time;

out vec2 vertUV;
flat out float texID;
flat in float whichTexture[];
flat in float frameOffset[];
void main()
{
    float bbWidth=0.5;
    float bbHeight=1.0;
    float ctime=time+frameOffset[0];
    texID=whichTexture[0];
    float spriteOffset=0.1;
    vec3 pos = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(camerapos - pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);
    right*=bbWidth;
    up *= bbHeight;
    pos -= (right * 0.5);
    pos.z=gl_in[0].gl_Position.z;
    gl_Position = VP * vec4(pos, 1.0);
    vertUV = vec2(ctime*spriteOffset, 0.0);
    EmitVertex();

    pos.y += 1.0;
    gl_Position = VP * vec4(pos, 1.0);
    vertUV = vec2(ctime*spriteOffset, 1.0);
    EmitVertex();

    pos.y -= 1.0;
    pos += right;
    gl_Position = VP * vec4(pos, 1.0);
    vertUV = vec2((ctime+1)*spriteOffset, 0.0);
    EmitVertex();

    pos.y += 1.0;
    gl_Position = VP * vec4(pos, 1.0);
    vertUV = vec2((ctime+1)*spriteOffset, 1.0);
    EmitVertex();

    EndPrimitive();
}
