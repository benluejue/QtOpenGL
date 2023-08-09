#version 330 core
layout (location = 0) in vec3 aPos;
 
uniform mat4 view;
uniform mat4 projection;
 
out vec3 TexCoords;
 
void main(){
	TexCoords = aPos;
	vec4 pos = projection * view * vec4(aPos, 1.0f);
	//	这个pos.xyww特别魔性，一般默认传递值，都是pos.xyzw。
	//  而分量（z/w）的值即代表片段的深度值。所以当改将z改为w时，w/w=1.0f，则天空盒所有的片段深度皆为1.0f。
	gl_Position = pos.xyww;
}
