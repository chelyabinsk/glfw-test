#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoord),texture(texture2, 5*vec2(1.0 - TexCoord.s,TexCoord.t)),0.5)*ourColor;//*ourColor;
    //FragColor = texture(texture1, TexCoord)*inColor;//*vec4(1.0,0.5,0.0,1.0);
    //FragColor = texture(texture2, TexCoord)*ourColor;
    //FragColor = ourColor;
}