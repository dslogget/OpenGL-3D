#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED
#include "Matrix.h"
struct _Light{
    Vec3 pos;
    Vec3 lightCol;
};

typedef struct _Light Light;

#endif // LIGHT_H_INCLUDED
