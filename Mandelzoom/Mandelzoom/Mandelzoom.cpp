
#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include <cfloat>

using namespace std;

#define INITIAL_WIN_W 800
#define INITIAL_WIN_H 800
#define INITIAL_WIN_X 150
#define INITIAL_WIN_Y 50


// void methodName(Params)

// How to Draw Points:
//glBegin( GL_POINTS );
//  for (int i = 0; i<125; i++) glVertex2i(320, 200 + i);
//  for (int i = 0; i<60; i++) glVertex2i(320 + i, 325);
//  for (int i = 0; i<125; i++) glVertex2i(380, 200 + i);
//  glEnd();
// glFlush(); --> remember at the end

void reshape(int w, int h)
// Callback for processing reshape events.
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h);
}

void escExit(GLubyte key, int, int)
// Callback for processing keyboard events.
{
	if (key == 27 /* ESC */) std::exit(0);
}

// SAMPLE MAIN: Learning C++
int main(){
	int apples; 
	apples = 5;

	cout << "There are " << apples << " apples." << endl;

	apples = apples - 1;

	cout << "If I take one away, there will be " << apples << " apples." << endl;
	
	// Printing Hello World
	std::cout << "Hello World" << std::endl;

	// retains screen
	std::cin.get();

	// Mask floating point exceptions.
	_control87(MCW_EM, MCW_EM);

	// Choose RGB display mode for normal screen window.
	glutInitDisplayMode(GLUT_RGB);

	// Set initial window size, position, and title.
	glutInitWindowSize(INITIAL_WIN_W, INITIAL_WIN_H);
	glutInitWindowPosition(INITIAL_WIN_X, INITIAL_WIN_Y);
	glutCreateWindow("Mandelzoom");

	// You don't (yet) want to know what this does.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)INITIAL_WIN_W, 0.0, (double)INITIAL_WIN_H);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the color for clearing the normal screen window.
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Set the callbacks for the normal screen window.
	glutDisplayFunc(drawHouse);

	// Set the callback for reshape events.
	glutReshapeFunc(reshape);

	// Set the callback for keyboard events.
	glutKeyboardFunc(escExit);

	// Start the GLUT main loop.
	glutMainLoop();

}