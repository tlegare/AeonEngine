//
//  aeonKernel.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 5/7/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//
//#define AEON_DEBUG


#include <math.h>
#include <stdlib.h>

#include "aeonKernel.h"
#include "aeonEngineInternal.h"
#include "aeonPool.h"
#include "vertexShader.h"

#include "aeonCamera.h"

#define MAX_ATLASES POOL_MAX

static aeonPool<aeon_Atlas> atlasPool;

aeon_AtlasHandle currentAtlas = 0;

//----------------------------------------
// SDL\OpenGL
//----------------------------------------
#ifdef FRAMEWORK_IS_SDL

struct vertex_position
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct vertex_texcoord
{
    GLfloat u;
    GLfloat v;
};

struct vertex_color
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

struct vertex_translation
{
    GLfloat x;
    GLfloat y;
};

struct vertex_transform
{
    vertex_translation translation;
    GLfloat rotation;    
    GLfloat scale;
};

struct vertex
{
    vertex_position position;
    vertex_texcoord texcoord;
    vertex_color color;
    vertex_transform transform;
};

struct tri
{
    vertex v1;
    vertex v2;
    vertex v3;
};

struct quad
{
    tri top;
    tri bottom;
};

#define MAX_QUADS 2048
static quad quadArray[MAX_QUADS];
quad* curQuad = quadArray;

enum shaderAttributes { vert_data, tex_data, color_data, xform_data };

inline void vertexSetPosition(vertex* vert, GLfloat x, GLfloat y, GLfloat z)
{
    vert->position.x = x;
    vert->position.y = y;
    vert->position.z = z;
}

inline void vertexSetTexcoord(vertex* vert, GLfloat u, GLfloat v)
{
    vert->texcoord.u = u;
    vert->texcoord.v = v;
}

inline void vertexSetColor(vertex* vert, aeon_Color c)
{
    vert->color.r = c.r;
    vert->color.g = c.g;
    vert->color.b = c.b;
    vert->color.a = c.a;
}

inline void vertexSetTranform(vertex* vert, GLfloat transX, GLfloat transY, GLfloat rotation, GLfloat scale)
{
    vert->transform.translation.x = transX;
    vert->transform.translation.y = transY;
    vert->transform.rotation = rotation;
    vert->transform.scale = scale;
}

inline void quadSetTransform(quad* q, GLfloat transX, GLfloat transY, GLfloat rotation, GLfloat scale)
{
    vertexSetTranform(&q->top.v1, transX, transY, rotation, scale);
    vertexSetTranform(&q->top.v2, transX, transY, rotation, scale);
    vertexSetTranform(&q->top.v3, transX, transY, rotation, scale);
    vertexSetTranform(&q->bottom.v1, transX, transY, rotation, scale);
    vertexSetTranform(&q->bottom.v2, transX, transY, rotation, scale);
    vertexSetTranform(&q->bottom.v3, transX, transY, rotation, scale);
}

GLuint imageVBO;

// Shader stuff
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

aeon_AtlasHandle aeon_LoadAtlas(const char* path)
{
    SDL_Surface* s;
    GLenum format;
    GLint numColors;
    GLuint texture = 0;
    
    s = IMG_Load(path);
    if(!s) assert(0); // load failed!
    if((s->w & (s->w - 1)) != 0) assert(0); // not power of 2!
    if((s->h & (s->h - 1)) != 0) assert(0); // not power of 2!
    
    numColors = s->format->BytesPerPixel;
    if(numColors == 4)
    {
        if(s->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
            format = GL_BGRA;
    }
    else assert(0); // unsupported format!
    
    glGenTextures(1, &texture);
    assert(texture); // failed to build texture!
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, numColors, s->w, s->h, 0, format, GL_UNSIGNED_BYTE, s->pixels);
    
    aeon_Atlas* newAtlas = NULL;
    aeon_AtlasHandle newAtlasHandle = 0;
    
    unsigned int i = atlasPool.getNextEmptySlot();
    newAtlas = &atlasPool.storage[i];
    newAtlasHandle = atlasPool.mangleHandle(i);
    
    newAtlas->glTexture = texture;
    newAtlas->width = s->w;
    newAtlas->height = s->h;
    newAtlas->isAlive = true;
    SDL_FreeSurface(s);
    
    currentAtlas = newAtlasHandle;

    return newAtlasHandle;
}

void aeon_DestroyAtlas(aeon_AtlasHandle h)
{
    glDeleteTextures(1, &((atlasPool.getElement(h))->glTexture));
    atlasPool.killElement(h);
}

