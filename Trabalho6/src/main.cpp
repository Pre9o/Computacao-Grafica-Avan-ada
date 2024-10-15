#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include "glsl.h"
#include "MyGL.h"

// Uniform variables
GLint loc_u_resolution;
GLint loc_u_center;
GLint loc_u_zoom;
GLint loc_iterations;

Glsl *shader = NULL;
MyGL *gl = NULL;

int iterations = 512;

float zoom = 200.0f;
float centerX = 0.0f;
float centerY = 0.0f;
float maxZoom = 40000000.0f;

bool isDragging = false;
float lastMouseX, lastMouseY;

void init_gl(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    shader->setActive(true);

    glUniform2f(loc_u_resolution, 800.0f, 800.0f);
    glUniform2f(loc_u_center, centerX, centerY);
    glUniform1f(loc_u_zoom, zoom);
    glUniform1i(loc_iterations, iterations);

    printf("centerX: %f, centerY: %f, zoom: %f, iterations: %d\n", centerX, centerY, zoom, iterations);


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
    if(zoom < maxZoom){
        zoom *= 1.01f;
    }
    glutPostRedisplay(); 
    glutTimerFunc(32, timer, 0); 
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
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if(state == GLUT_UP){
            isDragging = false;
        }
    }
}

void motion(int x, int y)
{
    if(isDragging){
        float deltaX = (x - lastMouseX) / zoom;
        float deltaY = (lastMouseY - y) / zoom;

        centerX -= deltaX;
        centerY -= deltaY;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    if(key == '='){
        zoom *= 1.1f;
    }
    else if(key == '-'){
        zoom /= 1.1f;
    }
    else if(key == 'r'){
        zoom = 200.0f;
        centerX = 0.0f;
        centerY = 0.0f;
    }
    else if(key == 'd' && iterations < 5096){
        iterations *= 2;
    }
    else if(key == 'a' && iterations > 1){
        iterations /= 2;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mandelbrot Fractal");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glewInit();

    shader = new Glsl("glsl_1_Fixed_programmable\\src\\mandelbrot.vert", "glsl_1_Fixed_programmable\\src\\mandelbrot.frag");

    loc_u_resolution = shader->getUniformLoc("u_resolution");
    loc_u_center = shader->getUniformLoc("u_center");
    loc_u_zoom = shader->getUniformLoc("u_zoom");
    loc_iterations = shader->getUniformLoc("iterations");

    init_gl();
    glutTimerFunc(0, timer, 0); 
    glutMainLoop();
    return 0;
}