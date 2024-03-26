#ifdef _APPLE_
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>      // handle the window-managing operations
#else
#include <GL/glew.h>
#include <GL/freeglut.h>    // handle the window-managing operations
#endif
#include <stdlib.h>
#include <iostream>
#include <string>
#include <thread> //texts
#include<Windows.h>//sound
#include<mmsystem.h>   //a header file for Windows multimedia functions

using namespace std;

GLint win_width = 500,
win_hight = 500;

float transValue;

//The scenes
bool flagStart = true;
bool flagPrintFirst = false;
bool flagAns1 = false;
bool flagAns2 = false;
bool flagAns3 = false;
bool flagAns4 = false;
bool flagAns5 = false;
bool flagGameOver = false;

bool flagQ1 = false;
bool flagQ2 = false;
bool flagQ3 = false;
bool flagQ4 = false;
bool flagQ5 = false;

GLdouble PI = 3.14159265359;
GLuint myTexture1;
char image1Path[] = "C:\\6th level\\graphics\\repos\\Project500\\window.bmp";

GLuint LoadTexture(const char* filename, int width, int height)
{
    GLuint texture;
    unsigned char* data;
    FILE* file;

    //The following code will read in our RAW file

    fopen_s(&file, filename, "rb");

    if (file == NULL)
    {
        cout << "Unable to open the image file" << endl << "Program will exit :(" << endl;
        exit(0);
        return 0;
    }
    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);

    fclose(file);

    //  reorder the image colors to RGB not BGR
    for (int i = 0; i < width * height; ++i)
    {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;

    }

    glGenTextures(1, &texture);            //generate the texture with the loaded data
    glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it's array

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //set texture environment parameters

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    free(data); //free the texture array

    if (glGetError() != GL_NO_ERROR)
        printf("GLError in genTexture()\n");

    return texture; //return whether it was successfull  
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int lineAmount = 100; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    glBegin(GL_LINE_LOOP);
    for (i = 0; i <= lineAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / lineAmount)),
            y + (radius * sin(i * twicePi / lineAmount))
        );
    }
    glEnd();
}

void drawMouth(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 1.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y - (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

//Scratch's mouth
void mouthLine(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int lineAmount = 100; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 1.0f * PI;

    glBegin(GL_LINE_LOOP);
    for (i = 0; i <= lineAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / lineAmount)),
            y - (radius * sin(i * twicePi / lineAmount))
        );
    }
    glEnd();
}

void renderBitmapString(float x, float y, void* font, const char* string) { // to display string on screen
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

//The question's box
void Box() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-.6, -.3);
    glTexCoord2f(1, 0); glVertex2f(.6, -.3);
    glTexCoord2f(1, 1); glVertex2f(.6, .4);
    glTexCoord2f(0, 1); glVertex2f(-.6, .4);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//The 1st stage of the game
void FirstStage()
{
    //The sky
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.6f, 0.6f);
    glVertex2f(-1.0f, -0.81f);
    glColor3f(0.0f, 0.6f, 0.6f);
    glVertex2f(1.0f, -0.81f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    //The ground
    glBegin(GL_QUADS);
    glColor3f(0.80f, 0.5f, 0.24f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.88f);
    glVertex2f(-1.0f, -0.88f);
    glEnd();

    //The grass
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.8f, 0.35f);
    glVertex2f(-1.0f, -0.88f);
    glVertex2f(1.0f, -0.88f);
    glVertex2f(1.0f, -0.81f);
    glVertex2f(-1.0f, -0.81f);
    glEnd();

    //The grass line
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-1.0f, -0.88f);
    glVertex2f(1.0f, -0.88f);
    glVertex2f(1.0f, -0.81f);
    glVertex2f(-1.0f, -0.81f);
    glEnd();

    //The grass lines
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.75f, -0.88f);
    glVertex2f(-0.75f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.45f, -0.88f);
    glVertex2f(-0.45f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.15f, -0.88f);
    glVertex2f(-0.15f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.15f, -0.88f);
    glVertex2f(0.15f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.45f, -0.88f);
    glVertex2f(0.45f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.75f, -0.88f);
    glVertex2f(0.75f, -0.81f);
    glEnd();

    //The roots lines
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-1.0f, -0.95f);
    glVertex2f(-0.88f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.95f, -0.93f);
    glVertex2f(-0.8f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.88f, -0.97f);
    glVertex2f(-0.7f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.77f, -0.92f);
    glVertex2f(-0.6f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.66f, -0.97f);
    glVertex2f(-0.52f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.59f, -0.92f);
    glVertex2f(-0.4f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.45f, -0.98f);
    glVertex2f(-0.34f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.39f, -0.92f);
    glVertex2f(-0.2f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.25f, -0.98f);
    glVertex2f(-0.14f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.2f, -0.93f);
    glVertex2f(0.0f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.05f, -0.98f);
    glVertex2f(0.06f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.0f, -0.93f);
    glVertex2f(0.2f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.15f, -0.98f);
    glVertex2f(0.26f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.2f, -0.93f);
    glVertex2f(0.4f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.35f, -0.98f);
    glVertex2f(0.46f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.4f, -0.93f);
    glVertex2f(0.6f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.55f, -0.98f);
    glVertex2f(0.66f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.6f, -0.93f);
    glVertex2f(0.8f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.75f, -0.98f);
    glVertex2f(0.86f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.8f, -0.93f);
    glVertex2f(1.0f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.95f, -0.98f);
    glVertex2f(1.0f, -0.88f);
    glEnd();
    //End of roots lines

    //The sun
    glColor3f(0.9f, 0.9f, 0.0f);
    drawFilledCircle(-1.0, 1.0, .25);

    //The sun light
    glColor3f(0.9f, 0.9f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.75f, 0.95f);
    glVertex2f(-0.63f, 0.91f);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.80f, 0.84f);
    glVertex2f(-0.72f, 0.74f);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.90f, 0.77f);
    glVertex2f(-0.85f, 0.66f);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.98f, 0.74f);
    glVertex2f(-0.96f, 0.65f);
    glEnd();
}

