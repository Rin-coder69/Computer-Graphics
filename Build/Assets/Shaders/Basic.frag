#version 460 core
in vec3 ocolor;
out vec4 f_color;

void main(){
//f_color = vec4(1,1,0,1);
f_color = vec4(ocolor,1);
}