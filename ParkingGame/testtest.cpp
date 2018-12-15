#include "include.h"
#include <fstream>
#include <assert.h>

#include "camera.h"
#include "heightfield.h"
#include "skybox.h"
#include "car.h"
#include "terrain.h"
#pragma comment(lib,"glew32.lib")

// ī�޶� ��ġ
float nx = 0;
float ny = 800;
float nz = 1000;

float xpos = 85.078, ypos = 351.594, zpos = 281.033, xrot = 758, yrot = 90, angle = 0.0;
float lastx, lasty;

float bounce;
float cScale = 1.0;

bool keyPressed[256];	//Ű�����Է»�Ȳ�迭
void myKeyboard(unsigned char key, int x, int y) { keyPressed[key] = true; }
//�ش�迭�� true�� Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }
//�ش�迭�� false�� Key Up

SwiftHeightField hField;
Skybox* skybox;	//��ī�̹ڽ�
Camera cam;	//ī�޶�
CCamera objCamera;
Terrain* terrain;	//����
Car* car; // �ڵ���

void camera(void) {
	int posX = (int)xpos;
	int posZ = (int)zpos;

	//glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);
	glTranslated(-xpos, -ypos, -zpos);
	printf("%.2f , %.2f , %.2f \n", xpos, ypos, zpos);
}

/* ���ȭ�� SkyBox ���� �Լ� */
void setBackGround() {
	glPushMatrix();
	
	glRotatef(90, 0.0, 1.0, 0.0);
	//glScalef(2.0f, 2.0f, 2.0f);
	skybox->draw(); // ��ī�̹ڽ� ����
					//hField.Render();
	glPopMatrix();
}

/* �ٴ� ���� �Լ� */
void setGround() {
	glPushMatrix();
	glTranslatef(-450.0f, -300.0f, 0.0); // ī�޶� ������ ���� ��ŭ ��ġ�� ���� ��ŭ - �ؼ� Translate ��Ų��.
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(2.0, 1.0, 2.0);
	terrain->RenderTerrain(cam.eye.x, cam.eye.z);//������ �׸���.��ǥ�� �����ִ� ������ ī�޶� ��ġ�� Ÿ�Ϻ��� ��ǥ�� ����ϱ� ���� ppt����
	//hField.Render();
	glPopMatrix();
}

void display(void) {
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* VIEW ���� */
	// Perspective -> ����� ���̶�� ����. �� �� �ִ� ���� �� �� �� �ִ� �Ÿ�.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//printf("%.2f, %.2f, %.2f \n", nx, ny, nz);
	gluPerspective(120, 1.0f, 0.1f, 99999); // �ִ� �� ���� -> ����� �ȳ�����

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(nx, ny, nz, 0, 0, 0, 0, 1, 0);
	/* �⺻ ���� */
	glShadeModel(GL_SMOOTH);
	// ���̹���, �ĸ�����Ȱ��ȭ
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);

	/* ��� ���� */
	setBackGround();

	/* �ٴ� ���� */
	setGround();
	
	/* �ڵ��� ���� */
	glTranslatef(0.0, 420.0, 480.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.8, 1.5, 1.5);
	car->drawCar();
	
	glutSwapBuffers();
}

void Init(void) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	hField.Create("test2.raw", 1024, 1024);
}

void mouseMovement(int x, int y) {
	int diffx = x - lastx;
	int diffy = y - lasty;
	lastx = x;
	lasty = y;
	xrot += (float)diffy;
	yrot += (float)diffx;
}

void keyboard(unsigned char key, int x, int y) {

	if (key == 'w')
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos += float(sin(yrotrad)) * cScale;
		zpos -= float(cos(yrotrad)) * cScale;
		ypos -= float(sin(xrotrad));
		bounce += 0.04;
	}

	if (key == 's')
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos -= float(sin(yrotrad)) * cScale;
		zpos += float(cos(yrotrad)) * cScale;
		ypos += float(sin(xrotrad));
		bounce += 4.04;
	}

	if (key == 'd')
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos += float(cos(yrotrad)) * cScale;
		zpos += float(sin(yrotrad)) * cScale;
	}

	if (key == 'a')
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos -= float(cos(yrotrad)) * cScale;
		zpos -= float(sin(yrotrad)) * cScale;
	}

}

