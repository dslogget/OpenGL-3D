#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define VERTINC 1000
#define TRIINC 1000
#define NORMINC 1000
#define TEXINC 1000

#include "FileLoading.h"


void loadModel(char* filePath, Model* toLoad){
    FILE* file = fopen(filePath, "r");
    //First we need to approximate how many vertices to allocate
    //and also how many indices
    //First guess is 1000 of each
    //increment by 1000

    toLoad->nVertices = VERTINC;
    toLoad->nTriIndices = TRIINC;
    unsigned int nNormals = NORMINC;
    unsigned int nTex = TEXINC;

    toLoad->pVertices = malloc(toLoad->nVertices*sizeof(*toLoad->pVertices));
    toLoad->pTriIndices = malloc(toLoad->nTriIndices*sizeof(*toLoad->pTriIndices));
    Normals* pNormals = malloc(nNormals*sizeof(*pNormals));
    UVCoords* pUVs = malloc(nTex*sizeof(*pUVs));


    float f1, f2, f3;
    unsigned int i1, i2, i3, i4, i5, i6, i7, i8, i9;
    char startStr[10] = {0};
    char wholeLine[512] = {0};

    unsigned int loadedVerts = 0;
    unsigned int loadedTris = 0;
    unsigned int loadedNorms = 0;
    unsigned int loadedUVs = 0;


    memset(toLoad->pVertices + (toLoad->nVertices - VERTINC), 0, VERTINC*sizeof(*toLoad->pVertices));

    while(!feof(file)){
        startStr[0] = '\0';
        fscanf(file, "%9s", startStr);
        fgets(wholeLine, 511, file);

        if(!strcmp("v", startStr)){

            sscanf(wholeLine, "%f %f %f", &f1, &f2, &f3);

            toLoad->pVertices[loadedVerts].pos.x0 = f1;
            toLoad->pVertices[loadedVerts].pos.x1 = f2;
            toLoad->pVertices[loadedVerts++].pos.x2 = f3;
        }else if(!strcmp("f", startStr)){

            unsigned int numLoaded = sscanf(wholeLine, "%u/%u/%u %u/%u/%u %u/%u/%u ",
                                            &i1, &i2, &i3, &i4, &i5, &i6, &i7, &i8, &i9);
            if(numLoaded != 9){
                sscanf(wholeLine, "%u//%u %u//%u %u//%u ",
                                            &i1, &i3, &i4, &i6, &i7, &i9);
                i2 = 1;
                i5 = 1;
                i8 = 1;

            }

            toLoad->pTriIndices[loadedTris].i1 = i1-1;
            toLoad->pVertices[i1-1].norm.x0 += pNormals[i3 - 1].x0;
            toLoad->pVertices[i1-1].norm.x1 += pNormals[i3 - 1].x1;
            toLoad->pVertices[i1-1].norm.x2 += pNormals[i3 - 1].x2;
            toLoad->pVertices[i1-1].uv.x0 = pUVs[i2 - 1].x0;
            toLoad->pVertices[i1-1].uv.x1 = pUVs[i2 - 1].x1;


            toLoad->pTriIndices[loadedTris].i2 = i4-1;
            toLoad->pVertices[i4-1].norm.x0 += pNormals[i6 - 1].x0;
            toLoad->pVertices[i4-1].norm.x1 += pNormals[i6 - 1].x1;
            toLoad->pVertices[i4-1].norm.x2 += pNormals[i6 - 1].x2;
            toLoad->pVertices[i4-1].uv.x0 = pUVs[i5 - 1].x0;
            toLoad->pVertices[i4-1].uv.x1 = pUVs[i5 - 1].x1;


            toLoad->pTriIndices[loadedTris++].i3 = i7-1;
            toLoad->pVertices[i7-1].norm.x0 += pNormals[i9 - 1].x0;
            toLoad->pVertices[i7-1].norm.x1 += pNormals[i9 - 1].x1;
            toLoad->pVertices[i7-1].norm.x2 += pNormals[i9 - 1].x2;
            toLoad->pVertices[i7-1].uv.x0 = pUVs[i8 - 1].x0;
            toLoad->pVertices[i7-1].uv.x1 = pUVs[i8 - 1].x1;



        }else if(!strcmp("vn", startStr)){

            sscanf(wholeLine, "%f %f %f", &f1, &f2, &f3);

            pNormals[loadedNorms].x0 = f1;
            pNormals[loadedNorms].x1 = f2;
            pNormals[loadedNorms++].x2 = f3;

        }else if(!strcmp("vt", startStr)){
            sscanf(wholeLine, "%f %f", &f1, &f2);

            pUVs[loadedUVs].x0 = f1;
            pUVs[loadedUVs++].x1 = -f2;

        }




        if(toLoad->nVertices - loadedVerts < 10){
            toLoad->nVertices += VERTINC;
            toLoad->pVertices = realloc(toLoad->pVertices, toLoad->nVertices*sizeof(*toLoad->pVertices));
            memset(toLoad->pVertices + (toLoad->nVertices - VERTINC), 0, VERTINC*sizeof(*toLoad->pVertices));
        }

        if(toLoad->nTriIndices - loadedTris < 10){
            toLoad->nTriIndices += TRIINC;
            toLoad->pTriIndices = realloc(toLoad->pTriIndices, toLoad->nTriIndices*sizeof(*toLoad->pTriIndices));
        }

        if(nNormals - loadedNorms < 10){
            nNormals += NORMINC;
            pNormals = realloc(pNormals, nNormals*sizeof(*pNormals));
        }

        if(nTex - loadedUVs < 10){
            nTex += TEXINC;
            pUVs = realloc(pUVs, nTex*sizeof(*pUVs));
        }


    }

    toLoad->nVertices = loadedVerts;
    toLoad->nTriIndices = loadedTris;

    toLoad->pVertices = realloc(toLoad->pVertices, toLoad->nVertices*sizeof(*toLoad->pVertices));
    toLoad->pTriIndices = realloc(toLoad->pTriIndices, toLoad->nTriIndices*sizeof(*toLoad->pTriIndices));

    for(int i = 0; i < loadedVerts; i++){
        normaliseVector(3, (Vec*)&toLoad->pVertices[i].norm, (Vec*)&toLoad->pVertices[i].norm);
    }


    free(pNormals);
    free(pUVs);

    fclose(file);

    //Setup buffers

    glGenVertexArrays(1, &toLoad->VAO);
    glGenBuffers(1, &toLoad->VBO);
    glGenBuffers(1, &toLoad->EBO);
    glBindVertexArray(toLoad->VAO);



    glBindBuffer(GL_ARRAY_BUFFER, toLoad->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*toLoad->pVertices)*toLoad->nVertices, toLoad->pVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, toLoad->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*toLoad->pTriIndices)*toLoad->nTriIndices, toLoad->pTriIndices, GL_STATIC_DRAW);



    glVertexAttribPointer(0, sizeof(toLoad->pVertices->pos)/4, GL_FLOAT, GL_FALSE, sizeof(*toLoad->pVertices),
                          (void*)0);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, sizeof(toLoad->pVertices->uv)/4, GL_FLOAT, GL_FALSE, sizeof(*toLoad->pVertices),
                          (void*)(sizeof(toLoad->pVertices->pos)));

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, sizeof(toLoad->pVertices->norm)/4, GL_FLOAT, GL_FALSE, sizeof(*toLoad->pVertices),
                          (void*)(sizeof(toLoad->pVertices->pos) + sizeof(toLoad->pVertices->uv)));

    glEnableVertexAttribArray(2);


}

