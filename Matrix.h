#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED
#include <string.h>
#include <stdio.h>
#include <math.h>


//Types
struct _Vec2{
    float x0;
    float x1;
};
typedef struct _Vec2 Vec2;


union _Vec3{
    struct{
        float x0;
        float x1;
        float x2;
    };
    Vec2 vec2;
};
typedef union _Vec3 Vec3;

union _Vec4{
    struct{
        float x0;
        float x1;
        float x2;
        float x3;
    };
    Vec3 vec3;
    Vec2 vec2;
};
typedef union _Vec4 Vec4;

union _Vec{
    Vec4 vec4;
    Vec3 vec3;
    Vec2 vec2;
};
typedef union _Vec Vec;


struct _Mat2{
    Vec2 rows[2];
};
typedef struct _Mat2 Mat2;

struct _Mat3{
    Vec3 rows[3];
};
typedef struct _Mat3 Mat3;

struct _Mat4{
    Vec4 rows[4];
};
typedef struct _Mat4 Mat4;

union _Mat{
    Mat4 mat4;
    Mat3 mat3;
    Mat2 mat2;
    struct{
        Vec* rows;
    };
};
typedef union _Mat Mat;

//Functions
void addVectorVector(unsigned int vecSize, Vec* a, Vec* b, Vec* dest);
void subVectorVector(unsigned int vecSize, Vec* a, Vec* b, Vec* dest);
float dotVectorVector(unsigned int vecSize, Vec* a, Vec* b);
void normaliseVector(unsigned int vecSize, Vec* a, Vec* dest);

void multMatVec(unsigned int vecSize, Mat* mat, Vec* vec, Vec* dest);
void transposeMat(unsigned int vecSize, Mat* mat, Mat* dest);
void multMatMat(unsigned int vecSize, Mat* matA, Mat* matB, Mat* destMat);
void multVecFloat(unsigned int vecSize, Vec* a, float f, Vec* dest);
void multMatFloat(unsigned int vecSize, Mat* a, float f, Mat* dest);

void printVec(unsigned int vecSize, Vec* vec);
void printMat(unsigned int vecSize, Mat* mat);

void createProjMat(float dFOV, unsigned int wScreen, unsigned int hScreen, float nearPlane, float farPlane, Mat4* res);
void createTranslationMat(Vec3* translation, Mat4* res);
void createRotationMat(float dX, float dY, float dZ, Mat4* res);
void createRotationMatX(float dX, Mat4* res);
void createRotationMatY(float dY, Mat4* res);
void createRotationMatZ(float dZ, Mat4* res);


#endif // MATRIX_H_INCLUDED
