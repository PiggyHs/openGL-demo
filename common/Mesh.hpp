//
//  ObjLoader.hpp
//  learnOpenGl
//  实现功能：
//      1、导入、解析OBJ类型模型；
//      2、返回解析出的模型顶点信息、法向量信息、纹理坐标信息；
//  使用方法：
//      1、引用该头文件；
//      2、new 一个mesh类；
//      3、调用InitMesh函数进行初始化
//  Created by heshuai on 2018/11/6.
//  Copyright © 2018年 heshuai. All rights reserved.
//

#ifndef ObjLoader_hpp
#define ObjLoader_hpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 表示一个顶点属性
struct Vertex
{

    std::vector<GLfloat> position;  // 顶点位置
    std::vector<GLfloat> normal;  // 法向量
    std::vector<GLfloat> texCoords; // 纹理坐标
};

struct VertexCombineIndex
{
    GLuint posIndex;  //位置索引
    GLuint textCoordIndex; //textCode索引
    GLuint normIndex;   //法向量索引
};

class mesh{
public:
    mesh();
    bool InitMesh(const char* path);
    bool loadVertex(FILE* file);
    bool loadVertexCombineIndex(FILE* file);
    std::vector<Vertex>& getVertData();
    void setVtn(std::string vtn, std::vector<VertexCombineIndex>& vertComIndices);
    std::vector<Vertex> vertData;
    std::vector<int> vertIndex;
private:
    mesh(const mesh&);
    const mesh& operator=(const mesh&);

};
#endif /* ObjLoader_hpp */
