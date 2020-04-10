#version 330 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 FragColor;  

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //arg: texture1, texture2, interpolation (the closer to 0 the texture 1 will be more prevailing, and vice versa)
   FragColor = mix(texture(texture1, texCoord),texture(texture2, texCoord), 0.2) * vec4(ourColor, 1.0);
}