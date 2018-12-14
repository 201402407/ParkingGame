#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LIST            "list.txt" //�ٲ� bitmap���� ��� cmd -> dir/b > list.txt 
#define INBITMAP        "in\\"     //bitmap���ϵ��� ��� 
#define OUTROW          "out\\"    //��ȯ�� raw������ ��� ��� 
#define CFILE           "c\\"      //.c���ϵ��� �� ���丮 ��� 

#define LIST_WIDTH_HEIGHT   "result.txt" //���ϸ�� + width + height ������ ���� 

#define WORD    unsigned short       //2byte
#define DWORD   unsigned int         //4byte
#define LONG    unsigned int         //4byte
#define BYTE    unsigned char        //1byte

#define WIDTHBYTES(bits)    (((bits)+31)/32*4)  //������ �������� 4����Ʈ�� ���
//width�� �þ�� ���� ���� �� ���� ��.. bitmap����
//raw�� �ٲܶ��� �þ width�� ���� �ϰ� �����ؾ� ��.. 

//��Ʈ�� ���Ͽ� ���� ����(���� ���) 
typedef struct tagBITMAPFILEHEAD {
	WORD    bfType;         //"BM"�̶�� ���� ������
	DWORD   bfSize;         //byte������ ��ü ���� ũ��
	WORD    bfReserved1;    //����� ����
	WORD    bfReserved2;    //����� ����
	DWORD   bfOffBits;      //���� ������ ��ġ������ �Ÿ�
}   BITMAPFILEHEADER;
//���� ��ü�� ���� ����(���� ���)
typedef struct tagBITMAPINFOHEADER {
	DWORD   biSize;             //���� ����� ũ�� 
	LONG    biWidth;            //�ȼ� ������ ������ �� 
	LONG    biHeight;           //������ ���� 
	WORD    biplanes;           //��Ʈ �÷��� ��(�׻� 1)
	WORD    biBitCount;         //�Ǽ��� ��Ʈ��(�÷�, ��� ����) ���⼭�� 24�� ó��
	DWORD   biCompression;      //���� ����
	DWORD   biSizeImage;        //������ ũ�� 
	LONG    biXPelsPerMeter;    //���� �ػ� 
	LONG    biYPelsPerMeter;    //���� �ػ�
	DWORD   biClrUsed;          //���� ��� �����
	DWORD   biClrImportant;     //�߿��� ���� �ε��� 
} BITMAPINFOHEADER;
//�ȷ�Ʈ(�ε����� ���� �÷����� �����ϱ� ���� ����ü)
//256Į���� ������ �ȷ�Ʈ �迭 ũ�� 256, Ʈ�� Į���� ��� �ε��� ������ �ƴ϶� ���� ���� 
//�׷��Ƿ� ���⼭�� Ʈ��Į�� �ٷ�Ƿ� �� ����ü�� ������� �ʴ´�.
typedef struct tagRGBQUAD {
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbreserved1;   //����� ���� 
}   RGBQUAD;

