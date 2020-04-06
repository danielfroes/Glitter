#version 330 core
out vec4 FragColor;  
in vec3 ourColor;

uniform float modifier ;

void main()
{
    FragColor = vec4(modifier*ourColor, 1.0);
    
}