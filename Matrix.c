#include "Matrix.h"


void addVectorVector(unsigned int vecSize, Vec* a, Vec* b, Vec* dest){
    float *pA, *pB, *pD;

    pA = (float*)a;
    pB = (float*)b;
    pD = (float*)dest;

    for(int i = 0; i < vecSize; i++){
        *(pD + i) = *(pA + i) + *(pB + i);
    }
}

void subVectorVector(unsigned int vecSize, Vec* a, Vec* b, Vec* dest){
    float *pA, *pB, *pD;

    pA = (float*)a;
    pB = (float*)b;
    pD = (float*)dest;

    for(int i = 0; i < vecSize; i++){
        *(pD + i) = *(pA - i) + *(pB - i);
    }
}

float dotVectorVector(unsigned int vecSize, Vec* a, Vec* b){
    float toRet = 0.0f;
    float *pA, *pB;

    pA = (float*)a;
    pB = (float*)b;


    for(int i = 0; i < vecSize; i++){
        toRet += (*(pA + i)) * (*(pB + i));
    }
    return toRet;
}

void normaliseVector(unsigned int vecSize, Vec* a, Vec* dest){
    float fact = 1.0f/sqrtf(dotVectorVector(vecSize, a, a));
    multVecFloat(vecSize, a, fact, dest);
}

void multMatVec(unsigned int vecSize, Mat* mat, Vec* vec, Vec* dest){
    float *pRow, *pD;
    pRow = (float*)mat;
    Vec res = {0};
    pD = (float*)&res;

    for(int i = 0; i < vecSize; i++){
    float dot = dotVectorVector(vecSize, (Vec*)(pRow + vecSize*i), vec);
    *(pD + i) = dot;
    }

    memcpy(dest, &res, vecSize * sizeof(float));
}

void transposeMat(unsigned int vecSize, Mat* mat, Mat* dest){
    Mat res = {0};

    float *pMat, *pDest;

    pMat = (float*)mat;
    pDest = (float*)&res;

    for(int row = 0; row < vecSize; row++){
        for(int col = 0; col < vecSize; col++){
            *(pDest + row + col*vecSize) = *(pMat + col + row*vecSize);
        }
    }

    memcpy(dest, &res, vecSize * vecSize * sizeof(float));

}


void multMatMat(unsigned int vecSize, Mat* matA, Mat* matB, Mat* destMat){
    Mat transpose;

    transposeMat(vecSize, matB, &transpose);

    float *pTransRow;
    pTransRow = (float*)&transpose;

    for(int row = 0; row < vecSize; row++){
        multMatVec(vecSize, matA, (Vec*)(pTransRow + row*vecSize), (Vec*)(pTransRow + row*vecSize));
    }

    transposeMat(vecSize, &transpose, destMat);

}

void multVecFloat(unsigned int vecSize, Vec* a, float f, Vec* dest){
    float *pA, *pD;

    pA = (float*)a;
    pD = (float*)dest;

    for(int i = 0; i < vecSize; i++){
        *(pD + i) = (*(pA + i))*f;
    }


}

void multMatFloat(unsigned int vecSize, Mat* a, float f, Mat* dest){
    float *pA, *pD;

    pA = (float*)a;
    pD = (float*)dest;

    for(int i = 0; i < vecSize*vecSize; i++){
        *(pD + i) = (*(pA - i))*f;
    }


}




void printVec(unsigned int vecSize, Vec* vec){
    float *pA;
    pA = (float*)vec;

    for(int i = 0; i < vecSize; i++){
        printf("%f\n", *(pA + i));
    }

}

void printMat(unsigned int vecSize, Mat* mat){
    float *pA;
    pA = (float*)mat;

    for(int row = 0; row < vecSize; row++){
        for(int col = 0; col < vecSize; col++){
            printf("%f ", *(pA + row*vecSize + col));
        }
        printf("\n");
    }

}


