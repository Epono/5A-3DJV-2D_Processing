#include <cstdlib>
#include <GL/glut.h>
#include <windows.h>

#include <vector>
#include <iostream>

#include <memory>

#include <math.h>
#include "utils.h"
#include "Point.h"
#include "LineStrip.h"
#include "Graham_Scan.h"
#include "Jarvis.h"
#include "Triangulation2D_qcq.h"
//#include "Triangulation2D_Delaunay_Bowyer_Watson.h"
#include "Voronoi.h"

creationState currentCreationState = WAITING_FOR_FIRST_CLICK;
algorithm currentAlgorithm = NONE;

std::vector<LineStrip*> lines;
LineStrip *currentLine = nullptr;
LineStrip *currentJarvisPoints = nullptr;
Jarvis* jarvis;
LineStrip *currentGrahamPoints = nullptr;
Graham_Scan* graham_scan;
LineStrip *currentTriangulation2D_qcqPoints = nullptr;
Triangulation2D_qcq* triangulation2D_qcq;
Voronoi* voronoi;
LineStrip *areaVoronoi = nullptr;


float windowColor[3] = {0, 0.5f, 0.5f};		// Window color
int windowVerticeToMove = -1;
float pas = 20;
color_rgb dessinColor = color_rgb(1.f, 0.f, 0.f);
const float DEG2RAD = M_PI / 180;

Point clicked;

int presse = 0;										// Stores if the mouse is dragging

/* Functions prototypes */
void display();										// manages displaying
void keyboard(unsigned char key, int x, int y);		// manages keyboard inputs
void keyboardSpecial(int key, int x, int y);		// manages keyboard inputs
void mouse(int bouton, int etat, int x, int y);		// manages mouse clicks
void motion(int x, int y);							// manages mouse motions

void drawWindow();									// draws the window (algorithm of my bite)
void createMenu();
void menu(int opt);
void colorPicking(int option);
void setPolygonColor(float colors[3], float r, float g, float b);
void write();										// Writes on the top left what's happening

void drawLineStrip(LineStrip& line, int lineSize, bool drawCurve);
void drawTriangleStrip(TriangleStrip& triangles, int lineSize);
void drawCircle(float radius, Point& center);


void translate(int xOffset, int yOffset);
void scale(float scaleX, float scaleY);
void rotate(float angle);

#define USE_DRAW_V2 TRUE

int main(int argc, char **argv) {
	//Glut and Window Initialization
	glutInit(&argc, argv);										// Initializes Glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);				// RGB display mode, with depth
	glutInitWindowSize(WIDTH, HEIGHT);								// Sets window's dimensions
	glutInitWindowPosition(100, 100);							// Positions the window
	glutCreateWindow("FunStuffWithOpenGL");						// Title of the window

	gluOrtho2D(0, WIDTH, 0, HEIGHT);								// 2D orthogonal frame with xMin, xMax, yMin, yMax

	// OpenGL initialization
	glClearColor(0.0, 0.0, 0.0, 0.0);		// Background color : black ?
	glColor3f(0.0, 0.0, 0.0);				// Color : white
	glPointSize(4.0);						// Point size : 4px

	// Registering of callback functions called by glut
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	currentLine = new LineStrip();
	jarvis = new Jarvis();
	graham_scan = new Graham_Scan();
	triangulation2D_qcq = new Triangulation2D_qcq();
	voronoi = new Voronoi();


	//glOrtho(-1, 1.0, -1, 1.0, -1.0, 1.0); // il faut le mettre ?
	createMenu();							// Creates the menu available via right-click
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();         // Launches main OpenGL loop, events handlers
	return 0;				// Should not get here (unless we exit with 'q' ?)
}

