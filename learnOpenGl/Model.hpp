//
//  ModelClass.hpp
//  learnOpenGl
//
//  Created by heshuai on 2018/11/8.
//  Copyright © 2018年 heshuai. All rights reserved.
//

#ifndef ModelClass_hpp
#define ModelClass_hpp
#include "LoadShader.hpp"
#include "Vector4.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"
#include "Texture.hpp"
#include "mesh.hpp"
#include "Camera.hpp"

#include <iostream>
#include <cmath>
////第三方库

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/OpenGL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
static double gScrollY = 0.0;
static int instantcount = 1024;  //4的指数

class Model{
public:
    Model();
    void initModeData(const char* objFilePath);
    void initModeData(GLfloat vertexData[], GLfloat vertexindeData[]);
    
    void loadTexture(const char* objTextFilePath);
    void loadShader(const char* vertShaderPath, const char* fragShaderPath);
    void setCameraInfo(Vector3 cameraPostion, float fieldOfView, float nearPlane, float farPlane, float viewportAspectRatio);
    void run(GLFWwindow* win);  //model渲染、加载函数
    void setLightDiffuse(float R,float G,float B);
    void setLightAmbient(float R,float G,float B);
    void setLightSpecular(float R,float G,float B);
    
private:
    GLuint _programID;   //着色器programID
    Texture* _gTexture = NULL;
    
    GLuint _gVAO = 0;
    GLint _vertCount;
    Camera _gCamera;
    float _gDegreesRotated;
    std::vector<GLfloat> _diffuse;
    std::vector<GLfloat> _ambient;
    std::vector<GLfloat> _specular;
    
    void _loadTexture(const char* objTextFilePath);
    void _loadShader(const char* vertShaderPath, const char* fragShaderPath);
    void _initModeData(const mesh& obj);
    void _render(GLFWwindow* win);
    void _update(float secondsElapsed,GLFWwindow* win);
    void _renderSetLight();
};

Model::Model()
:_programID(0)
,_gTexture(NULL)
,_gVAO(0)
,_gDegreesRotated(0.0f)
,_vertCount(0)
{
    for (int i = 0; i < 3; i++) {
        _diffuse.push_back(0.0f);
        _ambient.push_back(0.0f);
        _specular.push_back(0.0f);
    }
}

void Model::initModeData(const char * objFilePath)
{
    mesh o(objFilePath);
    _initModeData(o);
}

void Model::setLightDiffuse(float R,float G,float B)
{
    _diffuse[0] = R;
    _diffuse[1] = G;
    _diffuse[2] = B;
}

void Model::setLightAmbient(float R,float G,float B)
{
    _ambient[0] = R;
    _ambient[1] = G;
    _ambient[2] = B;
}

void Model::setLightSpecular(float R,float G,float B)
{
    _specular[0] = R;
    _specular[1] = G;
    _specular[2] = B;
}

void Model::loadTexture(const char* objTextFilePath)
{
    _loadTexture(objTextFilePath);
}

void Model::loadShader(const char* vertShaderPath, const char* fragShaderPath)
{
    _loadShader(vertShaderPath, fragShaderPath);
}

void Model::setCameraInfo(Vector3 cameraPostion, float fieldOfView, float nearPlane, float farPlane, float viewportAspectRatio)
{
    _gCamera.setPosition(cameraPostion);
    _gCamera.setFieldOfView(fieldOfView);
    _gCamera.setViewportAspectRatio(viewportAspectRatio);
    _gCamera.setNearAndFarPlanes(nearPlane, farPlane);
}

