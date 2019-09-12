#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Model.h"
#include "FileLoading.h"
#include "Matrix.h"
#include "Shader.h"
#include "Camera.h"


#define DEFWIDTH    2*1280
#define DEFHEIGHT    2*720
#define TARGETFRAMETIME 1.0f/60.0f

#define FOV 50.0f
#include "Light.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, float deltaTime);
void render(GLFWwindow *window);



Model myMod = {0};
Shader basicShader = {0};
Mat4 projMat = {0};
Mat4 viewMat = {0};
Entity myEnt = {0};
Entity myEnt2 = {0};
Camera cam = {0};
Light myLight = {0};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(DEFWIDTH, DEFHEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            printf("Failed to initialize GLAD\n");
        return -1;
    }

    glViewport(0, 0, DEFWIDTH, DEFHEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);


    //Load Model
    loadDatFile("sphere.dat", &myMod, &basicShader);
    glUniform1f(basicShader.shader_uniforms[6], myMod.specularReflectivity);
    glUniform1f(basicShader.shader_uniforms[7], myMod.specularFalloff);



    myEnt.model = &myMod;
    myEnt.position = (Vec3){{0.0f, 0.0f, -5.0f}};

    myEnt2 = myEnt;

    createProjMat(FOV, DEFWIDTH, DEFHEIGHT, 0.01f, 100.0f, &projMat);




    myLight.lightCol = (Vec3){{1.0f, 1.0f, 1.0f}};
    myLight.pos = (Vec3){{0.0f, 10.0f, 10.0f}};
    myEnt2.position = myLight.pos;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    //myEnt.rotation.x0 = 45.0f;
    double prev_time = glfwGetTime();

    while(!glfwWindowShouldClose(window)){
        double curr_time = glfwGetTime();
        float deltaTime = curr_time - prev_time;
        //printf("%f", deltaTime);
        processInput(window, deltaTime);


        myEnt.rotation.x1 += 10.0f*deltaTime;
        render(window);

        char title[64];

        sprintf(title, "FPS: %1.f", 1.0f/(deltaTime));

        glfwSetWindowTitle(window, title);

        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();

        //if(deltaTime < TARGETFRAMETIME){
          //  glfw
        //}
        prev_time = curr_time;
    }

    freeModelData(&myMod);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    createProjMat(FOV, width, height, 0.01f, 100.0f, &projMat);
}

void processInput(GLFWwindow *window, float deltaTime){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }

    Vec changeInCam = {0};
    changeInCam.vec4.x3 = 1.0f;
    Mat4 rotMat = {0};
    createRotationMat(cam.rotation.x0, cam.rotation.x1, cam.rotation.x2, &rotMat);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        changeInCam.vec4.x2 -= 5.0f * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        changeInCam.vec4.x2 += 5.0f * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        changeInCam.vec4.x0 -= 5.0f * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        changeInCam.vec4.x0 += 5.0f * deltaTime;
    }


    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        cam.rotation.x1 += 100.0f * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        cam.rotation.x1 -= 100.0f * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        changeInCam.vec4.x1 -= 10.0f * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        changeInCam.vec4.x1 += 10.0f * deltaTime;
    }



    multMatVec(4, (Mat*)&rotMat, &changeInCam, &changeInCam);


    addVectorVector(3, (Vec*)&cam.position, &changeInCam, (Vec*)&cam.position);
}

void render(GLFWwindow *window){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, myMod.texture);
    bindModel(myEnt.model);
    createViewMatrix(&cam, &viewMat);
    setupUniforms(&myEnt, &projMat, &viewMat, &myLight);
    glDrawElements(GL_TRIANGLES, 3*myMod.nTriIndices, GL_UNSIGNED_INT, 0);

    setupUniforms(&myEnt2, &projMat, &viewMat, &myLight);
    glDrawElements(GL_TRIANGLES, 3*myMod.nTriIndices, GL_UNSIGNED_INT, 0);


    unbindModel();
}