//The 2nd stage of the game
void Sunset() {
    //The sky
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.2f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glColor3f(0.8f, 0.4f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    //The ground
    glBegin(GL_QUADS);
    glColor3f(0.80f, 0.5f, 0.24f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.88f);
    glVertex2f(-1.0f, -0.88f);
    glEnd();

    //The grass
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.8f, 0.35f);
    glVertex2f(-1.0f, -0.88f);
    glVertex2f(1.0f, -0.88f);
    glVertex2f(1.0f, -0.81f);
    glVertex2f(-1.0f, -0.81f);
    glEnd();

    //The grass line
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-1.0f, -0.88f);
    glVertex2f(1.0f, -0.88f);
    glVertex2f(1.0f, -0.81f);
    glVertex2f(-1.0f, -0.81f);
    glEnd();

    //The grass lines
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.75f, -0.88f);
    glVertex2f(-0.75f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.45f, -0.88f);
    glVertex2f(-0.45f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.15f, -0.88f);
    glVertex2f(-0.15f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.15f, -0.88f);
    glVertex2f(0.15f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.45f, -0.88f);
    glVertex2f(0.45f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.75f, -0.88f);
    glVertex2f(0.75f, -0.81f);
    glEnd();

    //The roots lines
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-1.0f, -0.95f);
    glVertex2f(-0.88f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.95f, -0.93f);
    glVertex2f(-0.8f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.88f, -0.97f);
    glVertex2f(-0.7f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.77f, -0.92f);
    glVertex2f(-0.6f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.66f, -0.97f);
    glVertex2f(-0.52f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.59f, -0.92f);
    glVertex2f(-0.4f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.45f, -0.98f);
    glVertex2f(-0.34f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.39f, -0.92f);
    glVertex2f(-0.2f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.25f, -0.98f);
    glVertex2f(-0.14f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.2f, -0.93f);
    glVertex2f(0.0f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.05f, -0.98f);
    glVertex2f(0.06f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.0f, -0.93f);
    glVertex2f(0.2f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.15f, -0.98f);
    glVertex2f(0.26f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.2f, -0.93f);
    glVertex2f(0.4f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.35f, -0.98f);
    glVertex2f(0.46f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.4f, -0.93f);
    glVertex2f(0.6f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.55f, -0.98f);
    glVertex2f(0.66f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.6f, -0.93f);
    glVertex2f(0.8f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.75f, -0.98f);
    glVertex2f(0.86f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.8f, -0.93f);
    glVertex2f(1.0f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.95f, -0.98f);
    glVertex2f(1.0f, -0.88f);
    glEnd();
    //End of roots lines
}

// Function to draw a star at the given coordinates
void drawStar(float x, float y) {
    glBegin(GL_TRIANGLES); // Begin drawing triangles
    glColor4f(1.0, 1.0, 0.0, 0.4); // Set color to white

    // Draw the star using triangles
    glVertex2f(x, y + 0.03);
    glVertex2f(x - 0.01, y - 0.01);
    glVertex2f(x + 0.01, y - 0.01);

    glVertex2f(x, y - 0.03);
    glVertex2f(x - 0.01, y + 0.01);
    glVertex2f(x + 0.01, y + 0.01);

    glVertex2f(x - 0.03, y);
    glVertex2f(x + 0.01, y - 0.01);
    glVertex2f(x + 0.01, y + 0.01);

    glVertex2f(x + 0.03, y);
    glVertex2f(x - 0.01, y - 0.01);
    glVertex2f(x - 0.01, y + 0.01);

    glEnd(); // End drawing
}

//The last stage of the game
void NightStage()
{
    //The sky
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.07f);
    glVertex2f(-1.0f, -0.81f);
    glColor3f(0.0f, 0.0f, 0.2f);
    glVertex2f(1.0f, -0.81f);
    glColor3f(0.0f, 0.0f, 0.02f);
    glVertex2f(1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.01f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    //stars
    drawStar(0.0f, 0.4f);
    drawStar(0.6f, -0.2f);
    drawStar(-0.7, 0.7);
    drawStar(0.4, 0.8);
    drawStar(0.9, 0.2);
    drawStar(-0.9, -0.2);
    drawStar(-0.8, 0.3);
    drawStar(0, 0);
    drawStar(-0.1, 0.9);
    drawStar(-0.5, 0.1);

    //The ground
    glBegin(GL_QUADS);
    glColor3f(0.80f, 0.5f, 0.24f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.88f);
    glVertex2f(-1.0f, -0.88f);
    glEnd();

    //The grass
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.8f, 0.35f);
    glVertex2f(-1.0f, -0.88f);
    glVertex2f(1.0f, -0.88f);
    glVertex2f(1.0f, -0.81f);
    glVertex2f(-1.0f, -0.81f);
    glEnd();

    //The grass line
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-1.0f, -0.88f);
    glVertex2f(1.0f, -0.88f);
    glVertex2f(1.0f, -0.81f);
    glVertex2f(-1.0f, -0.81f);
    glEnd();

    //The grass lines
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.75f, -0.88f);
    glVertex2f(-0.75f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.45f, -0.88f);
    glVertex2f(-0.45f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.15f, -0.88f);
    glVertex2f(-0.15f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.15f, -0.88f);
    glVertex2f(0.15f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.45f, -0.88f);
    glVertex2f(0.45f, -0.81f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.75f, -0.88f);
    glVertex2f(0.75f, -0.81f);
    glEnd();

    //The roots lines
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-1.0f, -0.95f);
    glVertex2f(-0.88f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.95f, -0.93f);
    glVertex2f(-0.8f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.88f, -0.97f);
    glVertex2f(-0.7f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.77f, -0.92f);
    glVertex2f(-0.6f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.66f, -0.97f);
    glVertex2f(-0.52f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.59f, -0.92f);
    glVertex2f(-0.4f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.45f, -0.98f);
    glVertex2f(-0.34f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.39f, -0.92f);
    glVertex2f(-0.2f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.25f, -0.98f);
    glVertex2f(-0.14f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.2f, -0.93f);
    glVertex2f(0.0f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(-0.05f, -0.98f);
    glVertex2f(0.06f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.0f, -0.93f);
    glVertex2f(0.2f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.15f, -0.98f);
    glVertex2f(0.26f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.2f, -0.93f);
    glVertex2f(0.4f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.35f, -0.98f);
    glVertex2f(0.46f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.4f, -0.93f);
    glVertex2f(0.6f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.55f, -0.98f);
    glVertex2f(0.66f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.6f, -0.93f);
    glVertex2f(0.8f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.75f, -0.98f);
    glVertex2f(0.86f, -0.88f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.8f, -0.93f);
    glVertex2f(1.0f, -1.0f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.3f, 0.15f, 0.03f);
    glVertex2f(0.95f, -0.98f);
    glVertex2f(1.0f, -0.88f);
    glEnd();
    //End of roots lines
}

