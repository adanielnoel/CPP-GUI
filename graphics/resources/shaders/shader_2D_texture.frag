#version 330 core

in vec2 _texCoords;
// Ouput data
layout(location = 0) out vec4 color;

// Values that stay constant for the whole mesh.
uniform vec4 color_vec;
uniform sampler2D theTexture;

void main(){
    color = texture(theTexture, _texCoords);
}