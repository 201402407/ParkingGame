#include "include.h"
#include <stdio.h> 
#include <math.h> 
float   nx, ny;
GLfloat vertices[][3] = {
	{ -10.0, -10.0,  10.0 },   // 0  
	{ -10.0,  10.0,  10.0 },   // 1 
	{ 10.0,  10.0, 10.0 },   // 2 
	{ 10.0, -10.0, 10.0 },   // 3 
	{ -10.0, -10.0, -10.0 },   // 4 
	{ -10.0, 10.0, -10.0 },   // 5 
	{ 10.0, 10.0, -10.0 },   // 6 
	{ 10.0, -10.0, -10.0 } };  // 7 

GLfloat colors[][3] = {
	{ 1.0, 0.0, 0.0 },      // red 
	{ 1.0, 1.0, 0.0 },      // yellow 
	{ 0.0, 1.0, 0.0 },      // green 
	{ 0.0, 0.0, 1.0 },      // blue 
	{ 1.0, 1.0, 1.0 },      // white 
	{ 1.0, 0.0, 1.0 } };     // magenta 

void polygon(int a, int b, int c, int d)
{
	glColor3fv(colors[a]);
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[a]);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glEnd();
}

// 6개의 면을 만든다. 
void createCube(void)
{
	polygon(0, 3, 2, 1);    // front 
	polygon(2, 3, 7, 6);    // right 
	polygon(3, 0, 4, 7);    // bottom 
	polygon(4, 5, 6, 7);    // back 
	polygon(1, 2, 6, 5);    // top 
	polygon(5, 4, 0, 1);    // right 
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);    // 깊이 활성화 
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
}

void display(void)
{
	float theta;

	theta = 1.0;
	nx = 2.0 * cos(theta);
	ny = 2.0 * sin(theta);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(nx, ny, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// (앞의 세 인자는 카메라의 위치, 중간의 세 인자는 카메라의 초점, 
	//  뒤의 세 인자는 법선벡터 방향 (0, 1, 0))으로 해줘야 세워져 보인다. 
	/*
	glBegin(GL_LINES);  // X, Y, Z 선 표시
	glColor3f(1.0, 0.0, 0.0); // X축
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0); // Y축
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);

	glColor3f(0.0, 0.0, 1.0); // Z축
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);
	glEnd();
	*/
	createCube();   // 큐브 생성 

	glFlush();
	glutSwapBuffers();
}
void pressKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		ny += 0.15;
		break;
	case GLUT_KEY_DOWN:
		ny -= 0.15;
		break;
	case GLUT_KEY_LEFT:
		nx += 0.15;
		break;
	case GLUT_KEY_RIGHT:
		nx -= 0.15;
		break;
	case GLUT_KEY_PAGE_UP:
		//nz += 0.15;
		break;
	case GLUT_KEY_PAGE_DOWN:
		//nz -= 0.15;
		break;
	}
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 500);
	glutCreateWindow("Cube");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutSpecialFunc(pressKey);
	glutMainLoop();
}