#include "Model.h"


void bindModel(Model *toBind){
    glUseProgram(toBind->shader->shader_id);
    glBindVertexArray(toBind->VAO);
}

void unbindModel(){
    glBindVertexArray(0);
}

void setupUniforms(Entity *entity, Mat4* projMat, Mat4* viewMat, Light* light){
    Mat4 rotMat;
    createRotationMat(entity->rotation.x0, entity->rotation.x1, entity->rotation.x2, &rotMat);
    Mat4 transMat = {0};
    transMat.rows[0].x3 = entity->position.x0;
    transMat.rows[1].x3 = entity->position.x1;
    transMat.rows[2].x3 = entity->position.x2;

    transMat.rows[0].x0 = 1.0f;
    transMat.rows[1].x1 = 1.0f;
    transMat.rows[2].x2 = 1.0f;
    transMat.rows[3].x3 = 1.0f;

    glUniformMatrix4fv(entity->model->shader->shader_uniforms[0], 1, GL_TRUE, (float*)projMat);
    glUniformMatrix4fv(entity->model->shader->shader_uniforms[1], 1, GL_TRUE, (float*)viewMat);
    glUniformMatrix4fv(entity->model->shader->shader_uniforms[2], 1, GL_TRUE, (float*)&transMat);
    glUniformMatrix4fv(entity->model->shader->shader_uniforms[3], 1, GL_TRUE, (float*)&rotMat);


    glUniform3f(entity->model->shader->shader_uniforms[4], light->pos.x0, light->pos.x1, light->pos.x2);
    glUniform3f(entity->model->shader->shader_uniforms[5], light->lightCol.x0, light->lightCol.x1, light->lightCol.x2);
}



void freeEntityList(EntityList* list){
    while(list){
        EntityList* tmp = list->next;
        free(list);
        list = tmp;
    }
}
