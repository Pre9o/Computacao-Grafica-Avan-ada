#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include "glsl.h"
#include "MyGL.h"

// Uniform variables
GLint loc_u_resolution;
GLint loc_u_center;
GLint loc_u_zoom;
GLint loc_iTime;

Glsl *shader = NULL;
MyGL *gl = NULL;

float zoom = 200.0f;
float centerX = 0.0f;
float centerY = 0.0f;
float time = 0.0;

void init_gl(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    shader->setActive(true);

    glUniform2f(loc_u_resolution, 800.0f, 450.0f);
    glUniform2f(loc_u_center, centerX, centerY);
    glUniform1f(loc_u_zoom, zoom);
    glUniform1f(loc_iTime, time);


    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    shader->setActive(false);

    glutSwapBuffers();
}

void timer(int value)
{
    zoom *= 1.01; // Aumenta o zoom em 1% a cada chamada
    glutPostRedisplay(); // Redesenha a cena
    glutTimerFunc(16, timer, 0); // Chama a função novamente após 16ms (~60 FPS)
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-w / 2.0f, w / 2.0f, -h / 2.0f, h / 2.0f);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        // Converte as coordenadas da tela para coordenadas do espaço de visualização
        float newX = (x - 200.0f) / zoom + centerX;
        float newY = (200.0f - y) / zoom + centerY;

        centerX = newX;
        centerY = newY;

        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == '=')
    {
        zoom *= 1.1f;
    }
    else if (key == '-')
    {
        zoom /= 1.1f;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mandelbrot Fractal");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glewInit();

    shader = new Glsl("Trabalho6\\src\\mandelbrot.vert", "Trabalho6\\src\\mandelbrot.frag");

    loc_u_resolution = shader->getUniformLoc("u_resolution");
    loc_u_center = shader->getUniformLoc("u_center");
    loc_u_zoom = shader->getUniformLoc("u_zoom");
    loc_iTime = shader->getUniformLoc("iTime");


    init_gl();
    glutTimerFunc(0, timer, 0); // Inicia o timer imediatamente
    glutMainLoop();
    return 0;
}