void aeon_SelectAtlas(aeon_AtlasHandle h)
{
    if(h != currentAtlas)
    {
        currentAtlas = h;
        aeon_Atlas* atlas = atlasPool.getElement(h);
        glBindTexture(GL_TEXTURE_2D, atlas->glTexture);
    }
}

void aeon_DrawSubImage(float x, float y, float z, float w, float h, float rot, float scale, aeon_Color color, aeon_Rect subImg)
{
    // for building the quad around the origin
    GLfloat width_over_2 = w / 2.0f;
    GLfloat height_over_2 = h / 2.0f;

    // for texture coordinates
    GLfloat u1 = subImg.upperLeft.x;
    GLfloat v1 = subImg.upperLeft.y;
    GLfloat u2 = subImg.lowerRight.x;
    GLfloat v2 = subImg.lowerRight.y;
    
    // tri 1
    vertexSetPosition(&curQuad->top.v1, width_over_2 * -1.0, height_over_2 * -1.0, z);
    vertexSetTexcoord(&curQuad->top.v1, u1, v1);
    vertexSetColor(&curQuad->top.v1, color);
    
    vertexSetPosition(&curQuad->top.v2, width_over_2, height_over_2 * -1.0, z);
    vertexSetTexcoord(&curQuad->top.v2, u2, v1);
    vertexSetColor(&curQuad->top.v2, color);
    
    vertexSetPosition(&curQuad->top.v3, width_over_2 * -1.0, height_over_2, z);
    vertexSetTexcoord(&curQuad->top.v3, u1, v2);
    vertexSetColor(&curQuad->top.v3, color);
    
    // tri 2
    vertexSetPosition(&curQuad->bottom.v1, width_over_2, height_over_2 * -1.0, z);
    vertexSetTexcoord(&curQuad->bottom.v1, u2, v1);
    vertexSetColor(&curQuad->bottom.v1, color);
    
    vertexSetPosition(&curQuad->bottom.v2, width_over_2, height_over_2, z);
    vertexSetTexcoord(&curQuad->bottom.v2, u2, v2);
    vertexSetColor(&curQuad->bottom.v2, color);
    
    vertexSetPosition(&curQuad->bottom.v3, width_over_2 * -1.0, height_over_2, z);
    vertexSetTexcoord(&curQuad->bottom.v3, u1, v2);
    vertexSetColor(&curQuad->bottom.v3, color);
    
    quadSetTransform(curQuad, x, y, rot, scale);
    
    curQuad++;
}

void printVert(vertex *v)
{
    printf("Vertex:\n");
    printf("\tx: %f\n", v->position.x);
    printf("\ty: %f\n", v->position.y);
    printf("\tz: %f\n\n", v->position.z);
    printf("\tu: %f\n", v->texcoord.u);
    printf("\tv: %f\n\n", v->texcoord.v);
    printf("\tr: %f\n", v->color.r);
    printf("\tg: %f\n", v->color.g);
    printf("\tb: %f\n", v->color.b);
    printf("\ta: %f\n\n", v->color.a);
    printf("\txform: %f, %f\n", v->transform.translation.x, v->transform.translation.y);
    printf("\trot: %f\n", v->transform.rotation);
    printf("\tscale: %f\n\n", v->transform.scale);
}

void printTri(tri* t)
{
    printVert(&t->v1);
    printVert(&t->v2);
    printVert(&t->v3);
}

void printQuad(int idx)
{
    printTri(&quadArray[idx].top);
    printTri(&quadArray[idx].bottom);
}

int vertCompare(const void* q1, const void* q2)
{
    float z1 = ((quad*)q1)->top.v1.position.z;
    float z2 = ((quad*)q2)->top.v1.position.z;
    
    if(z1 < z2) return -1;
    else if(z1 > z2) return 1;
    return 0;
}

void aeon_RenderImages(void)
{
#ifdef AEON_DEBUG
    static int average = 0;
    static int num_frames = 30;
    static int frame_cnt = 0;
    
    int start;
    int end;
    start = aeon_GetTicks();
#endif
    
    int quadCount = (int)(curQuad - quadArray);
    
    assert(quadCount < MAX_QUADS);
    
    // sort quadArray in ascending z order
    qsort((void*)quadArray, quadCount, sizeof(quad), &vertCompare);

    // render
    GLvoid* vbo_buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(vbo_buffer, quadArray, quadCount * sizeof(quad));
    glUnmapBuffer(GL_ARRAY_BUFFER);
        
    glDrawArrays(GL_TRIANGLES, 0, quadCount * 6);

    curQuad = quadArray;

#ifdef AEON_DEBUG
    glFinish();
    
    end = aeon_GetTicks();
    
    frame_cnt++;
    average += end - start;
    
    if(frame_cnt >= num_frames)
    {
        printf("render = %dms\n", average / num_frames);
        frame_cnt = 0;
        average = 0;
    }
#endif
}

