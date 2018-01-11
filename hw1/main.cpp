
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
//#include <nanogui/opengl.h>

#include "../shader_loader/GLShader.hpp"
#include "mickey.c"

struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f() {}

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

GLuint VBO;
GLint zoomLoc, xLoc, yLoc;
float zoom = 1, x = 0, y = 0, xdrag = 0, ydrag = 0;
GLuint texture;
const int screenSize = 800;

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1f(zoomLoc, zoom);
    glUniform1f(xLoc, x);
    glUniform1f(yLoc, y);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableVertexAttribArray(0);
    glutSwapBuffers();
}

float scale(float x) {
    return (x - screenSize / 2) / (screenSize / 2);
}

void drag(int _x, int _y) {
    x = xdrag - scale(_x);
    y = ydrag + scale(_y);
}

void mouse(int button, int state, int _x, int _y) {
    if (button == 0) {
        xdrag = scale(_x) + x;
        ydrag = -scale(_y) + y;
    }
    if (button == 3) {
        zoom *= 1.1f;
        x = ( scale(_x) + x) * 1.1f - scale(_x);
        y = (-scale(_y) + y) * 1.1f + scale(_y);
    }
    if (button == 4) {
        zoom /= 1.1f;
        x = ( scale(_x) + x) / 1.1f - scale(_x);
        y = (-scale(_y) + y) / 1.1f + scale(_y);
    }
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
    glutMouseFunc(mouse);
    glutMotionFunc(drag);
}

static void CreateVertexBuffer()
{
    Vector3f Vertices[4];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(1.0f, 1.0f, 0.0f);
    Vertices[3] = Vector3f(-1.0f, 1.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateTexture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gimp_image.width, gimp_image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, gimp_image.pixel_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

static void CompileShaders()
{
    GLuint ShaderProgram = LoadShader("shader.vert", "shader.frag");
    glUseProgram(ShaderProgram);

    zoomLoc = glGetUniformLocation(ShaderProgram, "zoom");
    xLoc = glGetUniformLocation(ShaderProgram, "x");
    yLoc = glGetUniformLocation(ShaderProgram, "y");
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(screenSize, screenSize);
    glutCreateWindow("hw1");

    InitializeGlutCallbacks();

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    CreateVertexBuffer();

    CreateTexture();

    CompileShaders();

    glutMainLoop();

    return 0;
}