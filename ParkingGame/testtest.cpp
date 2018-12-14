#include "include.h"
#include <fstream>
#include <assert.h>

#include "camera.h"
#include "heightfield.h"
#include "skybox.h"
#include "terrain.h"
#pragma comment(lib,"glew32.lib")

// 카메라 위치
float nx = 0;
float ny = 450;
float nz = 700;

float xpos = 85.078, ypos = 351.594, zpos = 281.033, xrot = 758, yrot = 90, angle = 0.0;
float lastx, lasty;

float bounce;
float cScale = 1.0;

bool keyPressed[256];	//키보드입력상황배열
void myKeyboard(unsigned char key, int x, int y) { keyPressed[key] = true; }
//해당배열이 true면 Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }
//해당배열이 false면 Key Up

SwiftHeightField hField;
Skybox* skybox;	//스카이박스
Camera cam;	//카메라
CCamera objCamera;
Terrain* terrain;	//지형


void camera(void) {
	int posX = (int)xpos;
	int posZ = (int)zpos;

	//glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);
	glTranslated(-xpos, -ypos, -zpos);
	printf("%.2f , %.2f , %.2f \n", xpos, ypos, zpos);
}

void display(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	gluLookAt(nx, ny, nz, 0, 0, 0, 0, 1, 0);
	//glTranslated(-xpos, -ypos, -zpos);
	//camera();
	glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
	glPushMatrix();
//	glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
	//camera();
	printf("%.2f , %.2f , %.2f \n", nx,ny,nz);
	skybox->draw(); // 스카이박스 생성
	//hField.Render();
	glPushMatrix();
	glTranslatef(0, -50, 0);
	//glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
	
	glRotatef(90, 0.0, 1.0, 0.0);
	terrain->RenderTerrain(0, 0);//지형을 그린다.좌표를 보내주는 이유는 카메라가 위치한 타일블럭의 좌표를 계산하기 위해 ppt참조
	//terrain->RenderTerrain(cam.eye.x, cam.eye.z);//지형을 그린다.좌표를 보내주는 이유는 카메라가 위치한 타일블럭의 좌표를 계산하기 위해 ppt참조
	glPopMatrix();
	glPopMatrix();
	//glPopMatrix();
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

void Idle() {//해당키가 눌려있는지 지속적으로 검사해 다중입력을 할수 있게 한다
	if (keyPressed['1'])glPolygonMode(GL_FRONT, GL_LINE);
	if (keyPressed['2'])glPolygonMode(GL_FRONT, GL_FILL);
	if (keyPressed['d']) { cam.slide(0.2, 0, 0); }
	if (keyPressed['a']) { cam.slide(-0.2, 0, 0); }
	if (keyPressed['s']) { cam.slide(0, 0, 1.0); }
	if (keyPressed['w']) { cam.slide(0, 0, -1.0); }
	if (cam.eye.y<terrain->getHeight(cam.eye.x, cam.eye.z)) { cam.slide(0, 1.0, 0); }// 간단한 Colision Detection 지표면 아래로 카메라가 내려갈때는 지표면 위로 유지시킴
	if (keyPressed['i']) { cam.pitch(-0.5); }
	if (keyPressed['k']) { cam.pitch(0.5); }
	if (keyPressed['q']) { cam.yaw(-0.5); }
	if (keyPressed['e']) { cam.yaw(0.5); }
	if (keyPressed['j']) { cam.roll(0.5); }
	if (keyPressed['l']) { cam.roll(-0.5); }
	else { cam.slide(0, 0, 0); } //아무것도 눌리지 않았을때는 이동없다고 보고 계속 모델뷰 행렬을 유지시켜준다.
	glutPostRedisplay();// 다시그리기
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
		nx += 50;
		break;
	case GLUT_KEY_F2:
		nx -= 50;
		break;
	case GLUT_KEY_F3:
		ny += 50;
		break;
	case GLUT_KEY_F4:
		ny -= 50;
		break;
	case GLUT_KEY_F5:
		nz += 50;
		break;
	case GLUT_KEY_F6:
		nz -= 50;
		break;
	case GLUT_KEY_UP:
		ny += 5;
		break;
	case GLUT_KEY_DOWN:
		ny -= 5;
		break;
	case GLUT_KEY_LEFT:
		nz += 5;
		break;
	case GLUT_KEY_RIGHT:
		nz -= 5;
		break;
	}
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

	skybox = new Skybox();
	// Terrain, Skybox객체
	terrain = new Terrain("background.raw", "down.bmp", 1024, 1024);
	// 뷰포트와 카메라 설정
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0);
	cam.setShape(60.0f, 64.0f / 48.0f, 0.5f, 1000.0f);

	// 카메라의 초기위치와 회전각
	// 설정을 마친 카메라가 처음에 어디에서 어디를 보고있을지 결정
	cam.slide(0, 100, 0);
	cam.roll(0);
	cam.yaw(180);
	cam.pitch(45);
	glutSpecialFunc(pressKey);
	// 깊이버퍼, 후면제거활성화
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutMainLoop();
	return 0;
}