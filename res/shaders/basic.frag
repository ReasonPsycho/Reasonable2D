#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 lightColor;

void main()
{
    vec4 texColor = texture(ourTexture , TexCoord)* vec4(lightColor,1.0f);
    if(texColor.a < 0.2)
    discard;
    FragColor = texColor;   
}