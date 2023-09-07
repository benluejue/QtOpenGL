#ifndef PUBLIC_H
#define PUBLIC_H

/**
* 存储其他文件要使用的公共路径
*/

//const char frag_path[] = "G:/c++/VSOpenGL/FirstBigProject/FirstBigProject/colors.frag";
//const char vert_path[] = "G:/c++/VSOpenGL/FirstBigProject/FirstBigProject/colors.vert";

extern float vertices[];
extern unsigned int vertices_num;

const char container2[] = "D:/OpenGL/VSOpenGL/FirstBigProject/FirstBigProject/texture/container2.png";
const char specular[] = "D:/OpenGL/VSOpenGL/FirstBigProject/FirstBigProject/texture/container2_specular.png.";
const char metal_path2[] = "D:/OpenGL/textures/metal.png";
// D:/OpenGL/OpenGL/resources/cube/base_cube.obj
// D:/OpenGL/OpenGL/resources/planet/planet.obj
// D:/OpenGL/OpenGL/resources/plane/plane.obj
// D:/OpenGL/OpenGL/resources/circle/untitled.obj
const char base_cube_objpath[] = "D:/OpenGL/OpenGL/resources/cube/base_cube.obj";
const char camera_cube_objpath[] = "D:/OpenGL/OpenGL/resources/camera_cube/camera_cube.obj";
const char base_plane_objpath[] = "D:/OpenGL/OpenGL/resources/plane/plane.obj";
const char nanosuit[] = "D:/OpenGL/VSOpenGL/FirstBigProject/FirstBigProject/3Dmodel/nanosuit/nanosuit.obj";


 enum SHAPETYPE {
     EASY_RENDER=1, // 简单渲染，没有任何漫反射之列的东西 例如地板
     FULL_RENDER,   // 较为复杂的渲染，如有漫反射和各种光照的模型
     CAMERA_RENDER
 };
 enum LIGHTTYPE
 {
     // 平行光 点光源 聚光
     DIRECTIONLIGHT=1,
     POINTLIGHT,
     SPOTLIGHT
 };

#endif




