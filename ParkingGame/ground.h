#include "include.h"


class Ground {
private:
	typedef const char* pcStr;
	typedef unsigned int uint;
	typedef unsigned char ubyte;

	uint _texId_ground;//�����ؽ��Ŀ� ���ؽ����� id
	AUX_RGBImageRec *img;//�ؽ����̹��� ��ü�� ����Ǵ� ������
	ubyte* _height; //�������� �迭�� �����ϴ� ������
	GLint _map_width, _map_height; //���������� ���μ���ũ��
	GLuint baseGround; //Ÿ���� �ε�����
	GLubyte minHeight; // ������������ �ּҳ���
	static ubyte* readRawData(pcStr filename, uint width, uint height); // �����������о����

public:
	Ground(pcStr heightmap, pcStr surfaceTex, GLuint width, GLint height);//����������
	~Ground(void);//�Ҹ���
	void RenderGround(GLfloat, GLfloat);//�����׸���
	void TileGround(GLint);//LoD�� ���� Ÿ�ϸ�
	GLfloat getHeight(GLfloat, GLfloat);//�ش� ��ġ�� ���̱��ϱ�
	GLint getTileX(GLfloat); // �ش���ġ�� xŸ�ϱ��ϱ�
	GLint getTileY(GLfloat);// �ش���ġ�� yŸ�ϱ��ϱ�	
};