void Model::run(GLFWwindow* win)
{

    double lastTime = glfwGetTime();
    int frame = 0;
    while(!glfwWindowShouldClose(win)){
        glfwPollEvents();
        double thisTime = glfwGetTime();
        _update((float)(thisTime - lastTime), win);
        _render(win);
        frame++;
        if(thisTime - lastTime > 1)
        {
            printf("FPS:%4.2f\n",
                   frame*1/(thisTime - lastTime));
            lastTime = thisTime;
            frame = 0;
        }
        
        if(glfwGetKey(win, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(win, GL_TRUE);
    }
}

//////////////////
////private
/////////////////
void Model::_initModeData(const mesh& obj)
{
    GLuint gVBO = 0;
    GLuint indexBuff = 0;
    
    //分配绑定顶点数组对象
    glGenVertexArrays(1,&_gVAO);
    glBindVertexArray(_gVAO);
    
    //分配绑定缓存对象
    glGenBuffers(1,&gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    
    //设置模型的顶点信息到缓存中
    std::vector<Vertex> v = obj.vertData;
    std::vector<int> vertIndexs = obj.vertIndex;
    _vertCount = (int)v.size();
    GLfloat vectData[_vertCount*8];
    GLint vectIndex[vertIndexs.size()];
    
    for (size_t i = 0; i<v.size(); i++) {
        size_t index = i*8;
        for (size_t posIndex = 0; posIndex < v[i].position.size(); posIndex++) {
            vectData[index + posIndex] = v[i].position[posIndex];
        }
        index += 3;
        for (size_t norIndex = 0; norIndex < v[i].normal.size(); norIndex++) {
            vectData[index + norIndex] = v[i].normal[norIndex];
        }
        index += 3;
        for (size_t texCoordIndex = 0; texCoordIndex < v[i].texCoords.size(); texCoordIndex++) {
            vectData[index + texCoordIndex] = v[i].texCoords[texCoordIndex];
        }
    }
    
    for (size_t i = 0; i<vertIndexs.size(); i++) {
        vectIndex[i] = vertIndexs[i];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vectData),vectData, GL_STATIC_DRAW);
    // 顶点位置
    GLint vertLocation = glGetAttribLocation(_programID,"vert");
    glEnableVertexAttribArray(vertLocation);
    glVertexAttribPointer(vertLocation, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLfloat*)0);
    //顶点法向量
    GLint verNormalLocation = glGetAttribLocation(_programID,"verNormal");
    glEnableVertexAttribArray(verNormalLocation);
    glVertexAttribPointer(verNormalLocation, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
    //顶点纹理信息
    GLint vertTexCoordLocation = glGetAttribLocation(_programID,"vertTexCoord");
    glEnableVertexAttribArray(vertTexCoordLocation);
    glVertexAttribPointer(vertTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (const GLvoid*)(6 * sizeof(GLfloat)));
    
    GLuint instanceVBO;
    GLfloat offset[instantcount * 2];
    int index = 0;
    int minPos = (sqrt(instantcount) - 1) * - 100;
    int maxPos = (sqrt(instantcount) - 1) * 100;
    
    for(int i = minPos; i<= maxPos;)
    {
        for (int j = minPos; j<= maxPos;)
        {
            offset[index++] = i;
            offset[index++] = j;
            j+= 200;
        }
         i += 200;
    }

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(offset), offset, GL_STATIC_DRAW);

    GLint offsetLocation = glGetAttribLocation(_programID,"offset");
    glEnableVertexAttribArray(offsetLocation);
    glVertexAttribPointer(offsetLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribDivisor(offsetLocation, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //绑定index信息
    glGenBuffers(1,&indexBuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vectIndex), vectIndex, GL_STATIC_DRAW);
    
    //解除对gVAO与gVBO的绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::_loadShader(const char *vertShaderPath, const char *fragShaderPath)
{
    _programID = LoadShaders(vertShaderPath, fragShaderPath);
}

void Model::_loadTexture(const char *objTextFilePath)
{
    _gTexture = new Texture(objTextFilePath);
}

void Model::_render(GLFWwindow* win)
{
//    glClearColor(1.0f, 1.0f,1.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//    char *str = "current fps = ";
//    int n = strlen(str);
//    //设置要在屏幕上显示字符的起始位置
////    glRasterPos2f(0,0);
//    //逐个显示字符串中的每个字符
//    
//    for (int i = 0; i < n; i++)
//    {
//        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *(str+i));
//    }
//
    
    glUseProgram(_programID);//激活着色器
    
    Matrix4* model = new Matrix4();
//    model->rotateY(_gDegreesRotated);
    
    _renderSetLight();
    GLuint cameraID = glGetUniformLocation(_programID, "camera");
    glUniformMatrix4fv(cameraID, 1, GL_FALSE, _gCamera.matrix().get());
    
    GLuint modelID = glGetUniformLocation(_programID, "model");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, model->get());
   
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _gTexture->object());
    GLuint texID = glGetUniformLocation(_programID, "tex");
    glUniform1i(texID, 0);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE); //剔除背面  提高效率
    
    glBindVertexArray(_gVAO);
    
    if (instantcount > 0)
    {
        glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)_vertCount*3, GL_UNSIGNED_INT, NULL,instantcount);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, (GLsizei)_vertCount*3, GL_UNSIGNED_INT, NULL);
    }
    
