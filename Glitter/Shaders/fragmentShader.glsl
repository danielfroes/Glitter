#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
 
uniform float modifier;
void main()
{
    FragColor = modifier * vec4(ourColor, 1.0);
}