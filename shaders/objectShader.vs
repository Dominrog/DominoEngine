#version 460 core
layout(location = 0) in vec3 aPos;      // Position
layout(location = 1) in vec3 aNormal;   // Normal
layout(location = 2) in vec2 aTexCoord; // UV

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));           // Weltposition für Lichtberechnung
    Normal  = mat3(transpose(inverse(model))) * aNormal; // korrekte Normalen nach Skalierung
    TexCoord = aTexCoord;

    gl_Position = proj * view * vec4(FragPos, 1.0);
}