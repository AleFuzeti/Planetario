//  gcc planetario.c -lGL -lglut -lm -lGLU -o planetario && ./planetario
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

float angle = 0.0;
float rotation = 0.0;
float cameraAngleX = 0.0;
float cameraAngleY = 0.0;
float cameraDistance = 50.0;
bool day = true;

GLuint loadTexture(const char *filename) {
    GLuint tex;
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);
    return tex;
}

GLuint backgroundTexture; // Declare the background texture
GLuint sunTexture, mercuryTexture, venusTexture, earthTexture, moonTexture, marsTexture, jupiterTexture, saturnTexture, uranusTexture, neptuneTexture, satRingTexture,sunTexture2,sunAuxTexture;

void init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // fundo preto
    glEnable(GL_DEPTH_TEST);   // deixa ter profundidade
    glEnable(GL_LIGHTING);        // deixa ter luz difusa
    glEnable(GL_LIGHT0);            // deixa ter luz
    glEnable(GL_COLOR_MATERIAL); // deixa botar cor
    glEnable(GL_TEXTURE_2D);        // deixa ter textura
    backgroundTexture = loadTexture("textures/fundo2.jpg");
    sunTexture = loadTexture("textures/sun.jpg");
    sunTexture2 = loadTexture("textures/sun2.jpg");
    mercuryTexture = loadTexture("textures/mercury.jpg");
    venusTexture = loadTexture("textures/venus.jpg");
    earthTexture = loadTexture("textures/earth.jpg");
    moonTexture = loadTexture("textures/moon.jpg");
    marsTexture = loadTexture("textures/mars.jpg");
    jupiterTexture = loadTexture("textures/jupiter.jpg");
    saturnTexture = loadTexture("textures/saturn.jpg");
    uranusTexture = loadTexture("textures/uranus.jpg");
    neptuneTexture = loadTexture("textures/neptune.jpg");
    satRingTexture = loadTexture("textures/saturnRing.jpg");
    sunAuxTexture = sunTexture;
}

void drawSphere(float radius) { 
    GLUquadric *quad;
    quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluSphere(quad, radius, 50, 50);
    gluDeleteQuadric(quad);
}