// The 1st charachter: Thanos
void Thanos() {
    //right arm
    glColor3f(0.36f, 0.27f, 0.54f);
    drawFilledCircle(0.4, 0.12, 0.13);
    glBegin(GL_QUADS);
    glColor3f(0.36f, 0.27f, 0.54f);
    glVertex2f(0.49, 0.03);
    glVertex2f(0.3, 0.03);
    glVertex2f(0.3, -0.18);
    glVertex2f(0.49, -0.18);
    glEnd();

    //left arm
    glColor3f(0.36f, 0.27f, 0.54f);
    drawFilledCircle(-0.4, 0.12, 0.13);
    glBegin(GL_QUADS);
    glColor3f(0.36f, 0.27f, 0.54f);
    glVertex2f(-0.49, 0.03);
    glVertex2f(-0.3, 0.03);
    glVertex2f(-0.3, -0.15);
    glVertex2f(-0.49, -0.15);
    glEnd();

    //righ hand 'dark yellow glove'
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.8f, 0.0f);
    glVertex2f(0.49, -0.18);
    glVertex2f(0.30, -0.18);
    glVertex2f(0.30, -0.22);
    glVertex2f(0.49, -0.22);
    glEnd();

    //righ hand 'light yellow glove'
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.9f, 0.8f);
    glVertex2f(0.49, -0.22);
    glVertex2f(0.30, -0.22);
    glVertex2f(0.30, -0.26);
    glVertex2f(0.49, -0.26);
    glEnd();

    //righ hand 'dark yellow glove' after the light 
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.8f, 0.0f);
    glVertex2f(0.41, -0.475);//1
    glVertex2f(0.45, -0.475);//2
    glVertex2f(0.47, -0.475);//3
    glVertex2f(0.48, -0.47);//4
    glVertex2f(0.5, -0.46);//5
    glVertex2f(0.51, -0.45);//6
    glVertex2f(0.515, -0.44);//7
    glVertex2f(0.515, -0.36);
    glVertex2f(0.5, -0.33);
    glVertex2f(0.49, -0.31);
    glVertex2f(0.49, -0.25);
    glVertex2f(0.4, -0.25);
    glVertex2f(0.3, -0.25);
    glVertex2f(0.3, -0.475);
    glVertex2f(0.35, -0.475);
    glEnd();

    glColor3f(0.9f, 0.8f, 0.0f);
    drawFilledCircle(0.327, -0.36, 0.05);
    drawFilledCircle(0.305, -0.4, 0.05);
    drawFilledCircle(0.34, -0.465, 0.05);
    glColor3f(0.99f, 0.96f, 0.56f);
    drawFilledCircle(0.4, -0.35, 0.045);
    glColor3f(0.41f, 0.42f, 0.41f);
    drawFilledCircle(0.3, -0.39, 0.02);
    glColor3f(0.67f, 0.50f, 0.99f);
    drawFilledCircle(0.33, -0.443, 0.02);
    glColor3f(0.93f, 0.702f, 0.13f);
    drawFilledCircle(0.38, -0.443, 0.02);
    glColor3f(0.93f, 0.41f, 0.65f);
    drawFilledCircle(0.43, -0.443, 0.02);
    glColor3f(0.99f, 0.49f, 0.0f);
    drawFilledCircle(0.48, -0.443, 0.02);

    //navy sweater
    //right shoulder
    glBegin(GL_POLYGON);
    glColor3f(0.15f, 0.25f, 0.54f);
    glVertex2f(0.0, -0.17);
    glVertex2f(0.17, -0.17);
    glVertex2f(0.19, -0.14);
    glVertex2f(0.2, -0.12);
    glVertex2f(0.21, -0.1);
    glVertex2f(0.3, -0.1);
    glVertex2f(0.34, -0.07);
    glVertex2f(0.37, -0.04);
    glVertex2f(0.37, 0.2);
    glVertex2f(0.35, 0.24);
    glVertex2f(0.32, 0.27);
    glVertex2f(0.285, 0.28);
    glVertex2f(0.26, 0.27);
    glVertex2f(0.25, 0.275);
    glVertex2f(0.24, 0.29);
    glVertex2f(0.255, 0.3);
    glVertex2f(0.15, 0.3);

    //left shoulder
    glVertex2f(-0.15, 0.3);
    glVertex2f(-0.255, 0.3);
    glVertex2f(-0.24, 0.29);
    glVertex2f(-0.25, 0.275);
    glVertex2f(-0.26, 0.27);
    glVertex2f(-0.285, 0.28);
    glVertex2f(-0.32, 0.27);
    glVertex2f(-0.35, 0.24);
    glVertex2f(-0.37, 0.2);
    glVertex2f(-0.37, -0.04);
    glVertex2f(-0.34, -0.07);
    glVertex2f(-0.3, -0.1);
    glVertex2f(-0.21, -0.1);
    glVertex2f(-0.2, -0.12);
    glVertex2f(-0.19, -0.14);
    glVertex2f(-0.17, -0.17);
    glVertex2f(0.0, -0.17);
    glEnd();

    //clothes ( yellow neck)
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.8f, 0.0f);
    glVertex2f(0.0, 0.04);
    glVertex2f(0.15, 0.03);
    glVertex2f(0.2, 0.04);
    glVertex2f(0.23, 0.07);
    glVertex2f(0.23, 0.1);
    glVertex2f(0.23, 0.25);
    glVertex2f(0.19, 0.28);
    glVertex2f(0.16, 0.3);
    glVertex2f(0.12, 0.34);
    glVertex2f(0.0, 0.39);
    glVertex2f(0.0, 0.39);
    glVertex2f(-0.12, 0.34);
    glVertex2f(-0.16, 0.3);
    glVertex2f(-0.19, 0.28);
    glVertex2f(-0.23, 0.25);
    glVertex2f(-0.23, 0.1);
    glVertex2f(-0.23, 0.07);
    glVertex2f(-0.2, 0.04);
    glVertex2f(-0.15, 0.03);
    glVertex2f(0.0, 0.04);
    glEnd();

    //the yellow tie
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.8f, 0.0f);
    glVertex2f(0.04, 0.04);
    glVertex2f(0.04, -0.17);
    glVertex2f(-0.04, -0.17);
    glVertex2f(-0.04, 0.04);
    glEnd();

    //yellow belt
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.8f, 0.0f);
    glVertex2f(0.16, -0.17);
    glVertex2f(-0.16, -0.17);
    glVertex2f(-0.16, -0.225);
    glVertex2f(0.16, -0.225);
    glEnd();

    //the right yellow peice
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.8f, 0.0f);
    glVertex2f(0.4, 0.0);
    glVertex2f(0.42, 0.02);
    glVertex2f(0.42, 0.1);
    glVertex2f(0.42, 0.17);
    glVertex2f(0.42, 0.2);
    glVertex2f(0.41, 0.22);
    glVertex2f(0.39, 0.24);
    glVertex2f(0.38, 0.25);
    glVertex2f(0.35, 0.25);
    glVertex2f(0.35, 0.25);
    glVertex2f(0.36, 0.22);
    glVertex2f(0.37, 0.20);
    glVertex2f(0.37, 0.16);
    glVertex2f(0.37, 0.0);
    glVertex2f(0.42, 0.02);
    glEnd();

    //the left yellow peice
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.8f, 0.0f);
    glVertex2f(-0.4, 0.0);
    glVertex2f(-0.42, 0.02);
    glVertex2f(-0.42, 0.1);
    glVertex2f(-0.42, 0.17);
    glVertex2f(-0.42, 0.2);
    glVertex2f(-0.41, 0.22);
    glVertex2f(-0.39, 0.24);
    glVertex2f(-0.38, 0.25);
    glVertex2f(-0.35, 0.25);
    glVertex2f(-0.35, 0.25);
    glVertex2f(-0.36, 0.22);
    glVertex2f(-0.37, 0.20);
    glVertex2f(-0.37, 0.16);
    glVertex2f(-0.37, 0.0);
    glVertex2f(-0.42, 0.02);
    glEnd();

    // head
    glColor3f(0.36f, 0.27f, 0.54f);
    drawFilledCircle(0.0, 0.42, 0.13);
    glBegin(GL_POLYGON);
    glColor3f(0.36f, 0.27f, 0.54f);
    glVertex2f(0.0, 0.13);
    glVertex2f(0.1, 0.13);
    glVertex2f(0.15, 0.15);
    glVertex2f(0.17, 0.17);
    glVertex2f(0.17, 0.27);
    glVertex2f(0.14, 0.30);
    glVertex2f(0.12, 0.32);
    glVertex2f(0.11, 0.36);
    glVertex2f(0.11, 0.39);
    glVertex2f(0.05, 0.39);
    glVertex2f(-0.05, 0.39);
    glVertex2f(-0.11, 0.39);
    glVertex2f(-0.11, 0.36);
    glVertex2f(-0.12, 0.32);
    glVertex2f(-0.14, 0.30);
    glVertex2f(-0.17, 0.27);
    glVertex2f(-0.17, 0.17);
    glVertex2f(-0.15, 0.15);
    glVertex2f(-0.1, 0.13);
    glVertex2f(0.0, 0.13);
    glEnd();

    //ears
    glBegin(GL_POLYGON);
    glColor3f(0.36f, 0.27f, 0.54f);
    glVertex2f(0.14, 0.25);
    glVertex2f(0.14, 0.38);
    glVertex2f(0.12, 0.39);
    glVertex2f(0.1, 0.39);
    glVertex2f(0.05, 0.39);
    glVertex2f(-0.05, 0.39);
    glVertex2f(-0.1, 0.39);
    glVertex2f(-0.12, 0.39);
    glVertex2f(-0.14, 0.38);
    glVertex2f(-0.14, 0.25);
    glEnd();

    //eyebrows
    glLineWidth(4.0);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.02, 0.37);
    glVertex2f(0.08, 0.4);
    glVertex2f(-0.02, 0.37);
    glVertex2f(-0.08, 0.4);
    glEnd();

    //eyes
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(0.037, 0.36, 0.015);
    drawFilledCircle(0.037, 0.345, 0.015);
    drawFilledCircle(-0.037, 0.36, 0.015);
    drawFilledCircle(-0.037, 0.345, 0.015);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0.037, 0.348, 0.009);
    drawFilledCircle(-0.037, 0.348, 0.009);

    //mouth
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0, 0.25);
    glVertex2f(0.05, 0.25);
    glVertex2f(0.06, 0.3);
    glVertex2f(-0.06, 0.3);
    glVertex2f(-0.05, 0.25);
    glVertex2f(0.0, 0.25);
    glEnd();
    glLineWidth(1.2);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.055, 0.275);
    glVertex2f(-0.055, 0.275);
    glColor3f(0.34f, 0.23f, 0.52f);
    glVertex2f(0.015, 0.22);
    glVertex2f(0.015, 0.16);
    glVertex2f(0.04, 0.22);
    glVertex2f(0.04, 0.16);
    glVertex2f(-0.015, 0.22);
    glVertex2f(-0.015, 0.16);
    glVertex2f(-0.04, 0.22);
    glVertex2f(-0.04, 0.16);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(0.0, 0.25);
    glVertex2f(0.05, 0.25);
    glVertex2f(0.06, 0.3);
    glVertex2f(-0.06, 0.3);
    glVertex2f(-0.05, 0.25);
    glVertex2f(0.0, 0.25);
    glEnd();

    //legs
    glBegin(GL_POLYGON);
    glColor3f(0.15f, 0.25f, 0.54f);
    glVertex2f(0.0, -0.32);
    glVertex2f(0.12, -0.32);
    glVertex2f(0.15, -0.32);
    glVertex2f(0.15, -0.3);
    glVertex2f(0.16, -0.26);
    glVertex2f(0.16, -0.225);
    glVertex2f(0.0, -0.225);
    glVertex2f(-0.16, -0.225);
    glVertex2f(-0.16, -0.26);
    glVertex2f(-0.15, -0.3);
    glVertex2f(-0.15, -0.32);
    glVertex2f(-0.12, -0.32);
    glEnd();

    //legs
    glBegin(GL_POLYGON);
    glColor3f(0.15f, 0.25f, 0.54f);
    glVertex2f(-0.0, -0.3);
    glVertex2f(0.05, -0.3);
    glVertex2f(0.05, -0.5);
    glVertex2f(0.15, -0.5);
    glVertex2f(0.15, -0.27);
    glVertex2f(0.16, -0.23);
    glVertex2f(0.16, -0.225);
    glVertex2f(-0.16, -0.225);
    glVertex2f(-0.16, -0.23);
    glVertex2f(-0.15, -0.27);
    glVertex2f(-0.15, -0.5);
    glVertex2f(-0.05, -0.5);
    glVertex2f(-0.05, -0.3);
    glEnd();

    // left arm2
    glBegin(GL_POLYGON);
    glColor3f(0.36f, 0.27f, 0.54f);
    glVertex2f(-0.4, -0.25);
    glVertex2f(-0.12, -0.25);
    glVertex2f(-0.12, -0.08);
    glVertex2f(-0.15, -0.07);
    glVertex2f(-0.17, -0.05);
    glVertex2f(-0.18, -0.045);
    glVertex2f(-0.2, -0.05);
    glVertex2f(-0.215, -0.06);
    glVertex2f(-0.23, -0.08);
    glVertex2f(-0.49, -0.08);
    glVertex2f(-0.49, -0.15);
    glVertex2f(-0.485, -0.18);
    glVertex2f(-0.475, -0.205);
    glVertex2f(-0.45, -0.24);
    glVertex2f(-0.41, -0.245);
    glEnd();
    glColor3f(0.36f, 0.27f, 0.54f);
    drawFilledCircle(-0.12, -0.1, 0.025);
    drawFilledCircle(-0.12, -0.14, 0.025);
    drawFilledCircle(-0.12, -0.18, 0.025);
    drawFilledCircle(-0.12, -0.22, 0.025);

    //left shoes 
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.8f, 0.0f);
    glVertex2f(0.15, -0.625);
    glVertex2f(0.15, -0.45);
    glVertex2f(0.04, -0.45);
    glVertex2f(0.04, -0.625);
    glEnd();
    glColor3f(0.9f, 0.8f, 0.0f);
    drawFilledCircle(0.15, -0.60, 0.024);
    glBegin(GL_QUADS);
    glColor3f(0.54f, 0.35f, 0.16f);
    glVertex2f(0.16, -0.47);
    glVertex2f(0.16, -0.45);
    glVertex2f(0.03, -0.45);
    glVertex2f(0.03, -0.47);
    glEnd();

    //right shoes 
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.8f, 0.0f);
    glVertex2f(-0.15, -0.625);
    glVertex2f(-0.15, -0.45);
    glVertex2f(-0.04, -0.45);
    glVertex2f(-0.04, -0.625);
    glEnd();
    glColor3f(0.9f, 0.8f, 0.0f);
    drawFilledCircle(-0.15, -0.60, 0.024);
    glBegin(GL_QUADS);
    glColor3f(0.54f, 0.35f, 0.16f);
    glVertex2f(-0.16, -0.47);
    glVertex2f(-0.16, -0.45);
    glVertex2f(-0.03, -0.45);
    glVertex2f(-0.03, -0.47);
    glEnd();
}

