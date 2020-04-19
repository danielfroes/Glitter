#version 330 core

in vec3 ourColor;

out vec4 FragColor;  

uniform float modifier;

void main()
{

   FragColor = vec4(ourColor * modifier, 1.0) ;
}