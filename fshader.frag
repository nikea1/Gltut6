#version 330

in vec2 myTexCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;

out vec4 FragColor;

void main(){
    FragColor = mix(texture(tex0, myTexCoord), texture(tex1, myTexCoord), 0.2);
    //FragColor = texture(tex1, myTexCoord);
}
