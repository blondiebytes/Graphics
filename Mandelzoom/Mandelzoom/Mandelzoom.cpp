
// COMPUTER GRAPHICS!
// Kathryn Hodge
// ------------------- MANDELZOOM --------------

#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include <cfloat>
#include <vector>
#include "Mandelbrot.h"

using namespace std;

#define MAX 1000
#define INITIAL_WIN_X 150
#define INITIAL_WIN_Y 50

// window
double win_w = 400.0;
double win_h = 400.0;

// x & y for mandelbrot set
double x_2 = 0.5;
double x_1 = -2.0;
double y_2 = 1.25;
double y_1 = -1.25;

// for rubberbanding
int xAnchor, yAnchor, xStretch, yStretch;
bool rubberBanding = false;

// For stack, holding all previous mandelbrots
vector<Mandelbrot> vecStack;
int index = 0;


// DRAW PIXEL:

void drawPixel(int u, int v, int i) {
	// Go Find the Color --> Colors a point depending on how close (or even if it is)
	// it is to being in the Mandelbrot set. 
	float r;
	float g;
	float b;

	if (0 < i && i < 10) {
		//red;
		r = 1.0;
		g = 0.0;
		b = 0.0;
	}
	else if (10 < i && i < 350) {
		//green;
		r = 0.0;
		g = 1.0;
		b = 0.0;
	}
	else if (i < 350 && i < 450) {
		//blue;
		r = 0.0;
		g = 0.0;
		b = 1.0;
	}
	else if (i < 450 && i < MAX) {
		//pink
		r = 0.9;
		g = 0.9;
		b = 0.9;
	}
	else if (i == MAX) {
		//black;
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}
	else {
		//white;
		r = 1.0;
		g = 1.0;
		b = 1.0;
	}

	// Set the Color -->
	glColor3f(r, g, b);
	// Plot point
	glVertex2i(u, v);
	
}

// // Calculate s's initial real and imaginary value
// s = sReal + (sImaginary)i
double getSReal(int u) {
	return x_1 + u * ((x_2 - x_1) / (win_w - 1.0));
}

double getSImaginary(int v) {
	return y_1 + v * ((y_2 - y_1) / (win_h - 1.0));
}

// Is a given complex number (point) in the mandelbrot set?
int isInMandelbrotSet(int u, int v) {
	// get sReal and sImaginary values
	double sReal = getSReal(u);
	double sImaginary = getSImaginary(v);
	double sRealCurrent = 0;
	double sImaginaryCurrent = 0;
	// Set iteration at 0 first
	int i = 0;
	// while the size of z is less than 2 and we aren't at the max iterations
	while (((sRealCurrent * sRealCurrent) + (sImaginaryCurrent*sImaginaryCurrent)) <= 4 && i < MAX) {
		// (Real) z^2 = (x^2 - y^2)
		// z^2 = (sReal^2 - sImaginary^2)
		// z(s)_i = (z(s)_(i-1))^2 + x
		// z(s)_i = (z(s)_(i-1))^2 + sReal
		double nSRealCurrent = ((sRealCurrent * sRealCurrent) - (sImaginaryCurrent * sImaginaryCurrent)) + sReal;
		// (Imaginary) z^2 = (2xy)i
		// z^2 = (2 * sReal * sImaginary)
		// z(s)_i = (z(s)_(i-1))^2 + y
		// z(s)_i = (z(s)_(i-1))^2 + sImaginary
		sImaginaryCurrent = 2 * sRealCurrent*sImaginaryCurrent + sImaginary;
		sRealCurrent = nSRealCurrent;
		i++;
	}
	return i;
}

// drawing the MandelbrotSet
void drawMandelbrotSet() {
	// Looping through every pixel on the screen
	// Horizontal Plane
	glBegin(GL_POINTS);
	for (int u = 0; u < win_h; u++) {
		// Vertical Plane
		for (int v = 0; v < win_w; v++) {
			// returns iteration number
			int i = isInMandelbrotSet(u, v);
			// draws pixel
			drawPixel(u, v, i);
		}
	}
	glEnd();
	glFlush();
}



// drawing the rubber band
void drawRubberBand(int xA, int yA, int xS, int yS)
{
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_XOR);
	glBegin(GL_LINES);
	glVertex2i(xA, yA);
	glVertex2i(xS, yS);
	glEnd();
	glDisable(GL_COLOR_LOGIC_OP);
	glFlush();
}

// rubberbanding callback
void rubberBand(int x, int y)
// Callback for processing mouse motion.
{
	if (rubberBanding)
	{
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		y = win_h - y;
		xStretch = x;
		yStretch = y;
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		glFlush();
	}
}

