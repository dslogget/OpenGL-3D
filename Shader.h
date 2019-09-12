#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

struct _Shader{
    unsigned int shader_id;
    unsigned int shader_uniforms[10];
};

typedef struct _Shader Shader;

#endif // SHADER_H_INCLUDED
