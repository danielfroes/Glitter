#version 330 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 FragColor;  

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float interpolationValue;

void main()
{
   
   //arg: texture1, texture2, interpolation (the closer to 0 the texture 1 will be more prevailing, and vice versa)
   FragColor = mix(texture(ourTexture1, texCoord),texture(ourTexture2, texCoord), interpolationValue) * vec4(ourColor, 1.0);
}