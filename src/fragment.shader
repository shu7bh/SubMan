#version 330 core
in vec3 ourCol;
in vec3 objectPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform bool isDark;
uniform vec3 playerPos;
uniform bool isTexture;
uniform sampler2D texture;

void main()
{
    if(isDark && distance(playerPos,objectPos) >= 0.35f)
        FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    else if (isTexture)
        FragColor = texture2D(texture, TexCoord);
    else
        FragColor = vec4(ourCol, 1.0);
}
