//
//  Matrix4Test.cpp
//  learnOpenGl
//
//  Created by heshuai on 2018/10/31.
//  Copyright © 2018年 heshuai. All rights reserved.
//
#include "Matrix4.hpp"
#include <GLUT/GLUT.h>
using namespace std;

void Matrix4Test()
{
    Vector3 camera(3,0,3);
    Vector3 tar(-3,0,3);
    Vector3 up(0,1,0);
    Matrix4* mat2 = new Matrix4();
    
    mat2->lookAt(camera, tar, up);
    cout<<"###################"<<endl;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            printf("%8.2f, ", mat2->get()[i*4+j]);
        }
        cout<<endl;
    }
    cout<<"#################"<<endl;
    Matrix4* mat = new Matrix4();
    mat->Perspective(90.0f, 4.0f/3.0f, 1.0f, 100.0f);
    
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            printf("%8.2f, ", mat->get()[i*4+j]);
        }
        cout<<endl;
    }
    
    Matrix4* mode = new Matrix4();
    cout<<"*********************"<<endl;
    Matrix4 mvp = (*mat) * (*mat2) * (*mode);
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            printf("%8.2f, ", mvp.get()[i*4+j]);
        }
        cout<<endl;
    }
}
