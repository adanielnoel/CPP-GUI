#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position_vec;

// Values that stay constant for the whole mesh.
uniform mat4 projection_matrix;
uniform mat4 transform_matrix;
uniform float z_coordinate;
uniform float dpi_scaling;

void main(){
    // Projection matrix adapted for pixel scaling
    // (e.g. a pixel scaling of 2 means that for every standard pixel there are 2 screen pixels)
    mat4 m = projection_matrix;
    m[0] /= dpi_scaling;
    m[1] /= dpi_scaling;
    gl_Position =  m * vec4(position_vec.x * dpi_scaling, position_vec.y * dpi_scaling, z_coordinate,1);
}