/*
* Function in charge of refreshing the display of the window
*/
void display() {
	glClear(GL_COLOR_BUFFER_BIT);	// Clears the display

	write();

#if USE_DRAW_V2
	switch(currentAlgorithm) {
	case GRAHAM_SCAN:
		drawLineStrip(*currentGrahamPoints, 2, true);
		break;
	case JARVIS:
		drawLineStrip(LineStrip(jarvis->getEnveloppe()), 2, true);
		break;
	case TRIANGULATION2D_QCQ:
		drawTriangleStrip(*triangulation2D_qcq, 2);
		break;
	case VORONOI:
		//drawLineStrip(LineStrip(voronoi->getArea()), 2, true);
		drawLineStrip(LineStrip(voronoi->getPoints()), 1, true);
		drawLineStrip(LineStrip(voronoi->getMediatriceP()), 2, true);
		drawCircle(voronoi->getRadius().at(0), *voronoi->getActualPoint().at(0));
		drawCircle(voronoi->getRadius().at(0), *voronoi->getActualPoint().at(1));
		drawLineStrip(LineStrip(voronoi->getIntersect()),2,true);
		break;
	case NONE:
		break;
	default:
		break;
	}
	drawLineStrip(*currentLine, 2, false);
#else
	if(currentLine != nullptr)
		drawLineStrip(*currentLine, 2, false);
	if(currentJarvisPoints != nullptr)
		drawLineStrip(*currentJarvisPoints, 2, true);
	if(currentGrahamPoints != nullptr)
		drawLineStrip(*currentGrahamPoints, 2, true);
#endif

	glutSwapBuffers();				// Double buffer ?
	glFlush();						// Forces refresh ?
}

/*
* Function in charge of handling mouse events (clicking only, not dragging)
*/
void mouse(int button, int state, int x, int y) {
	y = HEIGHT - y;
	clicked = Point(x, y);
	Point* point = new Point(x, y);
	if(currentLine != nullptr) {
		if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			presse = 1;
			switch(currentCreationState) {
			case PENDING:
				printf("Coords clicked (pending state) : (%d, %d)\n", x, y);
				break;
			case WAITING_FOR_FIRST_CLICK:
				printf("Coords clicked (pending state) : (%d, %d)\n", x, y);
				currentLine->addPoint(point);
				currentCreationState = WAITING_FOR_NEXT_CLICK;;
				break;
			case WAITING_FOR_NEXT_CLICK:
				printf("Coords clicked (pending state) : (%d, %d)\n", x, y);
				currentLine->addPoint(point);
				break;
			case SELECT_POINT:
				break;
			}
		}
		if(currentLine->getPoints().size() > 0) {
			if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
				presse = 0;
				windowVerticeToMove = -1;
				std::vector<Point*>& points = currentLine->getPoints();
				if(currentCreationState == SELECT_POINT) {
					for(unsigned int i = 0; i < points.size(); i++) {
						float tempX = points.at(i)->getX();
						float tempY = points.at(i)->getY();
						int distance = 10;
						if(abs(tempX - x) < distance && abs(tempY - y) < distance) {
							windowVerticeToMove = i;
							break;
						}
					}
				}
			}
		}
	}

	switch(currentAlgorithm) {
	case GRAHAM_SCAN:
		graham_scan->setPoints(currentLine->getPoints());
		graham_scan->calculEnveloppe();
		break;
	case JARVIS:
		jarvis->setPoints(currentLine->getPoints());
		jarvis->computeJarvis();
		break;
	case TRIANGULATION2D_QCQ:
		triangulation2D_qcq->setPoints(currentLine->getPoints());
		triangulation2D_qcq->computeTriangulation();
		break;
	case VORONOI:
		
		break;
	case NONE:
		break;
	default:
		break;
	}

	glutPostRedisplay();		// Refresh display
}