void Idle() {//�ش�Ű�� �����ִ��� ���������� �˻��� �����Է��� �Ҽ� �ְ� �Ѵ�
	if (keyPressed['1'])glPolygonMode(GL_FRONT, GL_LINE);
	if (keyPressed['2'])glPolygonMode(GL_FRONT, GL_FILL);
	if (keyPressed['d']) { cam.slide(0.2, 0, 0); }
	if (keyPressed['a']) { cam.slide(-0.2, 0, 0); }
	if (keyPressed['s']) { cam.slide(0, 0, 1.0); }
	if (keyPressed['w']) { cam.slide(0, 0, -1.0); }
	if (cam.eye.y<terrain->getHeight(cam.eye.x, cam.eye.z)) { cam.slide(0, 1.0, 0); }// ������ Colision Detection ��ǥ�� �Ʒ��� ī�޶� ���������� ��ǥ�� ���� ������Ŵ
	if (keyPressed['i']) { cam.pitch(-0.5); }
	if (keyPressed['k']) { cam.pitch(0.5); }
	if (keyPressed['q']) { cam.yaw(-0.5); }
	if (keyPressed['e']) { cam.yaw(0.5); }
	if (keyPressed['j']) { cam.roll(0.5); }
	if (keyPressed['l']) { cam.roll(-0.5); }
	else { cam.slide(0, 0, 0); } //�ƹ��͵� ������ �ʾ������� �̵����ٰ� ���� ��� �𵨺� ����� ���������ش�.
	glutPostRedisplay();// �ٽñ׸���
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void pressKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		nx += 1;
		//cam.pitch(0.5);
		break;
	case GLUT_KEY_F2:
		nx -= 1;
		//cam.pitch(-0.5);
		break;
	case GLUT_KEY_F3:
		ny += 150;
		//cam.yaw(0.5);
		break;
	case GLUT_KEY_F4:
		ny -= 50;
		//cam.yaw(-0.5);
		printf("F4");
		break;
	case GLUT_KEY_F5:
		nz += 1;
		//cam.roll(0.5);
		break;
	case GLUT_KEY_F6:
		nz -= 1;
		//cam.roll(-0.5);
		break;
	case GLUT_KEY_UP: // ������ �̵�
		nz += 50;
		break;
	case GLUT_KEY_DOWN: // �ڷ� �̵�
		nz -= 50;
		break;
	case GLUT_KEY_LEFT: // ���� �̵�
		nx += 50;
		break;
	case GLUT_KEY_RIGHT: // ������ �̵�
		nx -= 50;
		break;
	}
	glutPostRedisplay();// �ٽñ׸���
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(1200, 700);
	glutCreateWindow("Parking-Game!");
	Init();
	glutDisplayFunc(display);
	glutIdleFunc(Idle);
	glutReshapeFunc(reshape);
	//glutKeyboardFunc(keyboard);
	//glutPassiveMotionFunc(mouseMovement);
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardUp);

	// �ڵ��� ��ü
	car = new Car();
	skybox = new Skybox();
	// Terrain, Skybox��ü
	terrain = new Terrain("map2.raw", "down.bmp", 1024, 1024);
	// ����Ʈ�� ī�޶� ���� (ī�޶� ���� �Լ� ���ο� �̹� PERSPECTIVE�� MODELVIEW ������ �� �� �ְ� �ߴ�.)
	/*
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0);
	cam.setShape(120.0f, 64.0f / 48.0f, 0.5f, 2000.0f);
	printf("%.2f, %.2f, %.2f", cam.eye.x, cam.eye.y, cam.eye.z);
	// ī�޶��� �ʱ���ġ�� ȸ����
	// ������ ��ģ ī�޶� ó���� ��𿡼� ��� ���������� ����
	cam.slide(0, 100, 0);
	cam.roll(0);
	cam.yaw(10);
	cam.pitch(45);
	*/
	glutSpecialFunc(pressKey);

	glutMainLoop();
	return 0;
}