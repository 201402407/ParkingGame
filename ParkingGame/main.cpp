#include "include.h"
#include "skybox.h"
#include "camera.h"
// ī�޶� ��ġ
float nx = 0;
float ny = 0;
float nz = 2;

GLUquadricObj *mercury, *venus, *earth, *moon, *mars, *jupiter, *saturn, *uranus, *neptune, *sun;
GLuint texmain[2]; // ���� �ΰ�
GLuint tex[6]; //�ؽ��� ���� ����

GLUquadricObj *solar[3];
GLuint textureMapping[3]; //�ؽ��� ���� ����
AUX_RGBImageRec *textureBMPImg[3];

Skybox* skybox;	//��ī�̹ڽ�
Camera cam;	//ī�޶�
CCamera objCamera;

char *BMPFileName[3] = { "posy.bmp", "backGR.bmp", "background.bmp" };

AUX_RGBImageRec *LoadBMPFile(char *filename) { // �̹��� �о����
	FILE *pFile = NULL;
	if (!filename) return NULL;
	pFile = fopen(filename, "r");

	if (pFile) {
		fclose(pFile);
		return auxDIBImageLoad(filename);
	}
	return NULL;
}

void Texture() {
	glGenTextures(3, &textureMapping[0]);
	for (int i = 0; i < 3; i++) {
		solar[i] = gluNewQuadric();
	//	gluQuadricDrawStyle(solar[i], GLU_FILL);
//		gluQuadricTexture(solar[i], GL_TRUE);
	//	gluQuadricOrientation(solar[i], GLU_INSIDE);
	//	gluQuadricNormals(solar[i], GLU_NONE);
		textureBMPImg[i] = LoadBMPFile(BMPFileName[i]);
		glBindTexture(GL_TEXTURE_2D, textureMapping[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, textureBMPImg[i]->sizeX, textureBMPImg[i]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, textureBMPImg[i]->data);
	}
	int i;
	for (i = 0; i < 3; i++) {
		if (textureBMPImg[i])                            // �ؽ�ó�� �����ϴ��� Ȯ��
		{
			if (textureBMPImg[i]->data)                    // �ؽ�ó �̹����� �����ϴ��� Ȯ��
			{
				free(textureBMPImg[i]->data);                // �ؽ�ó �̹��� �޸𸮸� ����
			}

			free(textureBMPImg[i]);                        // �̹��� ����ü�� ����
		}
	}
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

int LoadGLTextures() { //�ؽ��� ����
	int i;

	AUX_RGBImageRec *textureimg[6]; //�׸� ���Ͽ��� ���� �ҽ��� ��� ���� ����� �Լ��� ��ȯ���� ������ ����
	int status = FALSE;
	memset(textureimg, 0, sizeof(void *) * 1); // �޸� ���� ������ 0���� ����� �ڵ�
	//������ �׸� ������ ���� �о� ���̴� ���ǹ�
	if ((textureimg[0] = LoadBMPFile("negz.bmp")) && (textureimg[1] = LoadBMPFile("posz.bmp"))
		&& (textureimg[2] = LoadBMPFile("posx.bmp")) && (textureimg[3] = LoadBMPFile("negx.bmp"))
		&& (textureimg[4] = LoadBMPFile("negy.bmp")) && (textureimg[5] = LoadBMPFile("posy.bmp"))) {
		glGenTextures(6, &tex[0]); // ù ��° ���� : �� ���� �ؽ��� ���� �ҽ��� ������ ������.  �� ��° : ���� ù ��° �ؽ��� ���� �ҽ� �ĺ��� �ּҰ�.
		// ���� GLuint[] �� []���� �ؽ��� ���

		for (i = 0; i<6; i++) {
			glBindTexture(GL_TEXTURE_2D, tex[i]); // ������ ������ �ؽ����� �ĺ��ڸ� ����. ù ��° : �� ��������. �� ��°: ������ ������ �ؽ��� ���� �ĺ���
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ������(GL_LINEAR)�� �ֱ�����(GL_NEAREST)
			// MIN�� ���� �ҽ��� ��ü���� Ŭ ��� , MAG�� �� �ݴ�.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // ù ��°: �� ��������.
			glTexImage2D(GL_TEXTURE_2D, 0, 3, textureimg[i]->sizeX, textureimg[i]->sizeY,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureimg[i]->data);
			// ù��°�� �ؽ��� �̹����� 1�������� 2����. �ι�° ���ڴ� �׻� 0-> MidMap. ����° ���ڴ� ���� ���� ����� ����(RGB�� 3)
			// �׹�°�� �ټ���°�� �̹����� ���� ���̿� ���� ����. ������°�� �̹����� ��迡 ���� �ȼ��β�
			// �ϰ���°�� �ؽ��� ���� �ҽ��� ���� ���(������ �׸����� �������Ƿ� RGB)
			// ������°�� ���� ���� ��ҵ��� ���� ���� Ÿ���� ��Ÿ���µ� ��� BMP���Ͽ� �־ RGB�� ������ ������ҵ��� ��ȣ ���� Byte���̹Ƿ� GL_UNSIGNED_BYTE
			// ��ȩ��°�� �ؽ��� ���� �ҽ��� ���� ����Ÿ��(�׸� ����Ÿ)�� ���� ������

		}
	}
	else
		return FALSE;

	// �Ҵ�� �޸𸮸� ����
	for (i = 0; i<6; i++) {
		if (textureimg[i]) {
			if (textureimg[i]->data)
				free(textureimg[i]->data);
			free(textureimg[i]);
		}
		else
			return FALSE;
	}

	glEnable(GL_TEXTURE_2D); //  2���� �ؽ��� ������ ����ϰڳ��� OpenGL���� �˸�
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// GL_MODULATE�� ȭ����� �ȼ� ���� �ؽ����� �ȼ� ���� ���� ���ϰ� �ȴ�
	// GL_DECAL�� ȭ����� �ȼ� ���� �ؽ����� �ȼ� ������ ��ü. �� ��� ���� ���ؼ� ���� �ؽ��İ� ������ ���� �ʴ´�
	// GL_BLEND�� ȭ����� �ȼ� ���� �ؽ����� �Ȼ� ���� ���� Ư���� ���� ����

	return TRUE;
}

/* 2D ��鿡 �ȼ��� ������ BMP���� �ε� */
GLubyte *LoadBmp(const char *Path, int *Width, int *Height)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER *fh = NULL;
	BITMAPINFOHEADER *ih;
	BYTE *pRaster;

	hFile = CreateFileA(Path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	FileSize = GetFileSize(hFile, NULL);
	fh = (BITMAPFILEHEADER *)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	int len = FileSize - fh->bfOffBits;
	pRaster = (GLubyte *)malloc(len);
	memcpy(pRaster, (BYTE *)fh + fh->bfOffBits, len);

	// RGB�� ������ �ٲ۴�.
	for (BYTE *p = pRaster; p < pRaster + len - 3; p += 3) {
		BYTE b = *p;
		*p = *(p + 2);
		*(p + 2) = b;
	}

	ih = (BITMAPINFOHEADER *)((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	*Width = ih->biWidth;
	*Height = ih->biHeight;

	free(fh);

	return pRaster;
}

/* ����ϴ� �Լ� */
void DoDisplay()
{
	GLubyte *data;
	int Width = 200;
	int Height = 200;
	glClear(GL_COLOR_BUFFER_BIT);
	data = LoadBmp("background.bmp", &Width, &Height);

	if (data != NULL) {
		glRasterPos2f(-0.5, -0.5);
		glDrawPixels(Width, Height, GL_RGB, GL_UNSIGNED_BYTE, data);
		free(data);
	}
}

GLuint texture[6]; //the array for our texture

GLfloat angle = 0.0;

GLuint LoadTexture(const char * filename, int width, int height) {
	GLuint texture;
	unsigned char * data;
	FILE* file;

	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	return texture;
}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

void setBackground() {
	texture[0] = LoadTexture("background.bmp", 256, 256); //load the texture
	texture[1] = LoadTexture("skybox\front.jpg", 256, 256); //load the texture
	texture[2] = LoadTexture("skybox\left.jpg", 256, 256); //load the texture
	texture[3] = LoadTexture("skybox\right.jpg", 256, 256); //load the texture
	texture[4] = LoadTexture("skybox\bottom.jpg", 256, 256); //load the texture
	texture[5] = LoadTexture("skybox\top.jpg", 256, 256); //load the texture
	glEnable(GL_TEXTURE_2D); //enable 2D texturing
							 //glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
							 //glEnable(GL_TEXTURE_GEN_T);
	//skybox();
	for (int i = 0; i < 6; i++) {
		FreeTexture(texture[i]);
	}
}

GLfloat vertices[][3] = {
	{ -95.0, -95.0,  95.0 },   // 0  
	{ -95.0,  95.0,  95.0 },   // 1 
	{ 95.0,  95.0, 95.0 },   // 2 
	{ 95.0, -95.0, 95.0 },   // 3 
	{ -95.0, -95.0, -95.0 },   // 4 
	{ -95.0, 95.0, -95.0 },   // 5 
	{ 95.0, 95.0, -95.0 },   // 6 
	{ 95.0, -95.0, -95.0 } };  // 7 

GLfloat colors[][3] = {
	{ 1.0, 0.0, 0.0 },      // red 
	{ 1.0, 1.0, 0.0 },      // yellow 
	{ 0.0, 1.0, 0.0 },      // green 
	{ 0.0, 0.0, 1.0 },      // blue 
	{ 1.0, 1.0, 1.0 },      // white 
	{ 1.0, 0.0, 1.0 } };     // magenta 

void polygon(int a, int b, int c, int d)
{
	//glColor3fv(colors[a]);
	glBegin(GL_POLYGON);
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[a]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[b]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[c]);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[d]);
	glEnd();

}

// 6���� ���� �����. 
void createCube(void)
{
	polygon(0, 3, 2, 1);    // front 
	polygon(2, 3, 7, 6);    // right 
	polygon(3, 0, 4, 7);    // bottom 
	polygon(4, 5, 6, 7);    // back 
	polygon(1, 2, 6, 5);    // top 
	polygon(5, 4, 0, 1);    // right 
}

void display() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	//gluLookAt(nx, ny, nz, 0, 0, 0, 0, 1, 0);
	glPushMatrix();
	glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
	glPopMatrix();
	
	skybox->draw(); // ��ī�̹ڽ� ����

	//createCube();   // ť�� ���� 

	//setBackground();
	//glutSolidSphere(1.2, 100, 20);
	//GLUquadricObj *sphere = gluNewQuadric();
	//gluQuadricTexture(sphere, GL_TRUE);
	//glPushMatrix();
	//glBindTexture(GL_TEXTURE_2D, tex[2]);

	/*
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-55.0f, -55.0f, -55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-55.0f, 55.0f, -55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(55.0f, 55.0f, -55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, -55.0f, -55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[3]); // ��
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, -55.0f, 55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(55.0f, 55.0f, 55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-55.0f, 55.0f, 55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-55.0f, -55.0f, 55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[4]); // �Ʒ�
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(55.0f, -55.0f, 55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-55.0f, -55.0f, -55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-55.0f, -55.0f, 55.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, -55.0f, -55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[5]); // ��
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, 55.0f, -55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-55.0f, 55.0f, -55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-55.0f, 55.0f, 55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, 55.0f, 55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[1]); //
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-55.0f, -55.0f, 55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-55.0f, 55.0f, 55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-55.0f, 55.0f, -55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-55.0f, -55.0f, -55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[0]); // ��
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, -55.0f, -55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(55.0f, 55.0f, -55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(55.0f, 55.0f, 55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, -55.0f, 55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[2]); // ����

	*/
	//glColor3f(0.5, 0.5, 0.5);
	//glTexCoord4f(0.0, 0.0, 1.0, 1.0);
	//glutSolidSphere(1.2, 20, 20);
	//gluSphere(solar[0], 4, 50, 30);
	//DrawGLScene();
	//glPopMatrix();
	//glBindTexture(GL_TEXTURE_2D, texmain[0]);
	//glColor3f(0.5, 0.0, 0.0);
	//DoDisplay();
	//glRectd(5.0, 5.0, -5.0, -5.0);
	// ��������� ���۸� ��ü�Ͽ� ȭ���� ����
	glutSwapBuffers();

	glFlush();

}

void pressKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		ny += 0.05;
		break;
	case GLUT_KEY_DOWN:
		ny -= 0.05;
		break;
	case GLUT_KEY_LEFT:
		nx += 0.05;
		break;
	case GLUT_KEY_RIGHT:
		nx -= 0.05;
		break;
	case GLUT_KEY_PAGE_UP:
		nz += 0.05;
		break;
	case GLUT_KEY_PAGE_DOWN:
		nz -= 0.05;
		break;
	}
}