void motion(int x, int y) {
	y = HEIGHT - y;
	if(currentCreationState == SELECT_POINT) {
		if(windowVerticeToMove != -1) {
			std::vector<Point*>& points = currentLine->getPoints();
			points.at(windowVerticeToMove)->setX(x);
			points.at(windowVerticeToMove)->setY(y);
		}
	}
	else if(currentCreationState == SCALING) {
		float xOffset = clicked.getX() - x;
		float yOffset = clicked.getY() - y;
		scale(1 - (xOffset / 500), 1 + (yOffset / 500));
		clicked.setX(x);
		clicked.setY(y);
	}
	else if(currentCreationState == ROTATING) {
		float sumX = 0;
		float sumY = 0;

		//Calcul du barycentre pour décaler
		std::vector<Point*>& points = currentLine->getPoints();
		for(unsigned int i = 0; i < points.size(); i++) {
			sumX += points.at(i)->getX();
			sumY += points.at(i)->getY();
		}

		Point barycenter = {sumX / points.size(), sumY / points.size()};

		Point a(clicked);
		Point b(barycenter);
		Point c(x, y);

		Point ab(b.getX() - a.getX(), b.getY() - a.getY());
		Point cb(b.getX() - c.getX(), b.getY() - c.getY());

		float dot = (ab.getX() * cb.getX() + ab.getY() * cb.getY()); // dot product
		float cross = (ab.getX() * cb.getY() - ab.getY() * cb.getX()); // cross product

		float alpha = atan2(cross, dot);

		float angle = floor(alpha * 180. / M_PI + 0.5);

		rotate(angle * (M_PI / 180));

		clicked.setX(x);
		clicked.setY(y);
	}
	else if(currentCreationState == TRANSLATING) {
		float xOffset = clicked.getX() - x;
		float yOffset = clicked.getY() - y;
		translate(-xOffset, -yOffset);
		clicked.setX(x);
		clicked.setY(y);
	}

	switch(currentAlgorithm) {
	case GRAHAM_SCAN:
		graham_scan->calculEnveloppe();
		break;
	case JARVIS:
		jarvis->computeJarvis();
		break;
	case TRIANGULATION2D_QCQ:
		triangulation2D_qcq->computeTriangulation();
		break;
	case VORONOI:
		//TODO
		break;
	case NONE:
		break;
	default:
		break;
	}

	glutPostRedisplay(); // Rafraichissement de l'affichage
}

/*
* Function in charge of handling keyboard events
* key : key pressed
* x, y : coordinates of the pointer when the key was pressed ?
*/
void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 'd': // Switch to connected strip lines creation
		if(currentCreationState == SELECT_POINT) {
			currentCreationState = WAITING_FOR_NEXT_CLICK;
		}
		else if(currentCreationState != WAITING_FOR_FIRST_CLICK) {
			printf("Switching to window creation\n");
			currentCreationState = WAITING_FOR_FIRST_CLICK;
		}
		break;
	case 'v': // Validates
		currentCreationState = WAITING_FOR_FIRST_CLICK;
		if(currentLine != nullptr) {
			lines.push_back(currentLine);
			currentLine = new LineStrip();
		}
		break;
	case 'c': // Clear the window
		currentCreationState = WAITING_FOR_FIRST_CLICK;
		lines.clear();
		currentLine = new LineStrip();
		break;
	case 's':
		// select point
		if(currentCreationState != SELECT_POINT) {
			printf("Switching to select point\n");
			currentCreationState = SELECT_POINT;
		}
		break;
	case '-':
		if(pas > 0) --pas;
		break;
	case '+':
		// decrease step
		++pas;
		break;
	case 't':
		currentCreationState = TRANSLATING;
		break;
	case 'r':
		currentCreationState = ROTATING;
		break;
	case 'o':
		currentCreationState = SCALING;
		break;
	case 'n':
		currentAlgorithm = NONE;
		break;
	case 'm':
		currentAlgorithm = VORONOI;
		std::cout << "M is pressed" << std::endl;
		//voronoi->DefineArea(currentLine->getPoints());
		voronoi->VoronoiAlgorithm(currentLine->getPoints());
		break;
	case 'g':
		currentAlgorithm = GRAHAM_SCAN;
#if USE_DRAW_V2
		graham_scan->setPoints(currentLine->getPoints());
		graham_scan->calculEnveloppe();
#else
		{
			Graham_Scan g(currentLine->getPoints());
			g.calculEnveloppe();
			currentGrahamPoints = new LineStrip(g.getEnveloppe());
			break;
	}
#endif
	case 'j':
		currentAlgorithm = JARVIS;
#if USE_DRAW_V2
		jarvis->setPoints(currentLine->getPoints());
		jarvis->computeJarvis();
#else
		{
			Jarvis j(currentLine->getPoints());
			j.computeJarvis();
			currentJarvisPoints = new LineStrip(j.getEnveloppe());
}
#endif
		break;
	case 'i':
		currentAlgorithm = TRIANGULATION2D_QCQ;
		triangulation2D_qcq->setPoints(currentLine->getPoints());
		triangulation2D_qcq->computeTriangulation();
		break;
	case 127:
		// deletes selected point
		if(windowVerticeToMove != -1) {
			std::vector<Point*>& points = currentLine->getPoints();
			points.erase(points.begin() + windowVerticeToMove);
		}
		windowVerticeToMove = -1;
		//TODO: refresh les algos
		break;
	case 27:
		exit(0);
	case 'q':
		exit(0);
	}

	glutPostRedisplay(); // Rafraichissement de l'affichage
}