// zooming in --> pushes mandelbrot onto the stack
void Push() {
	// index > 0
	//if (index > 0) {
		// set mandel's stuff to be our stuff so that we have a saved mandelbrot
		// computation
	//	Mandelbrot mandel = vecStack.at(index++);
	//	mandel.x1 = x1;
	//	mandel.x2 = x2;
	//	mandel.y1 = y1;
	//	mandel.y2 = y2;
	//}
	//else {
		// index at 0
		// pushing it onto the stack
		Mandelbrot mandy(x_1, x_2, y_1, y_2);
		vecStack.push_back(mandy);
		index++;
//	} 
	glutPostRedisplay();
}

// pops mandelbrot off of the vector stack
void Pop() {
	// zoom out
	if (index > 0) {
		// Set it to the current view
		Mandelbrot mandel = vecStack.at(index - 1);
		x_1 = mandel.x1;
		x_2 = mandel.x2;
		y_1 = mandel.y1;
		y_2 = mandel.y2;
		index--;
	}
	else {
		return; // when there's nothing is there to pop off
	}
	glutPostRedisplay();
}



void reshape(int w, int h)
// Callback for processing reshape events.
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h);
}

// if left mouse button pressed
void processLeftDown(int x, int y)
// Function for processing mouse left botton down events.
{
	if (!rubberBanding)
	{
		int xNew = x;
		int yNew = win_h - y;
		xAnchor = xNew;
		yAnchor = yNew;
		xStretch = xNew;
		yStretch = yNew;
		rubberBanding = true;
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
	}
}


// if left mouse button released
void processLeftUp(int x, int y)
// Function for processing mouse left botton up events.
{
	if (rubberBanding)
	{
		// Setting up
		int xNew, yNew;
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		rubberBanding = false;
		xNew = x;
		yNew = win_h - y;
		
		x_1 = xAnchor;
		x_2 = xNew;
		y_1 = yAnchor;
		y_2 = yNew;

		cout << x_1;
		cout << "/";
		cout << x_2;
		cout << "/";
		cout << y_1;
		cout << "/";
		cout << y_2;
		cout << "/";


		

		// make a new mandelbrot set
		Mandelbrot mandy(x_1, x_2, y_1, y_2);
		// increase index & add malicious mandy
		index++;
		vecStack.push_back(mandy);
		
		// update
		glutPostRedisplay();
		// flush
		glFlush();
	}
}

// takes state of mouse and (u, v) coordinate
void mouse(int button, int state, int u, int v)
// Function for processing mouse events.
{
	if (button == GLUT_LEFT_BUTTON)
		switch (state)
		{
		case GLUT_DOWN: processLeftDown(u, v); break;
		case GLUT_UP: processLeftUp(u, v); break;
		}
}


// quits window
void escExit(GLubyte key, int, int)
// Callback for processing keyboard events.
{
	if (key == 27 /* ESC */) std::exit(0);
}

//callsback the menu
void mainMenu(int item)
{
	switch (item)
	{
	case 1: Push(); break;
	case 2: Pop(); break;
	case 3: std::exit(0);
	}
}

// Setting up menu items
void setMenus()
{
	glutCreateMenu(mainMenu);
	glutAddMenuEntry("Push", 1);
	glutAddMenuEntry("Pop", 2);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



// SAMPLE MAIN: Learning C++
int main(int argc, char* argv[]) 
{
	// get arguments
	x_1 = atof(argv[1]);
	x_2 = atof(argv[2]);
	y_1 = atof(argv[3]);
	y_2 = atof(argv[4]);
	win_w = atoi(argv[5]);
	win_h = atoi(argv[6]);
	Mandelbrot init(x_1, x_2, y_1, y_2);
	vecStack.push_back(init);

	// Mask floating point exceptions.
	_control87(MCW_EM, MCW_EM);

	// Choose RGB display mode for normal screen window.
	glutInitDisplayMode(GLUT_RGB);

	// Set initial window size, position, and title.
	glutInitWindowSize(win_w, win_h);
	glutInitWindowPosition(INITIAL_WIN_X, INITIAL_WIN_Y);
	glutCreateWindow("Mandelzoooom Set");

	// You don't (yet) want to know what this does.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)win_w, 0.0, (double)win_h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// This is a hack.
	glTranslatef(0.375, 0.375, 0.0);

	// Set the color for clearing the normal screen window.
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Set the callbacks for the normal screen window.
	glutDisplayFunc(drawMandelbrotSet);
	glutMouseFunc(mouse);
	glutMotionFunc(rubberBand);
	glutPassiveMotionFunc(rubberBand);

	// Set the callback for reshape events.
	glutReshapeFunc(reshape);

	// Set the callback for keyboard events.
	glutKeyboardFunc(escExit);

	// set menus
	setMenus();

	// Start the GLUT main loop.
	glutMainLoop();

}