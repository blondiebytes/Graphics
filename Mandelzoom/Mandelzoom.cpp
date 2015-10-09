
// COMPUTER GRAPHICS!
// Kathryn Hodge
// ------------------- MANDELZOOM --------------
// Favorite Picture @ (mandelzoom -1.921, -1.012, -0.465, 0.447, 400, 400)

#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include <cfloat>
#include <vector>
#include "Mandelbrot.h"
#include "Color.h"

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
vector<Mandelbrot> currentViewStack;
vector<Mandelbrot> savedPreviouslyViewedStack;
int currentViewStackIndex = -1;
int savedPreviouslyViewedStackIndex = -1;


// DRAW PIXEL:

void drawPixel(int u, int v, int i) {
	// Go Find the Color --> Colors a point depending on how close (or even if it is)
	// it is to being in the Mandelbrot set.
	Color c(0,0,0);

	if (0 < i && i < 10) {
		// black;
		c.r = 0.0;
		c.g = 0.0;
		c.b = 0.0;
	}
	else if (10 < i && i < 30) {
		//green
		c.r = 0;
		c.g = 0.0;
		c.b = 1.0;;
	}
	else if (i < 30 && i < 400) {
		//purple;
		c.r = 1.0;
		c.g = 0.0;
		c.b = 1.0;
		c.r = .53;
		c.g = .12;
		c.b = .47;
	}
	else if (i < 400 && i < MAX) {
		//pink
		c.r = 1.0;
		c.g = 1.0;
		c.b = 1.0;
	}
	else if (i == MAX) {
		//pink;
		c.r = 1.0;
		c.g = .11;
		c.b = .68;
	}
	else {
		//white;
		c.r = 1.0;
		c.g = 1.0;
		c.b = 1.0;
	}

	// Set the Color -->
	glColor3f(c.r, c.g, c.b);

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
	glBegin(GL_LINE_LOOP);
	glVertex2i(xA, yA);
	glVertex2i(xA, yS);
	glVertex2i(xS, yS);
	glVertex2i(xS, yA);
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
// push --> 
// 1. take top from the savedPreviouslyViewStack
// 2. put it on the currentViewStack
// 3. display new top of the currentViewStack
void Push() {
	if (savedPreviouslyViewedStackIndex >= 0) {
		//  1. take top from the savedPreviouslyViewStack
		Mandelbrot newTopOfCurrentViewStack = savedPreviouslyViewedStack.at(savedPreviouslyViewedStackIndex);
		savedPreviouslyViewedStackIndex--;
		// 2. put it on the currentViewStack
		currentViewStack.push_back(newTopOfCurrentViewStack);
		currentViewStackIndex++;
		// 3. display new top of the currentViewStack
		x_1 = newTopOfCurrentViewStack.x1;
		x_2 = newTopOfCurrentViewStack.x2;
		y_1 = newTopOfCurrentViewStack.y1;
		y_2 = newTopOfCurrentViewStack.y2;
		glutPostRedisplay();
		glFlush();
	}
	else {
		return; // nothing to show from the previously saved so just return
	}

	
}

// pop --> 
// 1. take the top from the currentViewStack
// 2. put it on the top of the savedPreviouslyViewedStack
// 3. display the new top of the currentViewStack
void Pop() {
	// zoom out
	if (currentViewStackIndex > 0) {
		// take the top from the currentViewStack
		Mandelbrot currentMandel = currentViewStack.at(currentViewStackIndex);
		currentViewStackIndex--;
		// put it on the top of the savedPreviouslyViewedStack
		savedPreviouslyViewedStack.push_back(currentMandel);
		savedPreviouslyViewedStackIndex++;
		// display the new top of the currentViewStack
		Mandelbrot newTopMandel = currentViewStack.at(currentViewStackIndex);
		x_1 = newTopMandel.x1;
		x_2 = newTopMandel.x2;
		y_1 = newTopMandel.y1;
		y_2 = newTopMandel.y2;
	}
	else {
		// when there's nothing is there to pop off, just return
		return;
	}
	glutPostRedisplay();
	glFlush();
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
		// draw the rubber band
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		rubberBanding = false;
		// converting coordinates that are upside down
		xNew = x;
		yNew = win_h - y;

		// Setting up new min and max
		double xMin = (xAnchor<xNew) ? xAnchor : xNew;
		double xMax = (xAnchor>xNew) ? xAnchor : xNew;

		double yMin = (yAnchor<yNew) ? yAnchor : yNew;
		double yMax = (yAnchor>yNew) ? yAnchor : yNew;


		// Going from the point (xMin, yMin) to Complex Plane!
		double xMinReal = getSReal(xMin);
		double yMinImaginary = getSImaginary(yMin);

		// Going from the point (xMax, yMax) to Complex Plane!
		double xMaxReal = getSReal(xMax);
		double yMaxImaginary = getSImaginary(yMax);

		// Across X of Rubberband Rectangle:
		double xDistance = xMaxReal - xMinReal;

		// Across Y of Rectangle:
		double yDistance = yMaxImaginary - yMinImaginary;

		// Ar = Aspect Ratio of the Rectangle:
		double Ar = yDistance / xDistance;

		// Getting the size of the window-->
		double xS = x_2 - x_1;
		double yS = y_2 - y_1;

		// Aw = Aspect Ratio of the Window:
		double Aw =  yS/xS;

		// If the Aspect Ratio of the Rect & Window are the same,
		// then no adjustments needed!
		if (Ar == Aw) {
			x_1 = xMinReal;
			x_2 = xMaxReal;
			y_1 = yMinImaginary;
			y_2 = yMaxImaginary;
		}
		else if (Ar > Aw) {
			// We need to give the new window a greater width xDistance so that:
			// Aw = (yMaxImaginary - yMinImaginary) / xDistNew; --> creating a new ratio
			double xDistNew = (yMaxImaginary - yMinImaginary)/Aw;
			// We also want the horizontal midpoint of the new region to be the same as in the old region.
			double xMidPoint = (xMinReal + xMaxReal) / 2;
			// Now we set x_1 and x_2 so that they are separated by xDistNew and centered on xMidPoint :
			x_1 = xMidPoint - xDistNew / 2;
			x_2 = xMidPoint + xDistNew / 2;
			y_1 = yMinImaginary;
			y_2 = yMaxImaginary;
		}
		else {
			// We need to give the new window a greater height yDistance so that: Aw = (x_2-x_1) / yDistNew
			double yDistNew = (xMaxReal - xMinReal) / Aw;
			// We also want the horizontal midpoint of the new region to be the same as in the old region.
			double yMidPoint = (yMinImaginary + yMaxImaginary) / 2;
			// Now we set x_1 and x_2 so that they are separated by xDistNew and centered on xMidPoint :
			x_1 = xMinReal;
			x_2 = xMaxReal;
			y_1 = yMidPoint - yDistNew / 2;
			y_2 = yMidPoint + yDistNew / 2;
		}

		cout << "x_1:";
		cout << x_1;
		cout << "/";
		cout << "x_2:";
		cout << x_2;
		cout << "/";
		cout << "y_1:";
		cout << y_1;
		cout << "/";
		cout << "y_2:";
		cout << y_2;
		cout << "/";
		cout << "win_w:";
		cout << win_w;
		cout << "/";
		cout << "win_h:";
		cout << win_h;

		// When we redraw with rubberband --> 
		// 1. Add new mandelbrot set to the currentViewStack
		// 2. Clear the previouslySavedStack
		// 3. Display top of the currentViewStack 

		// 1. Add new mandelbrot set to the currentViewStack
		Mandelbrot mandy(x_1, x_2, y_1, y_2);
		currentViewStack.push_back(mandy);
		currentViewStackIndex++;

		// 2. Clear the previouslySavedStack
		savedPreviouslyViewedStack.clear();
		savedPreviouslyViewedStackIndex = -1;

		// 3. Display mandy
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
	// PREVIOUS
	Mandelbrot init(x_1, x_2, y_1, y_2);
	currentViewStack.push_back(init);
	currentViewStackIndex++;

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