// The 2nd charachter: Scratch
void Scratch() {
    glColor3f(0.984f, 0.65f, 0.12f);
    drawFilledCircle(0, .32, .3);

    //ears
    glBegin(GL_TRIANGLES);
    glColor3f(0.984f, 0.65f, 0.12f);
    glVertex2f(0.1f, 0.6f);
    glVertex2f(0.3f, 0.75f);
    glVertex2f(0.2f, 0.55f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.13f, 0.6f);
    glVertex2f(0.27f, 0.72f);
    glVertex2f(0.17f, 0.55f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.984f, 0.65f, 0.12f);
    glVertex2f(-0.1f, 0.6f);
    glVertex2f(-0.3f, 0.75f);
    glVertex2f(-0.2f, 0.55f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.13f, 0.6f);
    glVertex2f(-0.27f, 0.72f);
    glVertex2f(-0.17f, 0.55f);
    glEnd();

    //drawing whiskers
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    // whisker 1
    glBegin(GL_LINES);
    glVertex2f(-0.1f, 0.2f);
    glVertex2f(-0.3f, 0.25f);
    glEnd();

    // whisker 2
    glBegin(GL_LINES);
    glVertex2f(-0.1f, 0.2f);
    glVertex2f(-0.3f, 0.2f);
    glEnd();

    // whisker 3
    glBegin(GL_LINES);
    glVertex2f(0.1f, 0.2f);
    glVertex2f(0.3f, 0.25f);
    glEnd();

    // whisker 4
    glBegin(GL_LINES);
    glVertex2f(0.1f, 0.2f);
    glVertex2f(0.3f, 0.2f);
    glEnd();

    // drawing the filled circle ( right eyes)
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glScalef(1.9, 1, 1);
    glTranslatef(-0.08, 0, 0);
    drawFilledCircle(0.17, .4, 0.04);
    glPopMatrix();

    // drawing the filled circle ( left eyes)
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glScalef(1.9, 1, 1);
    glTranslatef(0.08, 0, 0);
    drawFilledCircle(-0.17, .4, .04);
    glPopMatrix();

    // drawing the filled circle ( right pupil)
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(0.17, .4, .02);
    glPopMatrix();

    // drawing the filled circle ( left pupil)
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(-0.17, .4, .02);
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);
    //right lashes
    glBegin(GL_LINES);
    glVertex2f(-0.2f, 0.435f);
    glVertex2f(-0.25f, 0.455f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-0.2f, 0.425f);
    glVertex2f(-0.28f, 0.445f);
    glEnd();

    //left lashes
    glBegin(GL_LINES);
    glVertex2f(0.2f, 0.435f);
    glVertex2f(0.25f, 0.455f);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(0.2f, 0.425f);
    glVertex2f(0.28f, 0.445f);
    glEnd();

    //left eyebrows
    glBegin(GL_LINES);
    glVertex2f(-0.1f, 0.46f);
    glVertex2f(-0.2f, 0.46f);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(0.1f, 0.46f);
    glVertex2f(0.2f, 0.46f);
    glEnd();

    // drawing the nose
    glColor3f(0.9f, 0.2f, 0.12f);
    drawFilledCircle(0.0, 0.32, .05);

    // drawing the mouth
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glScalef(1.3, 1, 1);
    glLineWidth(5.0f);
    drawMouth(0.0, 0.2, 0.07);
    glColor3f(0.0f, 0.0f, 0.0f);
    mouthLine(.0, 0.2, 0.07);
    glPopMatrix();

    //body
    glPushMatrix();
    glColor3f(0.984f, 0.65f, 0.12f);
    glScalef(0.7, 1.2, 1);
    drawFilledCircle(0.0, -0.27, .3);
    glColor3f(1, 1, 1);
    glScalef(1, 1.1, 1);
    drawFilledCircle(0.0, -0.25, .2);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawHollowCircle(0.0, -0.25, 0.2);
    glPopMatrix();

    glBegin(GL_QUADS);
    glColor3f(0.984f, 0.65f, 0.12f);
    glVertex2f(0.1, -0.1);
    glVertex2f(0.2, -0.02);
    glVertex2f(0.22, -0.3);
    glVertex2f(0.32, -0.25);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.984f, 0.65f, 0.12f);
    glVertex2f(-0.1, -0.1);
    glVertex2f(-0.2, -0.02);
    glVertex2f(-0.22, -0.3);
    glVertex2f(-0.32, -0.25);
    glEnd();

    //legs
    glPushMatrix();
    glBegin(GL_QUADS);

    glColor3f(0.984f, 0.65f, 0.12f);
    glScalef(8, 8, 1);
    glVertex2f(-0.12, -0.6);
    glVertex2f(0.0, -0.6);
    glVertex2f(0.1, -0.89);
    glVertex2f(0.19, -0.89);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.984f, 0.65f, 0.12f);
    glScalef(4, 4, 1);
    glVertex2f(0.08, -0.6);
    glScalef(4, 4, 1);
    glVertex2f(0.1, -0.63);
    glScalef(4, 4, 1);
    glVertex2f(-0.2, -0.82);
    glScalef(4, 4, 1);
    glVertex2f(-0.17, -0.92);
    glEnd();
    glPopMatrix();
}

