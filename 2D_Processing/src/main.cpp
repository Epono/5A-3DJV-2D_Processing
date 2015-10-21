#include <cstdlib>
#include <GL/glut.h>
#include <windows.h>

#include <vector>
#include <iostream>

#include <math.h>
#include "utils.h"
#include "Point.h"
#include "LineStrip.h"
#include "Graham-Scan.h"
#include "Jarvis.h"

#define M_PI 3.14

int creationState = waitingForFirstClick;

std::vector<LineStrip*> lines;
LineStrip *currentLine = nullptr;

float windowColor[3] = {0, 0.5f, 0.5f};		// Window color
int windowVerticeToMove = -1;
bool hideControlPoints = false;
float pas = 20;
color_rgb dessinColor = color_rgb(1.f, 0.f, 0.f);

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

void drawCurve(LineStrip& line, int lineSize);

void translate(int xOffset, int yOffset);
void scale(float scaleX, float scaleY);
void rotate(float angle);

#define WIDTH 1280
#define HEIGHT 720

int main(int argc, char **argv) {
	//Glut and Window Initialization
	glutInit(&argc, argv);										// Initializes Glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);				// RGB display mode, with depth
	glutInitWindowSize(1280, 720);								// Sets window's dimensions
	glutInitWindowPosition(100, 100);							// Positions the window
	glutCreateWindow("FunStuffWithOpenGL");						// Title of the window

	gluOrtho2D(0, WIDTH, HEIGHT, 0);								// 2D orthogonal frame with xMin, xMax, yMin, yMax

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

	for(auto &l : lines)
		drawCurve(*l, 2);
	if(currentLine != nullptr)
		drawCurve(*currentLine, 2);

	glutSwapBuffers();				// Double buffer ?
	glFlush();						// Forces refresh ?
}