void createProjMat(float dFOV, unsigned int wScreen, unsigned int hScreen, float nearPlane, float farPlane, Mat4* res){
    float cotan = 1.0f/tan(M_PI/360.0 * dFOV);
    float aspectRatio = (float)wScreen/(float)hScreen;
    float plDiff = farPlane - nearPlane;
    float plSum = farPlane + nearPlane;

    res->rows[0].x0 = cotan / aspectRatio;  res->rows[0].x1 = 0.0f;     res->rows[0].x2 = 0.0f;         res->rows[0].x3 = 0.0f;
    res->rows[1].x0 = 0.0f;                 res->rows[1].x1 = cotan;    res->rows[1].x2 = 0.0f;         res->rows[1].x3 = 0.0f;
    res->rows[2].x0 = 0.0f;                 res->rows[2].x1 = 0.0f;     res->rows[2].x2 = -plSum/plDiff;res->rows[2].x3 = -2.0f*farPlane*nearPlane/plDiff;
    res->rows[3].x0 = 0.0f;                 res->rows[3].x1 = 0.0f;     res->rows[3].x2 = -1.0f;        res->rows[3].x3 = 0.0f;
}

void createRotationMat(float dX, float dY, float dZ, Mat4* res){
    Mat4 temp;

    createRotationMatX(dX, res);
    createRotationMatY(dY, &temp);
    multMatMat(4, (Mat*)&temp, (Mat*)res, (Mat*)res);
    createRotationMatZ(dZ, &temp);
    multMatMat(4, (Mat*)&temp, (Mat*)res, (Mat*)res);
}

void createRotationMatX(float dX, Mat4* res){
    float sinX = sin(M_PI/180 * dX);
    float cosX = cos(M_PI/180 * dX);

    res->rows[0].x0 = 1.0f;  res->rows[0].x1 = 0.0f;     res->rows[0].x2 = 0.0f;     res->rows[0].x3 = 0.0f;
    res->rows[1].x0 = 0.0f;  res->rows[1].x1 = cosX;     res->rows[1].x2 = -sinX;    res->rows[1].x3 = 0.0f;
    res->rows[2].x0 = 0.0f;  res->rows[2].x1 = sinX;     res->rows[2].x2 = cosX;     res->rows[2].x3 = 0.0f;
    res->rows[3].x0 = 0.0f;  res->rows[3].x1 = 0.0f;     res->rows[3].x2 = 0.0f;     res->rows[3].x3 = 1.0f;


}

void createRotationMatY(float dY, Mat4* res){
    float sinY = sin(M_PI/180 * dY);
    float cosY = cos(M_PI/180 * dY);

    res->rows[0].x0 = cosY;  res->rows[0].x1 = 0.0f;     res->rows[0].x2 = sinY;     res->rows[0].x3 = 0.0f;
    res->rows[1].x0 = 0.0f;  res->rows[1].x1 = 1.0f;     res->rows[1].x2 = 0.0f;     res->rows[1].x3 = 0.0f;
    res->rows[2].x0 = -sinY; res->rows[2].x1 = 0.0f;     res->rows[2].x2 = cosY;     res->rows[2].x3 = 0.0f;
    res->rows[3].x0 = 0.0f;  res->rows[3].x1 = 0.0f;     res->rows[3].x2 = 0.0f;     res->rows[3].x3 = 1.0f;


}

void createRotationMatZ(float dZ, Mat4* res){
    float sinZ = sin(M_PI/180 * dZ);
    float cosZ = cos(M_PI/180 * dZ);

    res->rows[0].x0 = cosZ;  res->rows[0].x1 = -sinZ;    res->rows[0].x2 = 0.0f;     res->rows[0].x3 = 0.0f;
    res->rows[1].x0 = sinZ;  res->rows[1].x1 = cosZ;     res->rows[1].x2 = 0.0f;     res->rows[1].x3 = 0.0f;
    res->rows[2].x0 = 0.0f;  res->rows[2].x1 = 0.0f;     res->rows[2].x2 = 1.0f;     res->rows[2].x3 = 0.0f;
    res->rows[3].x0 = 0.0f;  res->rows[3].x1 = 0.0f;     res->rows[3].x2 = 0.0f;     res->rows[3].x3 = 1.0f;

}
