#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Matrix.h"
#include "Shader.h"
#include "Light.h"
#include <stdlib.h>

typedef Vec3 PosCoords;

typedef Vec3 ColVals;

typedef Vec3 Normals;

typedef Vec2 UVCoords;



struct _Vertex{
    PosCoords pos;
    UVCoords uv;
    Normals norm;
};

typedef struct _Vertex Vertex;

struct _TriIndices{
    unsigned int i1;
    unsigned int i2;
    unsigned int i3;
};
typedef struct _TriIndices TriIndices;


struct _Model{
    TriIndices* pTriIndices;
    Vertex* pVertices;

    float specularReflectivity;
    float specularFalloff;


    unsigned int nVertices;
    unsigned int nTriIndices;
    unsigned int texture;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Shader* shader;
    //EntityList* instances;
};
typedef struct _Model Model;

struct _Entity{
    Model* model;
    Vec3 position;
    Vec3 rotation;
};

typedef struct _Entity Entity;


struct _EntityList{
    struct _EntityList* next;
    Entity data;
};
typedef struct _EntityList EntityList;


void bindModel(Model *toBind);
void unbindModel();

void setupUniforms(Entity *entity, Mat4* projMat, Mat4* viewMat, Light* light);

void freeEntityList(EntityList* list);

EntityList* createInstance(Vec3 position, Vec3 rotation); //return the node




#endif // MODEL_H_INCLUDED
