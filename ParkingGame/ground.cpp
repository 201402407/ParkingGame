#include "include.h"
#include "ground.h"
#include <Windows.h>
#include <atlbase.h>
#include <iostream>

Ground::Ground(pcStr heightFile, pcStr surfaceTexFile, GLuint width, GLint height) {

	USES_CONVERSION; // ��ũ�� ����

	_map_width = width;// �� ���μ��� ũ�� ����
	_map_height = height;

	img = NULL;// ���� ������ ��Ʈ���̹��� ������ �ʱ�ȭ
	_texId_ground = 0;//�ؽ��� id��
	_height = 0;

	/* �����ؽ��� ���� �� ���� */
	wchar_t text[50];
	mbstowcs(text, surfaceTexFile, strlen(surfaceTexFile) + 1);
	LPCWSTR LPsurfaceTexFile = text;

	img = auxDIBImageLoad(W2A(LPsurfaceTexFile));//�����ؽ����̹��� �ε�
	glGenTextures(1, &_texId_ground);// �ؽ��� �����
	glBindTexture(GL_TEXTURE_2D, _texId_ground); //�ؽ���id�� ����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);//2d�ؽ����̹�������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// ���εɹ�ü�� ������� �ؽ��� �������� ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// ���ι�ü�� Ŭ��� �ؽ����� Ȯ�� ����� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	_height = readRawData(heightFile, width, height);// ���Ϸκ��� ���μ��θ�ŭ�� ���� �����͸� �о��
	
	// ������ �ּҰ� ����
	int i;
	for (i = 0, minHeight = 2 ^ 10; i<_map_width*height; i++) {
		if (minHeight>_height[i]) { minHeight = _height[i];
		cout << minHeight << endl;
		}
	}
	cout << "asgag" << endl;
	// ���� Ÿ�ϸ��ϱ�
	baseGround = glGenLists(1);// 4�ܰ��� ���� Ÿ��, �� 5���� ���÷��̸���Ʈ
	glNewList(baseGround, GL_COMPILE);
	TileGround(1);//detail level1 ����
	glEndList();

}

Ground::~Ground() {
	// Ground��ü�� �Ҹ��� �޸𸮸� ��ȯ�Ѵ�
	if (_height != 0) { delete[] _height; }
	if (_texId_ground != 0) { glDeleteTextures(1, &_texId_ground); }
	if (img != 0) { free(img->data); free(img); }
}

void Ground::RenderGround(GLfloat x, GLfloat y) {
	int i, j;
	// Ÿ�ϸ��� ���� ���÷��̸���Ʈ���� �ҷ����� �Լ�
	//detail level1 ����
	//ī�޶� ��ġ�� ���� �� ���� �׸���.
	glPushMatrix();
	glTranslatef(getTileX(x)*(_map_width - 1), 0.0, getTileY(y)*(_map_height - 1));
	glCallList(baseGround);
	glPopMatrix();
}
void Ground::TileGround(GLint levelOfDetail) { //levelOfDetail�Ķ���͸� �޾� �� Ÿ���� �׸��� �Լ�
	glEnable(GL_TEXTURE_2D); //�ؽ���Ȱ��ȭ
	glMatrixMode(GL_MODELVIEW);//�𵨺����

	glPushMatrix();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//�ؽ���ȯ�漳��
	glBindTexture(GL_TEXTURE_2D, _texId_ground); //�����ؽ��� id�� ����Ű�� �ؽ��ķ� �Ʒ� ������ ����
	float coordX = (float)1.0 / _map_width; // �����̵�����
	float coordZ = (float)1.0 / _map_height;// �����̵�����
	for (int z = 0; z<_map_height; z += levelOfDetail) { // x,z���� �������� �׸� ������ �ٴڿ� �����ϱ� ����
		glBegin(GL_TRIANGLE_STRIP);// ���ӻﰢ�� �׸���� �ؽ��ĸ���
		for (int x = 0; x<_map_width; x += levelOfDetail) {
			glTexCoord2f(coordX*x, coordZ*z); // �ؽ���������
			glVertex3f(x, _height[z*_map_height + x], z);// ��ü���� ����
			glTexCoord2f(coordX*x, coordZ*(z + levelOfDetail));// �ؽ���������
			glVertex3f(x, _height[(z + levelOfDetail)*_map_height + x], z + levelOfDetail);// ��ü���� ����
		}
		glEnd();
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

// raw ������ �д� �Լ�
// filename: ���̸� ������ ���
// w : ���̸��� ���� ũ��
// h : ���̸��� ���� ũ��
Ground::ubyte* Ground::readRawData(pcStr filename, uint w, uint h) {
	FILE *f;
	fopen_s(&f, filename, "rb"); // �������̵����� raw���Ͽ���
	if (!f) {//������ �������
		printf("file not found : %s\n", filename);
		throw std::exception();
	}
	// ����� ���� raw ������ ���,"������ ũ��=�̹����� �ȼ���"�̱�
	// ������ width*height ũ�� ��ŭ�� ������ �Ҵ��ϸ� ��� �����͸� �о�� �� ����
	ubyte* raw = new ubyte[w*h];// raw�� ũ�⸦ ���
	fread(raw, sizeof(ubyte), w*h, f);// ���̵������о����
	fclose(f);
	return raw;
}
GLfloat Ground::getHeight(GLfloat _x, GLfloat _y) {
	//�ؽ����̹����� �ش� _x,_y���� ���� �����͸� ������ ���� �Լ�
	int x, y;
	if (_x<0) x = _map_width + ((int)_x%_map_width);//�ؽ����̹��������� �Ѿ�� �ݴ�����ǥ��
	else x = ((int)_x % _map_width);
	if (_y<0) y = _map_height + ((int)_y % _map_height);
	else y = ((int)_y % _map_height);
	// ��ǥ���� �ؽ���ũ��ȿ� ������Ŵ
	// ���� �����ʹ� 1�����迭�� ����Ǿ� �����Ƿ� ���ó��
	return (_height[y*_map_height + x] / 5 + 10);// �ش���ġ�� ���̰� ��ȯ
}
GLint Ground::getTileX(GLfloat x) {// ����ī�޶� ��ġ�ϰ� �ִ� �ؽ��� Ÿ���� ����ǥ�� ��ȯ : x��ǥ
	if (x>0) return ((int)x) / (int)_map_width;
	else return ((int)x) / (int)_map_width - 1;
}
GLint Ground::getTileY(GLfloat y) {//// ����ī�޶� ��ġ�ϰ� �ִ� �ؽ��� Ÿ���� ����ǥ�� ��ȯ : y��ǥ
	if (y>0) return ((int)y) / (int)_map_height;
	else return ((int)y) / (int)_map_height - 1;
}