void reshapeFun(GLint newWidth, GLint newHight)
{
    glViewport(0, 0, newWidth, newHight);
    win_width = newWidth;
    win_hight = newHight;
}

static void play(void)
{
    glutPostRedisplay();
}

//Prints the start scene
void startScene() {
    FirstStage();

    Box();

    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.35f, 0.03f, GLUT_BITMAP_TIMES_ROMAN_24, "Press \'s\' to start");
    glColor3f(1.0f, 0.0f, 0.0f);
    renderBitmapString(-0.42f, -0.1f, GLUT_BITMAP_HELVETICA_12, "Use the right and left arrow to move");
}

//Prints the first stage with the charachters
void PrintFirst() {
    FirstStage();

    glPushMatrix();
    glTranslatef(-0.3, -0.56, 0.0);
    glScalef(0.4, 0.4, 1.0);
    Thanos();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.81, -0.56, 0.0);
    glScalef(0.3, 0.3, 1.0);
    glTranslatef(transValue, 0.0, 0.0);
    Scratch();
    glPopMatrix();
}

//Display the 1st question 
void Ques1() {
    PrintFirst();
    Box();

    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.53f, 0.18f, GLUT_BITMAP_HELVETICA_12, "The process of converting an object from vector ");
    renderBitmapString(-0.25f, 0.1f, GLUT_BITMAP_HELVETICA_12, "format to raster format:");
    renderBitmapString(-0.55f, -0.1f, GLUT_BITMAP_HELVETICA_12, "a) Shading    b) Transparency   c) Rasterization");
}

