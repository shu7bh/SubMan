#version 330 core
in vec3 ourCol;
out vec4 FragColor;
in vec3 objectPos;

uniform bool isDark;
uniform vec3 playerPos;


void main()
{
    if(isDark && distance(playerPos,objectPos) >= 0.35f)
        FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    else
        FragColor = vec4(ourCol, 1.0);
}
