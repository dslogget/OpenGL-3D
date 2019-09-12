#include "Camera.h"

void createViewMatrix(Camera* cam, Mat4* res){
    Mat4 rotMat;
    createRotationMat(-cam->rotation.x0, -cam->rotation.x1, -cam->rotation.x2, &rotMat);
    Mat4 transMat = {0};
    transMat.rows[0].x3 = -cam->position.x0;
    transMat.rows[1].x3 = -cam->position.x1;
    transMat.rows[2].x3 = -cam->position.x2;

    transMat.rows[0].x0 = 1.0f;
    transMat.rows[1].x1 = 1.0f;
    transMat.rows[2].x2 = 1.0f;
    transMat.rows[3].x3 = 1.0f;
    multMatMat(4, (Mat*)&rotMat, (Mat*)&transMat, (Mat*)res);
}