void drawRings(float innerRadius, float outerRadius) { 
    GLUquadric *quad;
    quad = gluNewQuadric();
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0); // Inclina o disco para ficar horizontal
    gluDisk(quad, innerRadius, outerRadius, 50, 1);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void drawBackground() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.0, 0.3, 1.0);
    glDisable(GL_DEPTH_TEST); // Desabilitar teste de profundidade
    glDisable(GL_LIGHTING);   // Desabilitar iluminação
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    // Rotacionar a textura 
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 0.0); glVertex2f(1.0, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex2f(0.0, 1.0); 
    glTexCoord2f(0.0, 1.0); glVertex2f(0.0, 0.0); 
    glTexCoord2f(0.0, 0.0); glVertex2f(1.0, 0.0); 
    glEnd();
    glColor3f(1.0, 1.0, 1.0);

    glEnable(GL_LIGHTING);    // Habilitar iluminação
    glEnable(GL_DEPTH_TEST);  // Habilitar teste de profundidade

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawBackground();
    glLoadIdentity();

    // Calcula a posição da câmera com base nos ângulos
    float eyeX = cameraDistance * cos(cameraAngleY) * cos(cameraAngleX);
    float eyeY = cameraDistance * sin(cameraAngleX);
    float eyeZ = cameraDistance * sin(cameraAngleY) * cos(cameraAngleX);
 
    gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // Cor da luz difusa
    GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 }; // Luz no centro do Sol
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 }; // Cor da luz ambiente
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    // Desenha o Sol
    glPushMatrix();
        // Iluminação do Sol
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = { 50.0 };
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        
        glPushMatrix();
            glRotatef(angle*0.9, 0.0, 1.0, 0.0);
            glBindTexture(GL_TEXTURE_2D, sunAuxTexture);
            drawSphere(5.0);
        glPopMatrix();

        GLfloat light_ambient_escuro[] = { 0.0, 0.0, 0.0, 1.0 }; // Cor da luz ambiente
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient_escuro);

        // Desenha Mercúrio
        glPushMatrix();
            glRotatef(angle * 4.15, 0.0, 1.0, 0.0);
            glTranslatef(6.0, 0.0, 0.0);
            glPushMatrix(); 
                glRotatef(rotation* 50, 0.0, 1.0, 0.0);
                glBindTexture(GL_TEXTURE_2D, mercuryTexture);
                drawSphere(0.38);
            glPopMatrix();
        glPopMatrix();
        drawRings(6.0, 6.08);

        // Desenha Vênus
        glPushMatrix();
            glRotatef(angle * 1.62, 0.0, 1.0, 0.0);
            glTranslatef(8.0, 0.0, 0.0);
            glPushMatrix(); 
                glRotatef(rotation*-0.9, 0.0, 1.0, 0.0);
                glBindTexture(GL_TEXTURE_2D, venusTexture);
                drawSphere(0.85);
            glPopMatrix();
        glPopMatrix();
        drawRings(8.0, 8.08);

        // Desenha a Terra
        glPushMatrix();
            glRotatef(angle, 0.0, 1.0, 0.0);
            glTranslatef(10, 0.0, 0.0);
            glPushMatrix(); 
                glRotatef(rotation, 0.0, 1.0, 0.0);
                glBindTexture(GL_TEXTURE_2D, earthTexture);
                drawSphere(1.0);
            glPopMatrix();

            // Desenha a Lua
            glPushMatrix();
                glRotatef(rotation, 0.0, 1.0, 0.0);
                glTranslatef(1.5, 0.0, 0.0);
                glBindTexture(GL_TEXTURE_2D, moonTexture);
                drawSphere(0.3);
            glPopMatrix();
            drawRings(1.5, 1.55);

        glPopMatrix();
        drawRings(10.0, 10.08);
        
        // Desenha Marte
        glPushMatrix();
            glRotatef(angle * 0.53, 0.0, 1.0, 0.0);
            glTranslatef(13.0, 0.0, 0.0);
            glPushMatrix(); 
                glRotatef(rotation*3, 0.0, 1.0, 0.0);
                glBindTexture(GL_TEXTURE_2D, marsTexture);
                drawSphere(0.53);
            glPopMatrix();
        glPopMatrix();
        drawRings(13.0, 13.08);

        // Desenha Júpiter
        glPushMatrix();
            glRotatef(angle * 0.3, 0.0, 1.0, 0.0);
            glTranslatef(17.0, 0.0, 0.0);
            glPushMatrix(); 
                glRotatef(angle*7, 0.0, 1.0, 0.0);
                glBindTexture(GL_TEXTURE_2D, jupiterTexture);
                drawSphere(1.85);
            glPopMatrix();
        glPopMatrix();
        drawRings(17.0, 17.08);

        // Desenha Saturno
        glPushMatrix();
            glRotatef(angle * 0.2, 0.0, 1.0, 0.0);
            glTranslatef(23.0, 0.0, 0.0);
            glPushMatrix();
                glRotatef(angle*7, 0.0, 1.0, 0.0);
                glBindTexture(GL_TEXTURE_2D, saturnTexture);
                drawSphere(1.55);
                glPushMatrix();
                    glBindTexture(GL_TEXTURE_2D, satRingTexture); // Bind da textura dos anéis de Saturno
                    drawRings(2.0, 2.5); // Desenha os anéis de Saturno com a textura correta
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
        drawRings(23.0, 23.08);

        // Desenha Urano
        glPushMatrix();
            glRotatef(angle * 0.16, 0.0, 1.0, 0.0);
            glTranslatef(27.0, 0.0, 0.0);
            glPushMatrix();
                glRotatef(angle * 12, 0.0, 1.0, 0.0);
                glBindTexture(GL_TEXTURE_2D, uranusTexture);
                drawSphere(1.2);
                drawRings(1.6, 1.8);
                drawRings(1.9, 2.1);
            glPopMatrix();      
        glPopMatrix();
        drawRings(27.0, 27.08);

        // Desenha Netuno
        glPushMatrix();
            glRotatef(angle * 0.1, 0.0, 1.0, 0.0);
            glTranslatef(33.0, 0.0, 0.0);
            glPushMatrix();
                glRotatef(angle * 6, 0.0, 1.0, 0.0);
                glBindTexture(GL_TEXTURE_2D, neptuneTexture);
                drawSphere(1.38);
            glPopMatrix();
        glPopMatrix();
        drawRings(33.0, 33.08);

    glPopMatrix();
    
    glutSwapBuffers();
    //glColor3f(1.0, 0.8, 0.6); MUDAR COR
}

void idleFunc() {
    angle += 0.5;
    rotation +=1;
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    float angleStep = 0.25;
    float distanceStep = 1.0;

    switch (key) {
        case GLUT_KEY_LEFT:
            cameraAngleY -= angleStep;
            break;
        case GLUT_KEY_RIGHT:
            cameraAngleY += angleStep;
            break;
        case GLUT_KEY_UP:
            cameraAngleX -= angleStep;
            break;
        case GLUT_KEY_DOWN:
            cameraAngleX += angleStep;
            break;
        case GLUT_KEY_PAGE_UP:
            angle += distanceStep*5;
            break;
        case GLUT_KEY_PAGE_DOWN:
            angle -= distanceStep*5;
            break;
        case GLUT_KEY_HOME:
            cameraDistance -= distanceStep;
            if (cameraDistance < 6.0) cameraDistance = 50.0; // Evitar câmera dentro do objeto
            break;
        case GLUT_KEY_END:
            cameraDistance += distanceStep;
            if (cameraDistance > 100.0) cameraDistance = 50.0; // Evitar câmera dentro do objeto
            break;
        case GLUT_KEY_INSERT:
            if (day){
                sunAuxTexture = sunTexture2;
                day = false;
            } else {
                sunAuxTexture = sunTexture;
                day = true;
            }
            break;

    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cameraDistance, (float)w / (float)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Planetario do Balacobaco");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idleFunc);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