void freeModelData(Model* toLoad){
    free(toLoad->pVertices);
    free(toLoad->pTriIndices);
}

char* loadFileToStr(char* fileName){
    char * buffer = 0;
    long length;
    FILE * file = fopen (fileName, "rb");

    if (file){
      fseek (file, 0, SEEK_END);
      length = ftell (file);
      fseek (file, 0, SEEK_SET);
      buffer = malloc(length + 1);
      memset(buffer, 0, length + 1);
      if (buffer)
      {
        fread (buffer, 1, length, file);
      }
      fclose (file);
    }

    return buffer;

}


unsigned int loadShader(char* vertPath, char* fragPath){

    char* vertexShaderSource = loadFileToStr(vertPath);
    char* fragmentShaderSource = loadFileToStr(fragPath);

    const char *vertexShaderSource_const = vertexShaderSource;
    const char *fragmentShaderSource_const = fragmentShaderSource;

    unsigned int shaderProgram;

    //Setup
    int  success = 0;
    char infoLog[512];

    //Shader

    unsigned int vertexShader;
    unsigned int fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource_const, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);



    if(success == GL_FALSE){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource_const, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(success == GL_FALSE){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINK_FAILED\n%s\n", infoLog);
    }

    glUseProgram(shaderProgram);

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free(vertexShaderSource);
    free(fragmentShaderSource);

    return shaderProgram;

}

unsigned int loadTexture(char* filename, unsigned int textureBank){
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    unsigned int texture;
    if(data){
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0 + textureBank); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }else{
        printf("Cannot load texture");
    }


    return texture;

}


void loadDatFile(char* filepath, Model* modelToLoad, Shader* shaderToLoad){
    FILE* datfile = fopen(filepath, "r");

    memset(modelToLoad, 0, sizeof(*modelToLoad));
    memset(shaderToLoad, 0, sizeof(*shaderToLoad));

    char wholeLine[512];
    char startStr[128];
    unsigned int numUniforms = 0;

    while(!feof(datfile)){
        startStr[0] = '\0';
        fscanf(datfile, "%9s", startStr);
        fgets(wholeLine, 511, datfile);

        if(!strcmp("o", startStr)){
            char objfile[64];
            sscanf(wholeLine, "%s", objfile);
            loadModel(objfile, modelToLoad);
        }else if(!strcmp("t", startStr)){
            char texfile[64];
            sscanf(wholeLine, "%s", texfile);
            modelToLoad->texture = loadTexture(texfile, 0);
        }else if(!strcmp("l", startStr)){
            sscanf(wholeLine, "%f %f", &modelToLoad->specularReflectivity, &modelToLoad->specularFalloff);
        }else if(!strcmp("s", startStr)){
            char vertshader[64];
            char fragshader[64];
            sscanf(wholeLine, "%s %s", vertshader, fragshader);
            shaderToLoad->shader_id = loadShader(vertshader, fragshader);
            modelToLoad->shader = shaderToLoad;
        }else if(!strcmp("u", startStr)){
            char uniformName[64];
            sscanf(wholeLine, "%s", uniformName);
            shaderToLoad->shader_uniforms[numUniforms++] = glGetUniformLocation(shaderToLoad->shader_id, uniformName);
        }

    }

    fclose(datfile);
}