//Displayed after the user gets the correct answer of the 1st question 
void Ans1() {
    FirstStage();

    glPushMatrix();
    glTranslatef(-0.12, -0.56, 0.0);
    glScalef(0.4, 0.4, 1.0);
    Thanos();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.81, -0.56, 0.0);
    glScalef(0.3, 0.3, 1.0);
    glTranslatef(transValue, 0.0, 0.0);
    Scratch();
    glPopMatrix();

    glColor3f(0.41f, 0.42f, 0.41f);
    drawFilledCircle(0.8, 0.88, 0.04);
}

//Display the 2nd question 
void Ques2() {
    FirstStage();

    glPushMatrix();
    glTranslatef(-0.12, -0.56, 0.0);
    glScalef(0.4, 0.4, 1.0);
    Thanos();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.81, -0.56, 0.0);
    glScalef(0.3, 0.3, 1.0);
    glTranslatef(transValue, 0.0, 0.0);
    Scratch();
    glPopMatrix();

    glColor3f(0.41f, 0.42f, 0.41f);
    drawFilledCircle(0.8, 0.88, 0.04);

    Box();

    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.53f, 0.18f, GLUT_BITMAP_HELVETICA_12, "Non-Emissive displays are constructed by filling");
    renderBitmapString(-0.26f, 0.1f, GLUT_BITMAP_HELVETICA_12, "the glass plates with: ");
    renderBitmapString(-0.4f, -0.1f, GLUT_BITMAP_HELVETICA_12, "a) Neon   b) Phosphor   c) Pixel");

}

//Displayed after the user gets the correct answer of the 2nd question 
void Ans2() {
    Sunset();

    glPushMatrix();
    glTranslatef(0.16, -0.56, 0.0);
    glScalef(0.4, 0.4, 1.0);
    Thanos();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.81, -0.56, 0.0);
    glScalef(0.3, 0.3, 1.0);
    glTranslatef(transValue, 0.0, 0.0);
    Scratch();
    glPopMatrix();

    glColor3f(0.41f, 0.42f, 0.41f);
    drawFilledCircle(0.8, 0.88, 0.04);
    glColor3f(0.67f, 0.50f, 0.99f);
    drawFilledCircle(0.7, 0.88, 0.04);
}

//Display the 3rd question 
void Ques3() {
    Sunset();

    glPushMatrix();
    glTranslatef(0.16, -0.56, 0.0);
    glScalef(0.4, 0.4, 1.0);
    Thanos();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.81, -0.56, 0.0);
    glScalef(0.3, 0.3, 1.0);
    glTranslatef(transValue, 0.0, 0.0);
    Scratch();
    glPopMatrix();

    glColor3f(0.41f, 0.42f, 0.41f);
    drawFilledCircle(0.8, 0.88, 0.04);
    glColor3f(0.67f, 0.50f, 0.99f);
    drawFilledCircle(0.7, 0.88, 0.04);

    Box();

    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.5f, 0.18f, GLUT_BITMAP_HELVETICA_12, "A large clipping window map to veiwport will");
    renderBitmapString(-0.2f, 0.1f, GLUT_BITMAP_HELVETICA_12, "give the effect of:");
    renderBitmapString(-0.34f, -0.1f, GLUT_BITMAP_HELVETICA_12, "a) Zoom out     b) Zooom in ");

}

//Displayed after the user gets the correct answer of the 3rd question 
void Ans3() {
    Sunset();

    glPushMatrix();
    glTranslatef(0.37, -0.56, 0.0);
    glScalef(0.4, 0.4, 1.0);
    Thanos();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.81, -0.56, 0.0);
    glScalef(0.3, 0.3, 1.0);
    glTranslatef(transValue, 0.0, 0.0);
    Scratch();
    glPopMatrix();

    glColor3f(0.41f, 0.42f, 0.41f);
    drawFilledCircle(0.8, 0.88, 0.04);
    glColor3f(0.67f, 0.50f, 0.99f);
    drawFilledCircle(0.7, 0.88, 0.04);
    glColor3f(0.93f, 0.702f, 0.13f);
    drawFilledCircle(0.6, 0.88, 0.04);
}

