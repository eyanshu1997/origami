#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include<string.h>
#if GL_EXT_polygon_offset
int polygon_offset;
#endif
#define RED 1
#define BLUE 2
#define YELLOW 3
#define BLACK 4
#define MAGNETA 5
float red = 1.0, green = 1.0, blue = 1.0;
enum { FLAT, FLAP1, FLAP2, CENTER2, WING2, CENTER1, WING1, FOLDED } States;
int ch;
int flag = 0;
int motion = 1;
int spinning = 1;
int state = FLAT;
int click = 0;
int delay = 0;
int direction;
int axis = 0;
float flap1_angle = 0;
float flap2_angle = 0;
float center1_angle = 0;
float center2_angle = 0;
float wing1_angle = 0;
float wing2_angle = 0;
typedef GLfloat Point[2];
Point poly1[] = { { -1, 0 },{ -1 / 3.0, 2 / 3.0 },{ -1, 2 / 3.0 } };
Point poly2[] = { { -1, 1 },{ -1, 2 / 3.0 },{ -1 / 3.0, 2 / 3.0 },{ 0, 1 } };
Point poly3[] = { { 0, 1 },{ 1, 1 },{ 1, 2 / 3.0 },{ 1 / 3.0, 2 / 3.0 } };
Point poly4[] = { { 1 / 3.0, 2 / 3.0 },{ 1, 2 / 3.0 },{ 1, 0 } };
Point poly5[] = { { -1 / 3.0, 2 / 3.0 },{ 0, 1 },{ 0, -1.5 },{ -1 / 3.0, -1.5 } };
Point poly6[] = { { 0, 1 },{ 1 / 3.0, 2 / 3.0 },{ 1 / 3.0, -1.5 },{ 0, -1.5 } };
Point poly7[] = { { -1, 0 },{ -1 / 3.0, 2 / 3.0 },{ -1 / 3.0, -1.5 },{ -1, -1.5 } };
Point poly8[] = { { 1, 0 },{ 1 / 3.0, 2 / 3.0 },{ 1 / 3.0, -1.5 },{ 1, -1.5 } };
void polydlist(int dlist, int num, Point points[])
{
	int i;
	glNewList(dlist, GL_COMPILE);
	glBegin(GL_POLYGON);
	for (i = 0; i < num; i++) {
		glVertex2fv(&points[i][0]);
	}
	glEnd();
	glEndList();
}
void idle(void)
{
	if (spinning)
		click++;
	switch (state) {
	case FLAT:
		delay++;
		if (delay >= 80) {
			delay = 0;
			state = FLAP1;
			glutSetWindowTitle("origami (folding)");
			direction = 1;
		}
		break;
	case FLAP1:
		flap1_angle += 2 * direction;
		if (flap1_angle >= 180) {
			state = FLAP2;
		}
		else if (flap1_angle <= 0) {
			state = FLAT;
		}
		break;
	case FLAP2:
		flap2_angle += 2 * direction;
		if (flap2_angle >= 180) {
			state = CENTER2;
		}
		else if (flap2_angle <= 0) {
			state = FLAP1;
		}
		break;
	case CENTER2:
		center2_angle += 2 * direction;
		if (center2_angle >= 84) {
			state = WING2;
		}
		else if (center2_angle <= 0) {
			state = FLAP2;
		}
		break;
	case WING2:
		wing2_angle += 2 * direction;
		if (wing2_angle >= 84) {
			state = CENTER1;
		}
		else if (wing2_angle <= 0) {
			state = CENTER2;
		}
		break;
	case CENTER1:
		center1_angle += 2 * direction;
		if (center1_angle >= 84) {
			state = WING1;
		}
		else if (center1_angle <= 0) {
			state = WING2;
		}
		break;
	case WING1:
		wing1_angle += 2 * direction;
		if (wing1_angle >= 84) {
			state = FOLDED;
		}
		else if (wing1_angle <= 0) {
			state = CENTER1;
		}
		break;
	case FOLDED:
		delay++;
		if (delay >= 80) {
			delay = 0;
			glutSetWindowTitle("origami (unfolding)");
			direction = -1;
			state = WING1;
		}
		break;
	}
	glutPostRedisplay();
}
void reverse()
{
	direction = -direction;
	if (direction > 0) {
		glutSetWindowTitle("origami (folding)");
	}
	else {
		glutSetWindowTitle("origami (unfolding)");
	}
}
void spin()
{
	spinning = 1 - spinning;
}
void move()
{
	motion = 1 - motion;
	if (motion) {
		glutIdleFunc(idle);
	}
	else {
		glutIdleFunc(NULL);
	}
}
void text(const char str[])
{
	int i;
	for (i = 0; i<strlen(str); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
}
void Planecolor(int option)
{
	switch (option)
	{
	case 6:
	case RED:red = 1.0, green = 0.0, blue = 0.0; break;
	case 7:
	case BLUE:red = 0.0, green = 0.0, blue = 1.0; break;
	case 8:
	case YELLOW:red = 1.0, green = 1.0, blue = 0.0; break;
	case 9:
	case BLACK:red = 1.0, green = 1.0, blue = 1.0; break;
	case 0:
	case MAGNETA:red = 1.0, green = 0.0, blue = 1.0; break;
	default:red = 67, green = 205, blue = 128;
	}
}
void sheet1()
{
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(-0.7, 1.6, -1.0);
	text("GRAPHICAL  IMPLEMENTATION");
	glRasterPos3f(-0.1, 1.4, -1.0);
	text("OF");
	glRasterPos3f(-0.85, 1.2, -1.0);
	text("ORIGAMI - PAPER PLANE SIMULATION");
	glRasterPos3f(-0.36, 0.8, -1.0);
	text("USING OPENGL");
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(1.4, -2.2, -1.2);
	text("Press c/C to continue");
	glRasterPos3f(-0.28, 0.4, -1.0);
	text("Submitted by : ");
	glRasterPos3f(-1.77, -0.2, -1.0);
	text("EYANSHU GUPTA");
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(-1.76, -0.4, -1.0);
	text("(1MV15CS034)");
	glRasterPos3f(1.15, -0.2, -1.0);
	text("PARAS JAIN");
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(1.24, -0.4, -1.0);
	text("(1MV15CS072)");
	glRasterPos3f(-1.00, -0.8, -1.0);
	text("SIR M. VISVESVARAYA INSTITUTE OF TECHNOLOGY");
	glRasterPos3f(-0.25, -1.0, -1.0);
	text("VI SEM");
	glRasterPos3f(-0.55, -1.2, -1.0);
	text("COMPUTER SCIENCE DEPT.");
	glRasterPos3f(-0.50, -1.6, -1.0);
	text("Under the guidance of : ");
	glRasterPos3f(-0.43, -1.8, -1.0);
	text("Mrs. MONIKA RANI H G");
	glRasterPos3f(-0.43, -2.0, -1.0);
	text("Asst. Professor, Dept CS&E");
	glFlush();
	glEnd();
}
void sheet2()
{
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(-0.36, 1.5, -1.0);
	text("Introduction");
	glRasterPos3f(-0.36, 1.4, -1.0);
	text("---------");
	glRasterPos3f(-2.3, 0.95, -1.0);
	text("-> This project emphasises on the graphical implementation of the simulation of an origami model of a");
	glRasterPos3f(-2.15, 0.75, -1.0);
	text("paper plane & hence to show the effectiveness of OpenGL API. ");
	glRasterPos3f(-2.3, 0.45, -1.0);
	text("-> Origami being the Japanese folk art  of paper folding transforms a flat sheet of material into a finished sculpture. ");
	glRasterPos3f(-2.3, 0.15, -1.0);
	text("-> Initially a square paper is displayed on the screen with markings to indicate the places of folding.");
	glRasterPos3f(-2.3, -0.15, -1.0);
	text("-> We then visually display the creation of a paper plane in a sequential step by step folding of the paper.");
	glRasterPos3f(-2.3, -0.45, -1.0);
	text("-> This image is animated in order to provide to the user a clear cut 3D view of the paper plane creation ");
	glRasterPos3f(-2.15, -0.65, -1.0);
	text("process at its different stages.");
	glRasterPos3f(-2.3, -0.95, -1.0);
	text("-> The paper model airplane remains a very important key in the graduation from a simple model to manned ");
	glRasterPos3f(-2.15, -1.15, -1.0);
	text("heavier than air flight.");
	glRasterPos3f(1.0, -2.2, -1.2);
	text("Press n/N to go back");
	glRasterPos3f(1.0, -2.0, -1.2);
	text("Press f/F to go to main menu");
	glFlush();
	glEnd();
}

void sheet3()
{
	glColor3f(0.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(-1.8, 1.45, -1.0);
	text("OPTIONS :");
	glRasterPos3f(-1.6, 0.85, -1.0);
	text("1. Press e/E to Execute ");
	glRasterPos3f(-1.6, 0.45, -1.0);
	text("2. Press h/H for help");
	glFlush();
	glRasterPos3f(-1.6, 0.05, -1.0);
	text("3. Press q/Q to quit");
	glRasterPos3f(1.0, -2.2, -1.2);
	text("Press i/I to go to Introduction");
	glFlush();
	glEnd();
}
void help()
{
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(-1.6, 1.95, -1.0);
	text("For usage of mouse keys to operate in the output :");
	glRasterPos3f(-1.2, 1.55, -1.0);
	text("> Click Right Button to open the Menu.");
	glRasterPos3f(-1.2, 1.25, -1.0);
	text("> Click Left Button to select the Options given in the menu.");
	glRasterPos3f(-1.6, 0.95, -1.0);
	text("For usage of keyboard keys to operate in the output :");
	glRasterPos3f(-1.2, 0.55, -1.0);
	text("> Press r/R to reverse direction of folding.");
	glRasterPos3f(-1.2, 0.25, -1.0);
	text("> Press s/S to stop spinning.");
	glRasterPos3f(-1.2, -0.05, -1.0);
	text("> Press m/M to stop motion.");
	glRasterPos3f(-1.2, -0.35, -1.0);
	text("> Press 1 for Plane color - RED.");
	glRasterPos3f(-1.2, -0.65, -1.0);
	text("> Press 2 for Plane color - BLUE.");
	glRasterPos3f(-1.2, -0.95, -1.0);
	text("> Press 3 for Plane color - YELLOW.");
	glRasterPos3f(-1.2, -1.25, -1.0);
	text("> Press 4 for Plane color - WHITE.");
	glRasterPos3f(-1.2, -1.55, -1.0);
	text("> Press 5 for Plane color - MAGENTA.");
	glRasterPos3f(1.2, -2.2, -1.2);
	text("Press b/B to go to main menu");
	glFlush();
	glEnd();
}
void draw_folded_plane(void)
{
	glPushMatrix();
	if (axis == 0)
		glRotatef(click, 0, 0, 1);
	if (axis == 1)
		glRotatef(click, 0, 1, 0);
	if (axis == 2)
		glRotatef(click, 1, 0, 0);
	glTranslatef(0, .25, 0);
	glPushMatrix();
	glRotatef(center1_angle, 0, 1, 0);
	glPushMatrix();
	glTranslatef(-.5, .5, 0);
	glRotatef(flap1_angle, 1, 1, 0);
	glTranslatef(.5, -.5, 0);
	glCallList(2);
	glPopMatrix();
	glCallList(5);

	glPushMatrix();
	glTranslatef(-1 / 3.0, 0, 0);
	glRotatef(-wing1_angle, 0, 1, 0);
	glTranslatef(1 / 3.0, 0, 0);

	glCallList(7);
	glPushMatrix();
	glTranslatef(-.5, .5, 0);
	glRotatef(flap1_angle, 1, 1, 0);
	glTranslatef(.5, -.5, 0);
	glCallList(1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-center2_angle, 0, 1, 0);
	glPushMatrix();
	glTranslatef(.5, .5, 0);
	glRotatef(-flap2_angle, -1, 1, 0);
	glTranslatef(-.5, -.5, 0);
	glCallList(3);
	glPopMatrix();
	glCallList(6);

	glPushMatrix();
	glTranslatef(1 / 3.0, 0, 0);
	glRotatef(wing2_angle, 0, 1, 0);
	glTranslatef(-1 / 3.0, 0, 0);

	glCallList(8);
	glPushMatrix();
	glTranslatef(.5, .5, 0);
	glRotatef(-flap2_angle, -1, 1, 0);
	glTranslatef(-.5, -.5, 0);
	glCallList(4);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void display(void)
{
	int i = -2.3;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (flag == 0)
		sheet1();
	else if (flag == 1)
		sheet2();
	else if (flag == 2)
		sheet3();
	else if (flag == 3)
		help();
	else
	{
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos3f(-0.85, 2.0, -1.0);
		text("ORIGAMI - PAPER PLANE SIMULATION");
		glRasterPos3f(1.0, -2.3, -1.2);
		text("Press b/B to go to main menu");
		glRasterPos3f(1.0, -2.1, -1.2);
		text("Press h/H for help");
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(red, green, blue);
#if GL_EXT_polygon_offset
		if (polygon_offset) {
			glPolygonOffset(0.5, 0.0);
			glEnable(GL_POLYGON_OFFSET_EXT);
		}
#endif
		draw_folded_plane();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3ub(0, 0, 0);
#if GL_EXT_polygon_offset
		if (polygon_offset) {
			glPolygonOffset(0.0, 0.0);
			glDisable(GL_POLYGON_OFFSET_EXT);
		}
		else {
			glPushMatrix();
			glTranslatef(0, 0, .05);
		}
#else
		glPushMatrix();
		glTranslatef(0, 0, .05);
#endif
		draw_folded_plane();
#if GL_EXT_polygon_offset
		if (!polygon_offset) {
			glPopMatrix();
		}
#else
		glPopMatrix();
#endif
	}
	glutSwapBuffers();
}
void visible(int state)
{
	if (state == GLUT_VISIBLE) {
		if (motion)
			glutIdleFunc(idle);
	}
	else {
		glutIdleFunc(NULL);
	}
}
void menu(int value)
{
	switch (value) {
	case 1:
		reverse();
		break;
	case 2:
		spin();
		break;
	case 3:
		move();
		break;
	case 4:
		axis++;
		if (axis > 2)
			axis = 0;
		break;
	case 666:
		exit(0);
	}
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A': axis++;
		if (axis > 2)
			axis = 0;
	case 'c':
	case 'C':if (flag == 0)
		flag = 1;
		break;
	case 'n':
	case 'N':if (flag == 1)
		flag = 0;
		break;
	case 'f':
	case 'F':if (flag == 1)
		flag = 2;
		break;
	case 'i':
	case 'I':if (flag == 2)
		flag = 1;
		break;
	case 'e':
	case 'E':if (flag == 2)
		flag = 5;
		break;
	case 'h':
	case 'H':flag = 3;
		break;
	case 'q':
	case 'Q':exit(0);
	case 'r':
	case 'R':reverse();
		break;
	case 's':
	case 'S':spin();
		break;
	case 'm':
	case 'M':move();
		break;
	case 'b':
	case 'B':flag = 2;
		break;
	case '1':Planecolor(6);
		display();
		break;
	case '2':Planecolor(7);
		break;
	case '3':Planecolor(8);
		break;
	case '4':Planecolor(9);
		break;
	case '5':Planecolor(0);
		break;
	case 27: break;
	}
	display();
}
int main(int argc, char **argv)
{
	int planecolor;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1300, 650);
	glutCreateWindow("origami");
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutVisibilityFunc(visible);
	//glClearColor(.34, .33, .75, 1.0);
	glClearColor(0.18, 2.26, 0.91, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(40.0, 1.0, 0.1, 10.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 5.5, 0, 0, 0, 0, 1, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glLineWidth(2.0);
	polydlist(1, sizeof(poly1) / sizeof(Point), poly1);
	polydlist(2, sizeof(poly2) / sizeof(Point), poly2);
	polydlist(3, sizeof(poly3) / sizeof(Point), poly3);
	polydlist(4, sizeof(poly4) / sizeof(Point), poly4);
	polydlist(5, sizeof(poly5) / sizeof(Point), poly5);
	polydlist(6, sizeof(poly6) / sizeof(Point), poly6);
	polydlist(7, sizeof(poly7) / sizeof(Point), poly7);
	polydlist(8, sizeof(poly8) / sizeof(Point), poly8);
	planecolor = glutCreateMenu(Planecolor);
	glutAddMenuEntry("RED", 1);
	glutAddMenuEntry("BLUE", 2);
	glutAddMenuEntry("YELLOW", 3);
	glutAddMenuEntry("WHITE", 4);
	glutAddMenuEntry("MAGNETA", 5);
	glutCreateMenu(menu);
	glutAddMenuEntry("Reverse direction", 1);
	glutAddMenuEntry("Toggle spinning", 2);
	glutAddMenuEntry("Toggle motion", 3);
	glutAddMenuEntry("Toggle axis", 4);
	glutAddSubMenu("Paper plane color", planecolor);
	glutAddMenuEntry("Quit", 666);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
#if GL_EXT_polygon_offset
	polygon_offset = glutExtensionSupported("GL_EXT_polygon_offset");
#endif
	glutMainLoop();
	return 0;
}
