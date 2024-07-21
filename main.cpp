#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Window dimensions
const int windowWidth = 800;
const int windowHeight = 600;

// Airplane properties
float airplaneY = 0.0f;
const float airplaneSpeed = 0.05f;

// Obstacle properties
const int numObstacles = 5;
float obstacleX[numObstacles];
float obstacleY[numObstacles];
const float obstacleSize = 0.1f;
const float obstacleSpeed = 0.02f;

// Score properties
float distanceCovered = 0.0f;
bool gameOver = false;

// Function to initialize the game
void initGame() {
    srand(time(NULL));
    for (int i = 0; i < numObstacles; ++i) {
        obstacleX[i] = 1.0f + i * 0.5f;
        obstacleY[i] = (rand() % 200 - 100) / 100.0f;
    }
}

// Function to draw the airplane
void drawAirplane() {
    glPushMatrix();
    glTranslatef(-0.8f, airplaneY, 0.0f);

    // Fuselage
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glScalef(0.2f, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Wings
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.05f);
    glScalef(0.4f, 0.01f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.05f);
    glScalef(0.4f, 0.01f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Tail
    glPushMatrix();
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glScalef(0.1f, 0.05f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

// Function to draw an obstacle
void drawObstacle(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glScalef(obstacleSize, obstacleSize, obstacleSize);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Function to display the current score
void displayScore() {
    char scoreStr[50];
    sprintf(scoreStr, "Score: %.2f", distanceCovered);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.9f, 0.9f);
    for (const char* c = scoreStr; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

// Function to display the final score
void displayFinalScore() {
    char scoreStr[50];
    sprintf(scoreStr, "Game Over! Your score: %.2f", distanceCovered);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.3f, 0.0f);
    for (const char* c = scoreStr; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

// Function to display the game
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    if (!gameOver) {
        drawAirplane();
        displayScore();

        for (int i = 0; i < numObstacles; ++i) {
            drawObstacle(obstacleX[i], obstacleY[i]);
        }
    } else {
        displayFinalScore();
    }

    glutSwapBuffers();
}

// Function to update the game state
void update(int value) {
    if (!gameOver) {
        distanceCovered += obstacleSpeed;

        for (int i = 0; i < numObstacles; ++i) {
            obstacleX[i] -= obstacleSpeed;
            if (obstacleX[i] < -1.0f) {
                obstacleX[i] = 1.0f;
                obstacleY[i] = (rand() % 200 - 100) / 100.0f;
            }
        }

        for (int i = 0; i < numObstacles; ++i) {
            if (obstacleX[i] < -0.75f && obstacleX[i] > -0.85f &&
                airplaneY < obstacleY[i] + obstacleSize / 2 &&
                airplaneY > obstacleY[i] - obstacleSize / 2) {
                gameOver = true; // Collision detected, end the game
            }
        }

        glutPostRedisplay();
        glutTimerFunc(16, update, 0);
    }
}

// Function to handle keyboard input
void handleKeys(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        airplaneY += airplaneSpeed;
    } else if (key == 's' || key == 'S') {
        airplaneY -= airplaneSpeed;
    }
}

// Function to initialize OpenGL settings
void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)windowWidth / (double)windowHeight, 0.1, 100.0);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Aviator");

    initOpenGL();
    initGame();

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeys);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