//    for (int i =0; i<instantcount; i++) {
//            glDrawElements(GL_TRIANGLES, (GLsizei)_vertCount*3, GL_UNSIGNED_INT, NULL);
//
//    }

    //释放之前绑定的顶点数组与着色器
    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers(win);
}

void Model::_update(float secondsElapsed,GLFWwindow* win)
{
    const GLfloat degreesPerSecond = 90.0f;
    _gDegreesRotated += secondsElapsed * degreesPerSecond;
    while(_gDegreesRotated > 360.0f) _gDegreesRotated -= 360.0f;
    
    const float moveSpeed = 2.0; //units per second
    
    if(glfwGetKey(win, 'S')){
        _gCamera.offsetPosition(secondsElapsed * moveSpeed * -_gCamera.forward());
        
    } else if(glfwGetKey(win, 'W')){
        _gCamera.offsetPosition(secondsElapsed * moveSpeed * _gCamera.forward());
    }
    if(glfwGetKey(win, 'Z')){
        _gCamera.offsetPosition(secondsElapsed * moveSpeed * -Vector3(0,1,0));
    } else if(glfwGetKey(win, 'X')){
        _gCamera.offsetPosition(secondsElapsed * moveSpeed * Vector3(0,1,0));
    }
    
    float R = _ambient[0];
    float G = _ambient[1];
    float B = _ambient[2];
    if(glfwGetKey(win, '1')){
        R = 1.0f; G = 0.0f; B = 0.0f;
        
    } else if(glfwGetKey(win, '2')){
        R = 0.0f; G = 1.0f; B = 0.0f;
    }
    if(glfwGetKey(win, '3')){
        R = 1.0f; G = 1.0f; B = 1.0f;
    } else if(glfwGetKey(win, '4')){
        R = 0.5f; G = 0.5f; B = 0.5f;
    }
    setLightAmbient(R,G,B);
    setLightDiffuse(R,G,B);
    
    const float mouseSensitivity = 0.1f;
    double mouseX, mouseY;
    glfwGetCursorPos(win, &mouseX, &mouseY);
    _gCamera.offsetOrientation(mouseSensitivity * (float)mouseY, mouseSensitivity * (float)mouseX);
    glfwSetCursorPos(win, 0, 0); //重置鼠标，这样它就不会离开窗口
    
    //increase or decrease field of view based on mouse wheel
    const float zoomSensitivity = -0.2f;
    float fieldOfView = _gCamera.fieldOfView() + zoomSensitivity * (float)gScrollY;
    if(fieldOfView < 5.0f) fieldOfView = 5.0f;
    if(fieldOfView > 130.0f) fieldOfView = 130.0f;
    _gCamera.setFieldOfView(fieldOfView);
    gScrollY = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Model::_renderSetLight()
{
    GLfloat postion[] = {_gCamera.position().x, _gCamera.position().y, _gCamera.position().z};
    GLfloat diffuse[] = {_diffuse[0], _diffuse[1], _diffuse[2]};
    GLfloat ambient[] = {_ambient[0], _ambient[1], _ambient[2]};
    GLfloat specular[] = {_specular[0], _specular[1], _specular[2]};
    GLuint positionID = glGetUniformLocation(_programID, "light.position");
    glUniform3fv(positionID, 1, postion);
    
    GLuint diffuseID = glGetUniformLocation(_programID, "light.diffuse");
    glUniform3fv(diffuseID, 1, diffuse);
    
    GLuint ambientID = glGetUniformLocation(_programID, "light.ambient");
    glUniform3fv(ambientID, 1, ambient);
    
    GLuint specularID = glGetUniformLocation(_programID, "light.specular");
    glUniform3fv(specularID, 1, specular);
}
#endif /* ModelClass_hpp */
