#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED
#include "Matrix.h"

struct _Camera {
    Vec3 position;
    Vec3 rotation;
};
typedef struct _Camera Camera;


void createViewMatrix(Camera* cam, Mat4* res);

#endif // CAMERA_H_INCLUDED
