//
//  ObjLoader.cpp
//  learnOpenGl
//
//  Created by heshuai on 2018/11/6.
//  Copyright © 2018年 heshuai. All rights reserved.
//
//
#include "mesh.hpp"

mesh::mesh()
{}

bool mesh::InitMesh(const char* filePath)
{
    std::ifstream file(filePath);
    std::vector<VertexCombineIndex> vertComIndices;
    std::vector<std::vector<GLfloat>> temp_vertices;
    std::vector<std::vector<GLfloat>> temp_textCoords;
    std::vector<std::vector<GLfloat>> temp_normals;
    int  curIndex = 0; //记录当前的位置
    if (!file)
    {
        std::cout<<" ObjLoader ERROR: could not open obj file:"<<filePath<<std::endl;
        return false;
    }
    
    std::string line;
    while(std::getline(file, line))
    {
        if("vt" == line.substr(0,2))  //顶点纹理坐标
        {
            std::istringstream s (line.substr(2));
            std::vector<GLfloat> v(2);
            s >> v[0];
            s >> v[1];
            v[1] = -v[1];
            temp_textCoords.push_back(v);
            
        }
        else if("vn" == line.substr(0, 2))  //顶点法向量
        {
            std::istringstream s (line.substr(2));
            
            std::vector<GLfloat> v(3);
            s >> v[0];
            s >> v[1];
            s >> v[2];
            temp_normals.push_back(v);
        }
        else if("v" == line.substr(0, 1))//顶点位置数据
        {
            std::istringstream s (line.substr(2));
            std::vector<GLfloat> v(3);
            s >> v[0];
            s >> v[1];
            s >> v[2];
            temp_vertices.push_back(v);
        }
        else if("f" == line.substr(0, 1))
        {
            std::istringstream vtns(line.substr(2));
            std::string vtn;
            std::vector<std::string> vectVtns;
            while (vtns >> vtn) {
                
                vectVtns.push_back(vtn);
            }
            if (vectVtns.size()==4)
            {
                vertIndex.push_back(curIndex);
                vertIndex.push_back(curIndex+1);
                vertIndex.push_back(curIndex+2);
                vertIndex.push_back(curIndex);
                vertIndex.push_back(curIndex+2);
                vertIndex.push_back(curIndex+3);
                setVtn(vectVtns[0],vertComIndices);
                setVtn(vectVtns[1],vertComIndices);
                setVtn(vectVtns[2],vertComIndices);
                setVtn(vectVtns[3],vertComIndices);
                curIndex += 4;
                
            }else if(vectVtns.size()==3){
                vertIndex.push_back(curIndex);
                vertIndex.push_back(curIndex+1);
                vertIndex.push_back(curIndex+2);
                setVtn(vectVtns[0],vertComIndices);
                setVtn(vectVtns[1],vertComIndices);
                setVtn(vectVtns[2],vertComIndices);
                curIndex += 3;
            }
        }
    }
    
    for (size_t i = 0; i<vertComIndices.size(); i++) {
        Vertex vert;
        VertexCombineIndex v = vertComIndices[i];
        vert.position = temp_vertices[v.posIndex];
        vert.texCoords =temp_textCoords[v.textCoordIndex];
        vert.normal = temp_normals[v.normIndex];
        vertData.push_back(vert);
    }
     return true;
}

void mesh::setVtn(std::string vtn, std::vector<VertexCombineIndex>& vertComIndices)
{
    VertexCombineIndex vertComIndex;
    std::replace(vtn.begin(), vtn.end(), '/', ' ');
    std::istringstream ivtn(vtn);
    
    if(vtn.find("  ") != std::string::npos)  //没有纹理数据
    {
        
    }
    
    ivtn >> vertComIndex.posIndex
    >>  vertComIndex.textCoordIndex
    >>  vertComIndex.normIndex;
    
    vertComIndex.posIndex--;
    vertComIndex.textCoordIndex--;
    vertComIndex.normIndex--;
    vertComIndices.push_back(vertComIndex);
}