//Display the 4th question 
void Ques4() {
    Sunset();

    glPushMatrix();
    glTranslatef(0.37, -0.56, 0.0);
    glScalef(0.4, 0.4, 1.0);
    Thanos();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.81, -0.56, 0.0);
    glScalef(0.3, 0.3, 1.0);
    glTranslatef(transValue, 0.0, 0.0);
    Scratch();
    glPopMatrix();

    glColor3f(0.41f, 0.42f, 0.41f);
    drawFilledCircle(0.8, 0.88, 0.04);
    glColor3f(0.67f, 0.50f, 0.99f);
    drawFilledCircle(0.7, 0.88, 0.04);
    glColor3f(0.93f, 0.702f, 0.13f);
    drawFilledCircle(0.6, 0.88, 0.04);

    Box();

    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.5f, 0.18f, GLUT_BITMAP_HELVETICA_12, "The intersection position of the rotation with");
    renderBitmapString(-0.13f, 0.1f, GLUT_BITMAP_HELVETICA_12, "xy-plane:");
    renderBitmapString(-0.53f, -0.1f, GLUT_BITMAP_HELVETICA_12, "a) Rotaion point    b) Pivot point    c) Fixid point");

}

//Displayed after the user gets the correct answer of the 4th question 
void Ans4() {
    NightStage();

    glPushMatrix();
    glTranslatef(0.6, -0.56, 0.0);
    glScalef(0.4, 0.4, 1.0);
    Thanos();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.81, -0.56, 0.0);
    glScalef(0.3, 0.3, 1.0);
    glTranslatef(transValue, 0.0, 0.0);
    Scratch();
    glPopMatrix();

    glColor3f(0.41f, 0.42f, 0.41f);
    drawFilledCircle(0.8, 0.88, 0.04);
    glColor3f(0.67f, 0.50f, 0.99f);
    drawFilledCircle(0.7, 0.88, 0.04);
    glColor3f(0.93f, 0.702f, 0.13f);
    drawFilledCircle(0.6, 0.88, 0.04);
    glColor3f(0.93f, 0.41f, 0.65f);
    drawFilledCircle(0.5, 0.88, 0.04);
}

//Display the 5th question 
void Ques5() {
    NightStage();

    glPushMatrix();
    glTranslatef(0.6, -0.56, 0.0);
    glScalef(0.4, 0.4, 1.0);
    Thanos();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.81, -0.56, 0.0);
    glScalef(0.3, 0.3, 1.0);
    glTranslatef(transValue, 0.0, 0.0);
    Scratch();
    glPopMatrix();

    glColor3f(0.41f, 0.42f, 0.41f);
    drawFilledCircle(0.8, 0.88, 0.04);
    glColor3f(0.67f, 0.50f, 0.99f);
    drawFilledCircle(0.7, 0.88, 0.04);
    glColor3f(0.93f, 0.702f, 0.13f);
    drawFilledCircle(0.6, 0.88, 0.04);
    glColor3f(0.93f, 0.41f, 0.65f);
    drawFilledCircle(0.5, 0.88, 0.04);

    Box();

    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.32f, 0.1f, GLUT_BITMAP_HELVETICA_12, "The cross product is not:");
    renderBitmapString(-0.55f, -0.1f, GLUT_BITMAP_HELVETICA_12, "a) Distributive    b) Commutative    c) Associative");
}

//Displayed after the user gets the correct answer of the 5th question (Congrats box)
void Ans5() {
    PlaySound(TEXT("tada-fanfare-a-6313.wav"), NULL, SND_SYNC); //Cheering sound
    NightStage();

    glPushMatrix();
    glTranslatef(-0.81, -0.53, 0.0);
    glScalef(0.4, 0.4, 1.0);
    glTranslatef(transValue, 0.0, 0.0);
    Scratch();
    glPopMatrix();


    glColor3f(0.41f, 0.42f, 0.41f);
    drawFilledCircle(0.8, 0.88, 0.04);
    glColor3f(0.67f, 0.50f, 0.99f);
    drawFilledCircle(0.7, 0.88, 0.04);
    glColor3f(0.93f, 0.702f, 0.13f);
    drawFilledCircle(0.6, 0.88, 0.04);
    glColor3f(0.93f, 0.41f, 0.65f);
    drawFilledCircle(0.5, 0.88, 0.04);
    glColor3f(0.99f, 0.49f, 0.0f);
    drawFilledCircle(0.4, 0.88, 0.04);

    Box();

    glColor3f(1.0f, 0.0f, 0.0f);
    renderBitmapString(-0.2f, 0.006f, GLUT_BITMAP_TIMES_ROMAN_24, "Congrats");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.35f, -0.07f, GLUT_BITMAP_HELVETICA_12, "You gathered the five stones!!");
    glColor3f(1.0f, 0.0f, 0.0f);
    renderBitmapString(-0.57f, -0.221f, GLUT_BITMAP_HELVETICA_12, "Press 1 to restart");
    renderBitmapString(0.2f, -0.221f, GLUT_BITMAP_HELVETICA_12, "Press 2 to exit");
}

//Displayed if the user gets the answer wrong
void GameOver() {
    PlaySound(TEXT("mixkit-sad-game-over-trombone-471.wav"), NULL, SND_SYNC); //Loosing sound
    NightStage();

    glPushMatrix();
    glTranslatef(0.6, -0.56, 0.0);
    glScalef(0.5, 0.5, 1.0);
    Thanos();
    glPopMatrix();

    Box();

    glColor3f(1.0f, 0.0f, 0.0f);
    renderBitmapString(-0.25f, 0.1f, GLUT_BITMAP_TIMES_ROMAN_24, "Game over");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.5f, -0.1f, GLUT_BITMAP_HELVETICA_12, "Press 1 to restart");
    renderBitmapString(0.1f, -0.1f, GLUT_BITMAP_HELVETICA_12, "Press 2 to exit");
}

