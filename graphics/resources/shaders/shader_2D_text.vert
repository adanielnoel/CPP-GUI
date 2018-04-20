#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection_matrix;
uniform float dpi_scaling;

void main()
{
    // Projection matrix adapted for pixel scaling
    // (e.g. a pixel scaling of 2 means that for every standard pixel there are 2 screen pixels)
    mat4 m = projection_matrix;
    m[0] /= dpi_scaling;
    m[1] /= dpi_scaling;
    gl_Position = m * vec4(vertex.x * dpi_scaling, vertex.y * dpi_scaling, 0.0, 1.0);
    TexCoords = vertex.zw;
}