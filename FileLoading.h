#ifndef FILELOADING_H_INCLUDED
#define FILELOADING_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "Model.h"
#include <string.h>


void loadDatFile(char* filepath, Model* modelToLoad, Shader* shaderToLoad);

void loadModel(char* filePath, Model* toLoad);
void freeModelData(Model* toLoad);

unsigned int loadShader(char* vertPath, char* fragPath);

char* loadFileToStr(char* fileName);

unsigned int loadTexture(char* filename, unsigned int textureBank);

#endif // FILELOADING_H_INCLUDED
