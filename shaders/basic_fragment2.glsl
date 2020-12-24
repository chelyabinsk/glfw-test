#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 newColor;

void main()
{
    //FragColor = mix(texture(texture1, TexCoord),texture(texture2, 5*vec2(1.0 - TexCoord.s,TexCoord.t)),1.0);
    FragColor = mix(texture(texture1, TexCoord),texture(texture2, TexCoord),1.0)*newColor;
}