void keyboardSpecial(int key, int x, int y) {
	int modifier = glutGetModifiers();

	switch(modifier) {
	case 0: // NONE - Translation
		switch(key) {
		case 100: // LEFT
			translate(-10, 0);
			break;
		case 101: // UP
			translate(0, -10);
			break;
		case 102: // RIGHT
			translate(10, 0);
			break;
		case 103: // DOWN
			translate(0, 10);
			break;
		}
		break;
	case 1: // SHIFT - Scaling
		switch(key) {
		case 100: // LEFT
			scale(0.9f, 1.0f);
			break;
		case 101: // UP
			scale(1.0f, 1.1f);
			break;
		case 102: // RIGHT
			scale(1.1f, 1.0f);
			break;
		case 103: // DOWN
			scale(1.0f, 0.9f);
			break;
		}
		break;
	case 2: // CTRL - Rotation
		switch(key) {
		case 100: // LEFT
			rotate(0.05f);
			break;
		case 101: // UP
			rotate(0.05f);
			break;
		case 102: // RIGHT
			rotate(-0.05f);
			break;
		case 103: // DOWN
			rotate(-0.05f);
			break;
		}
		break;
	case 3: // ALT
		break;
	}

	glutPostRedisplay(); // Rafraichissement de l'affichage
}

/*
* Creates the menu available via right-click
*/
void createMenu() {
	int mainMenu;

	mainMenu = glutCreateMenu(menu);

	glutAddMenuEntry("Jarvis", 1);
	glutAddMenuEntry("Graham-Scan", 2);
	/*
	glutAddMenuEntry("Vert", 1);
	glutAddMenuEntry("Rouge", 2);
	glutAddMenuEntry("Bleu", 3);
	glutAddMenuEntry("Nouvelle courbe", 4);
	*/

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int opt) {
	switch(opt) {
	case 1:
		std::cout << "Jarvis Implem" << std::endl;
		break;
	case 2:
		std::cout << "Graham-Scan Implem" << std::endl;
		break;
	default:
		printf("What ? %d choisie mais pas d'option\n", opt);
		break;
	}
	display();
}

void setPolygonColor(float colors[3], float r, float g, float b) {
	colors[0] = r;
	colors[1] = g;
	colors[2] = b;
}

void drawLineStrip(LineStrip& lineStrip, int lineSize, bool drawLine) {
	if(lineStrip.getPoints().empty()) {
		return;
	}

	glLineWidth(lineSize);
	glColor3f(1.0f, 0.0f, 0.0f);		// Sets the drawing color
	if(drawLine) {
		// Draws line strip
		glBegin(GL_LINE_STRIP);
		for(auto &p : lineStrip.getPoints()) {
			glVertex2f(p->getX(), p->getY());
		}
		glVertex2f(lineStrip.getPoints().at(0)->getX(), lineStrip.getPoints().at(0)->getY());
		glEnd();
	}

	// Draws vertices of the connected lines strip
	glBegin(GL_POINTS);
	for(auto &p : lineStrip.getPoints())
		glVertex2f(p->getX(), p->getY());
	glEnd();

	if(lineStrip.getPoints().size() > 2) {
		color_rgb c = lineStrip.getColor();
		glColor3f(c._r, c._g, c._b);		// Sets the drawing color
	}
}

void drawTriangleStrip(TriangleStrip& triangleStrip, int lineSize) {
	if(triangleStrip.getPoints().empty()) {
		return;
	}

	glLineWidth(lineSize);
	glColor3f(1.0f, 0.0f, 0.0f);		// Sets the drawing color
	glBegin(GL_LINE_STRIP);
	for(auto &t : triangleStrip.getTriangles()) {
		glVertex2f(t->getPointA()->getX(), t->getPointA()->getY());
		glVertex2f(t->getPointB()->getX(), t->getPointB()->getY());
		glVertex2f(t->getPointC()->getX(), t->getPointC()->getY());
		glVertex2f(t->getPointA()->getX(), t->getPointA()->getY());
	}
	glEnd();

	// Draws vertices of the connected lines strip
	glBegin(GL_POINTS);
	for(auto &p : triangleStrip.getPoints()) {
		glVertex2f(p->getX(), p->getY());
	}
	glEnd();

	if(triangleStrip.getPoints().size() > 2) {
		color_rgb c = triangleStrip.getColor();
		glColor3f(c._r, c._g, c._b);		// Sets the drawing color
	}
}