int aeon_GetImageWidth(aeon_AtlasHandle imageHandle)
{
    aeon_Atlas* image = atlasPool.getElement(imageHandle);
    return image->width;
}

int aeon_GetImageHeight(aeon_AtlasHandle imageHandle)
{
    aeon_Atlas* image = atlasPool.getElement(imageHandle);
    return image->height;
}

GLuint aeon_GetShaderProgram()
{
    return shaderProgram;
}

int aeon_GetTicks(void)
{
    return SDL_GetTicks();
}

void aeon_Delay(int t)
{
    SDL_Delay(t);
}

void aeon_ShowCursor(bool show)
{
    SDL_ShowCursor((int)show);
}

void aeon_Init(int width, int height, int depth, bool fullscreen)
{
    // initialize OpenGL and SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetVideoMode(width, height, depth, SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, width, height);
    
    // initialize shaders
    
    // vertex shader
    aeon_InitVertexShader();
    const char* src = aeon_GetVertexShader();
    int status;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &src, NULL);
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        char infoLog[256];
        int l;
        glGetShaderInfoLog(vertexShader, 254, &l, infoLog);
        infoLog[255] = '\0';
        printf("\n\n%s\n\n", infoLog);
        assert(0);
    }
    
    // fragment shader
    aeon_InitFragmentShader();
    src = aeon_GetFragmentShader();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &src, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        char infoLog[256];
        int l;
        glGetShaderInfoLog(fragmentShader, 254, &l, infoLog);
        infoLog[255] = '\0';
        printf("\n\n%s\n\n", infoLog);
        assert(0);
    }
    
    // create and link shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    glBindAttribLocation(shaderProgram, vert_data, "vert_data");
    glBindAttribLocation(shaderProgram, tex_data, "tex_data");
    glBindAttribLocation(shaderProgram, color_data, "color_data");
    glBindAttribLocation(shaderProgram, xform_data, "xform_data");
    
    glLinkProgram(shaderProgram);
    
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        char infoLog[256];
        int l;
        glGetShaderInfoLog(shaderProgram, 254, &l, infoLog);
        infoLog[255] = '\0';
        printf("\n\n%s\n\n", infoLog);
        assert(0);
    }
    glUseProgram(shaderProgram);
    
    // this is the projection for geometry not affected by the camera
    float left = 0.0f;
    float right = 100.0f; // (float)width;
    float bottom = 65.0f; // (float)height;
    float top = 0.0f;
    float near = -1.0f;
    float far = 1.0f;
    
    float p[16] = {0.0f};
    p[0] = 2.0f / (right - left);
    p[5] = 2.0f / (top - bottom);
    p[10] = -2.0f / (far - near);
    p[12] = -(right + left) / (right - left);
    p[13] = -(top + bottom) / (top - bottom);
    p[14] = -(far + near) / (far - near);
    p[15] = 1.0f;

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "static_projection"), 1, false, p);
    
    aeon_CameraLookAt(0, 0, 100);
    
    // initialize the VBO
    glGenBuffers(1, &imageVBO);
    glBindBuffer(GL_ARRAY_BUFFER, imageVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad) * MAX_QUADS, NULL, GL_DYNAMIC_DRAW);
    
    // setup attributes for shader
    glVertexAttribPointer(vert_data, 3, GL_FLOAT, false, sizeof(vertex), 0); // verts
    glVertexAttribPointer(tex_data, 2, GL_FLOAT, false, sizeof(vertex), (GLvoid*)(sizeof(vertex_position))); // texture coords
    glVertexAttribPointer(color_data, 4, GL_FLOAT, false, sizeof(vertex), (GLvoid*)(sizeof(vertex_position) + sizeof(vertex_texcoord))); // color
    glVertexAttribPointer(xform_data, 4, GL_FLOAT, false, sizeof(vertex), (GLvoid*)(sizeof(vertex_position) + sizeof(vertex_texcoord) + sizeof(vertex_color))); // xform data
    
    glEnableVertexAttribArray(vert_data);
    glEnableVertexAttribArray(tex_data);
    glEnableVertexAttribArray(color_data);
    glEnableVertexAttribArray(xform_data);
}

void aeon_BeginDrawing(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void aeon_EndDrawing(void)
{
    SDL_GL_SwapBuffers();
}

void aeon_Shutdown(void)
{
    for(int i = 0; i < MAX_ATLASES; i++)
    {
        if(atlasPool.storage[i].isAlive)
        {
            aeon_DestroyAtlas(atlasPool.mangleHandle(i));
        }
    }
    
    SDL_Quit();
}

#endif