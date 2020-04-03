#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
 
uniform float modifier;
void main()
{
    FragColor = vec4(ourColor.x, ourColor.y * modifier, ourColor.z, 1.0);
    
}