void drawCircle(float radius, Point& center) {
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < 360; i++) {
		float degInRad = i*DEG2RAD;
		glVertex2f(center.getX() + cos(degInRad)*radius, center.getY() + sin(degInRad)*radius);
	}

	glEnd();
}

void write() {
	char* truc;
	if(currentCreationState == SELECT_POINT) {
		truc = "Selecting point";
	}
	else if(currentCreationState == WAITING_FOR_FIRST_CLICK || currentCreationState == WAITING_FOR_NEXT_CLICK) {
		truc = "Drawing lines";
	}
	else {
		truc = "Unknown action";
	}
	glRasterPos2f(5, HEIGHT - 20);

	for(int i = 0; truc[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, truc[i]);
}

void translate(int xOffset, int yOffset) {

	float x, y;

	float matrix[6] = {
		1, 0, xOffset,
		0, 1, yOffset
	};

	std::vector<Point*>& points = currentLine->getPoints();
	for(unsigned int i = 0; i < points.size(); i++) {
		x = points.at(i)->getX();
		y = points.at(i)->getY();

		points.at(i)->setX((x * matrix[0]) + (y * matrix[1]) + (1 * matrix[2]));
		points.at(i)->setY((x * matrix[3]) + (y * matrix[4]) + (1 * matrix[5]));
	}
}

void scale(float scaleX, float scaleY) {

	if(scaleX == 0.0f) {
		scaleX = 1.0f;
	}
	if(scaleY == 0.0f) {
		scaleY = 1.0f;
	}

	float x, y;

	float matrix[4] = {
		scaleX, 0,
		0, scaleY
	};

	float sumX = 0;
	float sumY = 0;

	//Calcul du barycentre pour décaler
	std::vector<Point*>& points = currentLine->getPoints();
	for(unsigned int i = 0; i < points.size(); i++) {
		sumX += points.at(i)->getX();
		sumY += points.at(i)->getY();
	}

	Point barycenter = {sumX / currentLine->getPoints().size(), sumY / currentLine->getPoints().size()};

	for(unsigned int i = 0; i < points.size(); i++) {
		// Translate barycenter to origin
		points.at(i)->setX(points.at(i)->getX() - barycenter.getX());
		points.at(i)->setY(points.at(i)->getY() - barycenter.getY());

		x = points.at(i)->getX();
		y = points.at(i)->getY();

		// Scale
		points.at(i)->setX((x * matrix[0]) + (y * matrix[1]));
		points.at(i)->setY((x * matrix[2]) + (y * matrix[3]));

		// Translation back
		points.at(i)->setX(points.at(i)->getX() + barycenter.getX());
		points.at(i)->setY(points.at(i)->getY() + barycenter.getY());
	}
}

void rotate(float angle) {

	float x, y;
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);

	float matrix[4] = {
		cos_angle, -sin_angle,
		sin_angle, cos_angle
	};

	float sumX = 0;
	float sumY = 0;

	//Calcul du barycentre pour décaler
	std::vector<Point*>& points = currentLine->getPoints();
	for(unsigned int i = 0; i < points.size(); i++) {
		sumX += points.at(i)->getX();
		sumY += points.at(i)->getY();
	}

	Point barycenter = {sumX / points.size(), sumY / points.size()};

	for(unsigned int i = 0; i < points.size(); i++) {

		// Translate barycenter to origin
		points.at(i)->setX(points.at(i)->getX() - barycenter.getX());
		points.at(i)->setY(points.at(i)->getY() - barycenter.getY());

		x = points.at(i)->getX();
		y = points.at(i)->getY();

		// Rotation around origin
		points.at(i)->setX((x * matrix[0]) + (y * matrix[1]));
		points.at(i)->setY((x * matrix[2]) + (y * matrix[3]));

		// Translation back
		points.at(i)->setX(points.at(i)->getX() + barycenter.getX());
		points.at(i)->setY(points.at(i)->getY() + barycenter.getY());
	}
}