/*
* Function in charge of handling mouse events (clicking only, not dragging)
*/
void mouse(int button, int state, int x, int y) {
	clicked = Point(x, y);
	Point point(x, y);
	if(currentLine != nullptr) {
		if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			presse = 1;
			switch(creationState) {
			case pending:
				printf("Coords clicked (pending state) : (%d, %d)\n", x, y);
				break;
			case waitingForFirstClick:
				currentLine->addPoint(point);
				creationState++;
				break;
			case waitingForNextClick:
				currentLine->addPoint(point);
				break;
			case selectPoint:
				break;
			}
		}
		if(currentLine->getPoints().size() > 0) {
			if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
				presse = 0;
				windowVerticeToMove = -1;
				std::vector<Point>& points = currentLine->getPoints();
				if(creationState == selectPoint) {
					for(unsigned int i = 0; i < points.size(); i++) {
						float tempX = points.at(i).getX();
						float tempY = points.at(i).getY();
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

	glutPostRedisplay();		// Refresh display
}

void motion(int x, int y) {
	if(creationState == selectPoint) {
		if(windowVerticeToMove != -1) {
			std::vector<Point>& points = currentLine->getPoints();
			points.at(windowVerticeToMove).setX(x);
			points.at(windowVerticeToMove).setY(y);
		}
	}
	else if(creationState == scaling) {
		float xOffset = clicked.getX() - x;
		float yOffset = clicked.getY() - y;
		scale(1 - (xOffset / 500), 1 + (yOffset / 500));
		clicked.setX(x);
		clicked.setY(y);
	}
	else if(creationState == rotating) {
		float sumX = 0;
		float sumY = 0;

		//Calcul du barycentre pour décaler
		std::vector<Point>& points = currentLine->getPoints();
		for(unsigned int i = 0; i < points.size(); i++) {
			sumX += points.at(i).getX();
			sumY += points.at(i).getY();
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
	else if(creationState == translating) {
		float xOffset = clicked.getX() - x;
		float yOffset = clicked.getY() - y;
		translate(-xOffset, -yOffset);
		clicked.setX(x);
		clicked.setY(y);
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
		if(creationState == selectPoint) {
			creationState = waitingForNextClick;
		}
		else if(creationState != waitingForFirstClick) {
			printf("Switching to window creation\n");
			creationState = waitingForFirstClick;
		}
		break;
	case 'v': // Validates
		creationState = waitingForFirstClick;
		if(currentLine != nullptr) {
			lines.push_back(currentLine);
			currentLine = new LineStrip();
		}
		break;
	case 'c': // Clear the window
		creationState = waitingForFirstClick;
		lines.clear();
		currentLine = new LineStrip();
		break;
	case 's':
		// select point
		if(creationState != selectPoint) {
			printf("Switching to select point\n");
			creationState = selectPoint;
		}
		break;
	case 'h':
		// hide control points
		hideControlPoints = !hideControlPoints;
		break;
	case '-':
		if(pas > 0) --pas;
		break;
	case '+':
		// decrease step
		++pas;
		break;
	case 't':
		creationState = translating;
		break;
	case 'r':
		creationState = rotating;
		break;
	case 'o':
		creationState = scaling;
		break;
	case 'g':
	{
		Graham_Scan g(currentLine->getPoints());
		g.computeCentroid();
		std::cout << "Barycentre : " << g.getCentroid()->getX() << ", " << g.getCentroid()->getY() << std::endl;
		g.sortPoints();
		break;
	}
	case 'j':
	{
		Jarvis j(currentLine->getPoints());
		j.computeJarvis();
		for(auto& point : j.getEnveloppe()) {
			std::cout << "Enveloppe : " << point.getX() << ", " << point.getY() << std::endl;
		}
		break;
	}
	case 127:
		// deletes selected point
		if(windowVerticeToMove != -1) {
			std::vector<Point>& points = currentLine->getPoints();
			points.erase(points.begin() + windowVerticeToMove);
		}
		windowVerticeToMove = -1;
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

	glutAddMenuEntry("Vert", 1);
	glutAddMenuEntry("Rouge", 2);
	glutAddMenuEntry("Bleu", 3);
	glutAddMenuEntry("Nouvelle courbe", 4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int opt) {
	switch(opt) {
	case 1:
		std::cout << "Vert" << std::endl;
		currentLine->setColor(0.f, 1.f, 0.f);
		break;
	case 2:
		std::cout << "Rouge" << std::endl;
		currentLine->setColor(1.f, 0.f, 0.f);
		break;
	case 3:
		std::cout << "Bleu" << std::endl;
		currentLine->setColor(0.f, 0.f, 1.f);
		break;
	case 4:
		std::cout << "Nouvelle courbe" << std::endl;
		if(currentLine != nullptr)
			lines.push_back(currentLine);
		currentLine = new LineStrip();
		creationState = waitingForFirstClick;
		break;
	default:
		printf("What ? %d choisie mais pas d'option\n", opt);
		break;
	}
	display();
}

void setPolygonColor(float colors[3], float r, float g, float b) {
	*colors = r;
	*(colors + 1) = g;
	*(colors + 2) = b;
}

void drawCurve(LineStrip& line, int lineSize) {
	glLineWidth(lineSize);
	glColor3f(1.0f, 0.0f, 0.0f);		// Sets the drawing color
	if(!hideControlPoints) {
		// Draws line strip
		//glBegin(GL_LINE_STRIP);
		//for(auto &p : line.getPoints())
		//	glVertex2f(p.getX(), p.getY());
		//glEnd();

		// Draws vertices of the connected lines strip
		glBegin(GL_POINTS);
		for(auto &p : line.getPoints())
			glVertex2f(p.getX(), p.getY());
		glEnd();
	}
	if(line.getPoints().size() > 2) {
		color_rgb c = line.getColor();
		glColor3f(c._r, c._g, c._b);		// Sets the drawing color
	}
}

void write() {
	char* truc;
	if(creationState == selectPoint) {
		truc = "Selecting point";
	}
	else if(creationState == waitingForFirstClick || creationState == waitingForNextClick) {
		truc = "Drawing lines";
	}
	else {
		truc = "Unknown action";
	}
	glRasterPos2f(5, 20);

	for(int i = 0; truc[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, truc[i]);
}

void translate(int xOffset, int yOffset) {

	float x, y;

	float matrix[6] = {
		1, 0, xOffset,
		0, 1, yOffset
	};

	std::vector<Point>& points = currentLine->getPoints();
	for(unsigned int i = 0; i < points.size(); i++) {
		x = points.at(i).getX();
		y = points.at(i).getY();

		points.at(i).setX((x * matrix[0]) + (y * matrix[1]) + (1 * matrix[2]));
		points.at(i).setY((x * matrix[3]) + (y * matrix[4]) + (1 * matrix[5]));
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
	std::vector<Point>& points = currentLine->getPoints();
	for(unsigned int i = 0; i < points.size(); i++) {
		sumX += points.at(i).getX();
		sumY += points.at(i).getY();
	}

	Point barycenter = {sumX / currentLine->getPoints().size(), sumY / currentLine->getPoints().size()};

	for(unsigned int i = 0; i < points.size(); i++) {
		// Translate barycenter to origin
		points.at(i).setX(points.at(i).getX() - barycenter.getX());
		points.at(i).setY(points.at(i).getY() - barycenter.getY());

		x = points.at(i).getX();
		y = points.at(i).getY();

		// Scale
		points.at(i).setX((x * matrix[0]) + (y * matrix[1]));
		points.at(i).setY((x * matrix[2]) + (y * matrix[3]));

		// Translation back
		points.at(i).setX(points.at(i).getX() + barycenter.getX());
		points.at(i).setY(points.at(i).getY() + barycenter.getY());
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
	std::vector<Point>& points = currentLine->getPoints();
	for(unsigned int i = 0; i < points.size(); i++) {
		sumX += points.at(i).getX();
		sumY += points.at(i).getY();
	}

	Point barycenter = {sumX / points.size(), sumY / points.size()};

	for(unsigned int i = 0; i < points.size(); i++) {

		// Translate barycenter to origin
		points.at(i).setX(points.at(i).getX() - barycenter.getX());
		points.at(i).setY(points.at(i).getY() - barycenter.getY());

		x = points.at(i).getX();
		y = points.at(i).getY();

		// Rotation around origin
		points.at(i).setX((x * matrix[0]) + (y * matrix[1]));
		points.at(i).setY((x * matrix[2]) + (y * matrix[3]));

		// Translation back
		points.at(i).setX(points.at(i).getX() + barycenter.getX());
		points.at(i).setY(points.at(i).getY() + barycenter.getY());
	}
}