void reshape(int w, int h) {
	float ratio = w / (float)h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 10, 100);
}

void idle_func(void)
{
	
	glutPostRedisplay();
}
#define glCheckError() _glCheckError(__FILE__, __LINE__)
const char* glGetErrorString(GLenum error)
{
	switch (error)
	{
	case GL_NO_ERROR:          return "No Error";
	case GL_INVALID_ENUM:      return "Invalid Enum";
	case GL_INVALID_VALUE:     return "Invalid Value";
	case GL_INVALID_OPERATION: return "Invalid Operation";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid Framebuffer Operation";
	case GL_OUT_OF_MEMORY:     return "Out of Memory";
	case GL_STACK_UNDERFLOW:   return "Stack Underflow";
	case GL_STACK_OVERFLOW:    return "";
	case GL_CONTEXT_LOST:      return "Context Lost";
	default:                   return "Unknown Error";
	}
}

void _glCheckError(const char *filename, int line)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf("OpenGL Error: %s (%d) [%u] %s\n", filename, line, err, glGetErrorString(err));
}
void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	// �����켳��
	glutInitWindowSize(1000, 600);
	glutCreateWindow("Parking-Game!");
	skybox = new Skybox();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �̰��ؾ� ��� ���� �Ȼ���.
	glColor3f(1.0, 1.0, 1.0);
	LoadGLTextures();
	//DrawGLScene();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glDepthFunc(GL_LESS);
	glutSpecialFunc(pressKey);
	// �̺�Ʈ�� ������ ó�� �Լ��� ���
	glutIdleFunc(idle_func);
	//loadMainGLTextures();
	// in init of main after create window
	/* Auto-generation of texture coordinates */
	GLfloat plane_coef_s[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat plane_coef_t[] = { 0.0, 1.0, 0.0, 1.0 };
	//Texture();

	// ����Ʈ�� ī�޶� ����
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0);
	cam.setShape(60.0f, 64.0f / 48.0f, 0.5f, 1000.0f);

	// ī�޶��� �ʱ���ġ�� ȸ����
	// ������ ��ģ ī�޶� ó���� ��𿡼� ��� ���������� ����
	cam.slide(0, 100, 0);
	cam.roll(0);
	cam.yaw(180);
	cam.pitch(45);

	// ���̹���, �ĸ�����Ȱ��ȭ
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCheckError();
	glutMainLoop();
}