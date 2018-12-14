#include "include.h"
#include "ground.h"
#include "skybox.h"
#include "camera.h"

#include "heightfield.h"
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

SwiftHeightField hField;

//��ü ����
Ground* ground;
Skybox* skybox;	//��ī�̹ڽ�
Camera cam;	//ī�޶�
CCamera objCamera;

bool keyPressed[256];	//Ű�����Է»�Ȳ�迭
void myKeyboard(unsigned char key, int x, int y) { keyPressed[key] = true; }
//�ش�迭�� true�� Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }
//�ش�迭�� false�� Key Up

void dispose(); // ����� ����Ǵ� �Լ�
void Display(); // display callback
void Idle(); // idle callback
void Reshape(GLint w, GLint h); // reshape callback
//void fog();

/* test */
void Init(void) {
	glEnable(GL_DEPTH_TEST);

	hField.Create("", 1024, 1024);
}

void Display() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
	skybox->draw();
	glPopMatrix();

	glScalef(1.0f, 0.2f, 1.0f);
	glPushMatrix();

	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.0f, 2.0f, -2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, 2.0f, -2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0f, 2.0f, 2.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, 2.0f, 2.0f);
	glEnd();

//	ground->RenderGround(cam.eye.x, cam.eye.z);//������ �׸���.��ǥ�� �����ִ� ������ ī�޶� ��ġ�� Ÿ�Ϻ��� ��ǥ�� ����ϱ� ���� ppt����
	glPopMatrix();
//	fog(); // ����Ʒ� �Ȱ�ȿ��
	glFlush();
	glutSwapBuffers();//������۸�
}
void Idle() {//�ش�Ű�� �����ִ��� ���������� �˻��� �����Է��� �Ҽ� �ְ� �Ѵ�
	if (keyPressed['1'])glPolygonMode(GL_FRONT, GL_LINE);
	if (keyPressed['2'])glPolygonMode(GL_FRONT, GL_FILL);
	//if (keyPressed['d']) { cam.slide(0.2, 0, 0); }
	//if (keyPressed['a']) { cam.slide(-0.2, 0, 0); }
	//if (keyPressed['s']) { cam.slide(0, 0, 1.0); }
	//if (keyPressed['w']) { cam.slide(0, 0, -1.0); }
	//if (cam.eye.y<ground->getHeight(cam.eye.x, cam.eye.z)) { cam.slide(0, 1.0, 0); }// ������ Colision Detection ��ǥ�� �Ʒ��� ī�޶� ���������� ��ǥ�� ���� ������Ŵ
	/* x�� */
	if (keyPressed['q']) { cam.pitch(-0.5); }
	if (keyPressed['w']) { cam.pitch(0.5); }
	/* y�� */
	if (keyPressed['a']) { cam.yaw(-0.5); }
	if (keyPressed['s']) { cam.yaw(0.5); }
	/* z��*/
	if (keyPressed['z']) { cam.roll(0.5); }
	if (keyPressed['x']) { cam.roll(-0.5); }
	else { cam.slide(0, 0, 0); } //�ƹ��͵� ������ �ʾ������� �̵����ٰ� ���� ��� �𵨺� ����� ���������ش�.
	glutPostRedisplay();// �ٽñ׸���
}
void Reshape(GLint w, GLint h) {//�������� �ݹ�
	glViewport(0, 0, w, h);//����Ʈ����
	glMatrixMode(GL_PROJECTION);//���������� ���� �������
	glLoadIdentity();// �������
	gluPerspective(40.0, static_cast<GLdouble>(w / h), 1.0, 3000.0);//����������
}
void dispose() {//���α׷� ����� ��ü��ȯ
	delete ground;
	delete skybox;
	delete &cam;
}
/*
void fog() {
	//GL_FOG�� Ȱ��ȭ �Ѵ�.
	glEnable(GL_FOG);
	//�Ȱ��� �����̸� �����ϴ� ���� (GL_LINEAR, GL_EXP, GL_EXP2)
	glFogi(GL_FOG_MODE, GL_EXP2);
	//Viewer�� Y�� waterLevel���� ������, �� ���ӿ� �ִٸ�
	GLfloat waterFogColor[4] = { 0.0,0.6,0.6,5.0 };
	GLfloat fogColor[4] = { 0.75,0.75,0.75,0.0 };
	if (cam.eye.y<(terrain->waterLevel - 75)) {
		//�Ȱ� ������ waterFogColor(0.0,0.6,0.6,1.0)���� �����ϰ�
		glFogfv(GL_FOG_COLOR, waterFogColor);
		//�Ȱ� �Ÿ��� waterFogDensity(0.075)�� �����ؼ� �����̶�� ������ �ְ� �Ѵ�.
		glFogf(GL_FOG_DENSITY, 0.075);
	}
	else {//�ƴϸ� 
		  //�Ȱ� ������ fogColor(0.7,0.7.0.7,1) �����ϰ�
		glFogfv(GL_FOG_COLOR, fogColor);
		//�Ȱ� �Ÿ��� fogDensity(0.002) �����ؼ� �����̶�� ������ �ְ� �Ѵ�.
		glFogf(GL_FOG_DENSITY, 0.001);
	}
}
*/
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	// �����켳��
	glutInitWindowSize(1280, 720);
	glutCreateWindow("201402407 ���ؿ�");
	// �ݹ��Լ����
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	//objCamera.Mouse_Move();
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardUp);
	// ���̹���, �ĸ�����Ȱ��ȭ
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// ���� ��ü
	ground = new Ground("map.raw", "sand.bmp", 1024, 1024);

	// Skybox(���)��ü
	skybox = new Skybox();

	// ����Ʈ�� ī�޶� ����
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0);
	cam.setShape(60.0f, 64.0f / 48.0f, 0.5f, 1000.0f);

	// ī�޶��� �ʱ���ġ�� ȸ����
	// ������ ��ģ ī�޶� ó���� ��𿡼� ��� ���������� ����
	cam.slide(0, 100, 0);
	cam.roll(0);
	cam.yaw(180);
	cam.pitch(45);

	atexit(dispose);
	printf("W: foward S: backwad A: left D: right\n");
	printf("qw: x��\n");
	printf("as: y��\n");
	printf("zx: z��\n\n");
	printf("1,2: solid, wire rendering\n");
	glutMainLoop();
	return 0;
}