static void special(int key, int x, int y)
{
    //handle special keys
    switch (key) {

    case GLUT_KEY_LEFT:
        transValue -= .3;
        cout << " translate x:" << transValue << endl;
        break;

    case GLUT_KEY_RIGHT:
        transValue += .3;
        cout << " translate x:" << transValue << endl;

        if (transValue == 0.6f && flagPrintFirst == true) {  //Displays the 1st question if the transvalue of Scratch = 0.6
            flagPrintFirst = false;
            flagQ1 = true;
        }

        if (transValue == 1.2f && flagAns1 == true) {
            PlaySound(TEXT("achive-sound-132273.wav"), NULL, SND_SYNC); //SOUND OF SCORE 1
        }

        if (transValue == 1.5f && flagAns1 == true) {  //Displays the 2nd question if the transvalue of Scratch = 1.5
            flagPrintFirst = false;
            flagQ1 = false;
            flagAns1 = false;
            flagQ2 = true;
        }

        if (transValue == 2.1f && flagAns2 == true) {  //SOUND OF SCORE 2
            PlaySound(TEXT("achive-sound-132273.wav"), NULL, SND_SYNC);
        }

        if (transValue == 2.7f && flagAns2 == true) {  //Displays the 3rd question if the transvalue of Scratch = 2.7
            flagPrintFirst = false;
            flagQ1 = false;
            flagAns1 = false;
            flagQ2 = false;
            flagAns2 = false;
            flagQ3 = true;
        }

        if (transValue == 3.0f && flagAns3 == true) { //SOUND OF SCORE 3
            PlaySound(TEXT("achive-sound-132273.wav"), NULL, SND_SYNC);
        }

        if (flagAns3 == true && transValue == 3.3f) { //Displays the 4th question if the transvalue of Scratch = 3.3
            flagPrintFirst = false;
            flagAns2 = false;
            flagQ3 = false;
            flagAns3 = false;
            flagQ4 = true;
        }

        if (transValue == 3.6f && flagAns4 == true) {  //SOUND OF SCORE 4
            PlaySound(TEXT("achive-sound-132273.wav"), NULL, SND_SYNC);
        }

        if (transValue == 4.2f && flagAns4 == true) { //Displays the last question if the transvalue of Scratch = 4.2
            flagPrintFirst = false;
            flagQ1 = false;
            flagAns1 = false;
            flagQ2 = false;
            flagAns2 = false;
            flagQ3 = false;
            flagAns3 = false;
            flagQ4 = false;
            flagAns4 = false;
            flagQ5 = true;
        }
        break;

    default:
        break;
    }
}

static void key(unsigned char keyPressed, int x, int y) // key handling
{
    switch (keyPressed) {

    case'1': //option for the user to repeat the game
        if (flagGameOver == true) {
            flagStart = true;
            transValue = 0;
        }
        if (flagAns5 == true) {
            flagStart = true;
            transValue = 0;
        }
        break;

    case 'S': //To start the game
    case 's':
        if (flagStart == true) {
            flagStart = false;
            flagPrintFirst = true;
        }
        break;

        // Answer options for questions
    case 'a':
        if (flagQ2 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ2 = false;
            flagAns1 = false;
            flagGameOver = true;
        }
        if (flagQ1 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ1 = false;
            flagGameOver = true;
        }
        if (flagQ3 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ1 = false;
            flagQ2 = false;
            flagAns1 = false;
            flagAns2 = false;
            flagQ3 = false;
            flagAns3 = true;
        }
        if (flagQ4 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ2 = false;
            flagAns1 = false;
            flagAns2 = false;
            flagQ3 = false;
            flagAns3 = false;
            flagQ4 = false;
            flagGameOver = true;
        }
        if (flagQ5 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ2 = false;
            flagAns1 = false;
            flagAns2 = false;
            flagQ3 = false;
            flagAns3 = false;
            flagQ4 = false;
            flagQ5 = false;
            flagGameOver = true;
        }
        break;

    case'b':
        if (flagQ1 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ1 = false;
            flagGameOver = true;
        }
        if (flagQ2 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ2 = false;
            flagAns1 = false;
            flagAns2 = true;
        }
        if (flagQ3 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ2 = false;
            flagAns1 = false;
            flagAns2 = false;
            flagQ3 = false;
            flagGameOver = true;
        }
        if (flagQ4 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ2 = false;
            flagAns1 = false;
            flagAns2 = false;
            flagQ3 = false;
            flagAns3 = false;
            flagQ4 = false;
            flagAns4 = true;
        }
        if (flagQ5 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ2 = false;
            flagAns1 = false;
            flagAns2 = false;
            flagQ3 = false;
            flagAns3 = false;
            flagQ4 = false;
            flagQ5 = false;
            flagGameOver = true;
        }
        break;

    case 'c':
        if (flagQ1 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ1 = false;
            flagAns1 = true;
        }
        if (flagQ2 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ2 = false;
            flagAns1 = false;
            flagGameOver = true;
        }
        if (flagQ4 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ2 = false;
            flagAns1 = false;
            flagAns2 = false;
            flagQ3 = false;
            flagAns3 = false;
            flagQ4 = false;
            flagGameOver = true;
        }
        if (flagQ5 == true) {
            flagStart = false;
            flagPrintFirst = false;
            flagQ2 = false;
            flagAns1 = false;
            flagAns2 = false;
            flagQ3 = false;
            flagAns3 = false;
            flagQ4 = false;
            flagAns4 = false;
            flagQ5 = false;
            flagAns5 = true;
        }
        break;

    case '2': // To exit the game
        exit(0);
        break;

    default:
        fprintf(stderr, "\nYou entered a wrong letter / number\n\n"
            "Try again\n");
        break;
    }
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.4f); // Set background color 

    myTexture1 = LoadTexture(image1Path, 324, 344);

    if (myTexture1 == -1)
    {
        cout << "Error in loading the texture" << endl;
    }
    else
        cout << "The texture value is: " << myTexture1 << endl;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (flagStart) {
        startScene();
    }
    else if (flagPrintFirst) {
        PrintFirst();
    }
    else if (flagQ1) {
        Ques1();
    }
    else if (flagAns1) {
        Ans1();
    }
    else if (flagQ2) {
        Ques2();
    }
    else if (flagAns2) {
        Ans2();
    }
    else if (flagQ3) {
        Ques3();
    }
    else if (flagAns3) {
        Ans3();
    }
    else if (flagQ4) {
        Ques4();
    }
    else if (flagAns4) {
        Ans4();
    }
    else if (flagQ5) {
        Ques5();
    }
    else if (flagAns5) {
        Ans5();
    }
    else if (flagGameOver) {

        GameOver();
    }
    else {
        startScene();
    }

    glDisable(GL_BLEND);
    glFlush();
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);                          // Initialize GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);     //  Specify the display Mode   RGB, RGBA or color //  Index, single or double Buffer
    glutInitWindowSize(win_width, win_hight);       // Set the window's initial width & height
    glutInitWindowPosition(50, 50);                 // Position the window's initial top-left corner
    glutCreateWindow("");    // Create a window with the given title
    init();

    transValue = 0.0;

    glutDisplayFunc(display);                        // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeFun);
    glutIdleFunc(play);
    glutKeyboardFunc(key);
    glutSpecialFunc(special);
    glutMainLoop();                                  // Enter the infinitely event-processing loop
    return 0;
}