void Bitmap24ToRaw();
void MakeC(char *filename);
FILE *in, *out, *listout; //��Ʈ�� ����, �ٲ� raw����, .c��� ���� 
int main()
{
	FILE    *list;
	//FILE *in, *out;
	char file[256], filename[256];
	unsigned int i;

	char line[256];
	list = fopen(LIST, "r");

	listout = fopen(LIST_WIDTH_HEIGHT, "w");

	while (fscanf(list, "%s", line) != EOF) {
		printf("start : %s\n", line);

		strcpy(filename, line);
		for (i = 0; i<strlen(filename); i++)
			if (filename[i] == '.')
				break;
		filename[i] = '\0';  //���ϸ� ������ 

		strcpy(file, INBITMAP);
		strcat(file, line);
		if ((in = fopen(file, "rb")) == NULL) {
			printf("input file(%s) open fail!", file); exit(1);
		}

		strcpy(file, OUTROW);
		strcat(file, filename);
		strcat(file, ".raw");

		printf("out filename : %s \n", file);
		if ((out = fopen(file, "wb")) == NULL) {
			printf("output file(%s) open fail!", file); exit(2);
		}

		fprintf(listout, "%s.c", filename);
		Bitmap24ToRaw();

		fclose(in);
		fclose(out);

		MakeC(line);
	}
	fclose(list);
	fclose(listout);
	return 0;
}
void Bitmap24ToRaw()
{
	BITMAPFILEHEADER hf;    //�������� ��� 
	BITMAPINFOHEADER hInfo; //�������� ���
	BYTE *bmp;
	int i, j;

	fread(&hf.bfType, sizeof(WORD), 1, in);
	fread(&hf.bfSize, sizeof(DWORD), 1, in);
	fread(&hf.bfReserved1, sizeof(WORD), 1, in);
	fread(&hf.bfReserved2, sizeof(WORD), 1, in);
	fread(&hf.bfOffBits, sizeof(DWORD), 1, in);
	//printf("%x %lx %x %x %lx",hf.bfType, hf.bfSize, hf.bfReserved1, hf.bfReserved2, hf.bfOffBits);
	if (hf.bfType != 0x4D42) {  //"BM"
		printf("��Ʈ�� ������ �ƴϴ�.\n"); exit(1);
	}
	fread(&hInfo.biSize, sizeof(DWORD), 1, in);
	fread(&hInfo.biWidth, sizeof(LONG), 1, in);
	fread(&hInfo.biHeight, sizeof(LONG), 1, in);
	fread(&hInfo.biplanes, sizeof(WORD), 1, in);
	fread(&hInfo.biBitCount, sizeof(WORD), 1, in); //�������� ������ .. �ʿ� ����..

	fprintf(listout, "\t%d\t%d\n\n", hInfo.biWidth, hInfo.biHeight);

	int width = WIDTHBYTES(24 * hInfo.biWidth); //bitmap������ width����Ʈ ���� �ٽ� ��� 

	bmp = (BYTE *)malloc(width*hInfo.biHeight * 3);

	fseek(in, hf.bfOffBits, SEEK_SET);

	for (i = 0; i<hInfo.biHeight; i++)
		for (j = 0; j<width; j++)
			fread(&bmp[i*width + j], 1, 1, in);

	for (i = hInfo.biHeight - 1; i >= 0; i--)
		for (j = 0; j<hInfo.biWidth * 3; j += 3) {
			fwrite(&bmp[i*width + j + 2], 1, 1, out);
			fwrite(&bmp[i*width + j + 1], 1, 1, out);
			fwrite(&bmp[i*width + j], 1, 1, out);
		}
	delete[] bmp;
	//���� ��� �����  
}
void MakeC(char *filename)
{
	FILE *fin, *fout;
	char file[256], array_name[256];
	unsigned int i, j, tmp, size, num = 0;
	unsigned char cr, cg, cb;

	strcpy(file, OUTROW);
	strcat(file, filename);
	for (i = 0; i<strlen(file); i++)
		if (file[i] == '.') break;
	file[i] = '\0';
	strcat(file, ".raw");

	if ((fin = fopen(file, "rb")) == NULL) {
		printf("%s file read fail!!\n", file);
		exit(1);
	}

	strcpy(file, CFILE);
	strcat(file, filename);
	for (i = 0; i<strlen(file); i++)
		if (file[i] == '.') break;
	file[i] = '\0';

	strcat(file, ".c");
	if ((fout = fopen(file, "w")) == NULL) {
		printf("%s file write fail!!\n", file);
		exit(2);
	}

	strcpy(array_name, "img_");
	strcat(array_name, filename);
	for (i = 0; i<strlen(array_name); i++)
		if (array_name[i] == '.') break;
	array_name[i] = '\0';

	fprintf(fout, "/**************************************************************************/\n");
	fprintf(fout, "/* Header file containing C5x generated from BIN2ARAY.EXE                 */\n");
	fprintf(fout, "/* version : 0727_��ǥ����                                                */\n");
	fprintf(fout, "/* Made by OpenEyes Communication                                         */\n");
	fprintf(fout, "/**************************************************************************/\n");

	fseek(fin, 0L, SEEK_END);
	size = ftell(fin);
	rewind(fin);

	fprintf(fout, "\nconst unsigned short %s[] = { /* size=%d */\n", array_name, size);

	for (j = 0, i = 1; j<(size / 3); j++, i++)
	{
		fread(&cr, 1, 1, fin);
		fread(&cg, 1, 1, fin);
		fread(&cb, 1, 1, fin);

		//tmp = (c2<<8) | c1;
		//5: 6 : 5
		tmp = ((cr & 0xf8) << 8) | ((cg & 0xfc) << 3) | ((cb & 0xf8) >> 3);
		//fwrite((unsigned short *)&tmp, 1, 2, fout);

		fprintf(fout, "0x%04X, ", tmp);
		num += 2;

		if (i == 16) {
			fprintf(fout, "\n");
			i = 0;
		}
	}
	fprintf(fout, "\n};\n");
	printf("num=%d\n", num);
	fclose(fin);
	fclose(fout);

	printf("\nEnd of make DSP load image table\n");
	printf("**************************************************\n\n");

}