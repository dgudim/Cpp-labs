#include <iostream>
#include <math.h>

#include <time.h>

#include <GL/glut.h>

#include "chaos_attractor.h"

double x = 0, y = 0, prevX = 0, prevY = 0,
a = -0.966918,
b = 2.879879,
c = 0.765145,
d = 0.744728;
int	initialIterations = 10,
iterations = 500000,
points = 10;

constexpr auto WIDTH = 1000;
constexpr auto HEIGHT = 1000;

int main(int argc, char** argv) {
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Strange Attractors");

	glutTimerFunc(0, timer, 0);
	glutDisplayFunc(display);
	glutKeyboardFunc(exitOnEsc);

	init();

	glutMainLoop();

	return 0;
}

void computeNextIteration() {
	prevX = x;
	prevY = y;

	double xnew = sin(y * b) + c * sin(x * b);
	double ynew = sin(x * a) + d * sin(y * a);

	x = xnew;
	y = ynew;
}

void generateNewPoint() {
	x = (rand() / (double)RAND_MAX - 0.5) / 5;
	y = (rand() / (double)RAND_MAX - 0.5) / 5;

	for (int i = 0; i < initialIterations; i++) {
		computeNextIteration();
	}
}

void init() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POINT_SMOOTH);
	glPointSize(0.5f);

	glViewport(0, 0, WIDTH, HEIGHT);

	// set up the projection matrix (the camera)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2.0f, 2.0f, -2.0f, 2.0f);

	// set up the modelview matrix (the objects)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

	for (int p = 0; p < points; p++) {
		generateNewPoint();
		for (int i = 0; i < iterations; i++) {
			computeNextIteration();
			glColor4f(i / (float)iterations, (float)(1 - abs(prevY - x) / 6), (float)(1 - abs(prevX - y) / 6), 0.02f);
			glVertex2f((float)x, (float)y);
		}
	}

	a += 0.0001;
	b += 0.0001;
	c -= 0.0001;
	d += 0.0001;

	glEnd();
	glutSwapBuffers();
}

void exitOnEsc(unsigned char mychar, int x, int y) {
	if (mychar == 27) {
		exit(0);
	}
}
