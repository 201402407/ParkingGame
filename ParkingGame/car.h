class Car {
public:
	Car();
	~Car();
	void drawCar();

	AUX_RGBImageRec *textureImg[8];
private:
	#define PI 3.1492
	typedef unsigned int uint;
	typedef const char * pcStr;
	uint textures[8];
	static uint _loadTexture(pcStr filename);
	void carRight();
	void carLeft();
	void carTop();
	void carFront();
	void carBack();
	void carBottom();
	void carWheel(int x, int y, int z);
};

