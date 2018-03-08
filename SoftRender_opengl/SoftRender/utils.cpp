#include "stdafx.h"
#include "utils.h"

#define FCMP(a, b) ((fabs(a, b) < EPSILON_E3) ? 1 : 0)

float sin_look[361];
float cos_look[361];


void Build_Sin_Cos_Tables()
{
	for (int ang = 0; ang <= 360; ang++) {
		float theta = (float)ang * PI / (float)180;
		cos_look[ang] = cos(theta);
		sin_look[ang] = sin(theta);
	}
}

float Fast_Sin(float theta)
{
	theta = fmodf(theta, 360);
	if (theta < 0)
		theta += 360;
	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;
	return sin_look[theta_int] + theta_frac * (sin_look[theta_int + 1] - sin_look[theta_int]);
}

float Fast_Cos(float theta)
{
	theta = fmodf(theta, 360);
	if (theta < 0)
		theta += 360;
	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;

	return cos_look[theta_int] + theta_frac * (cos_look[theta_int + 1] - cos_look[theta_int]);
}

float Fast_Distance(float _x, float _y, float _z)
{
	int temp;  // used for swaping
	int x, y, z; // used for algorithm

	x = fabs(_x) * 1024;
	y = fabs(_y) * 1024;
	z = fabs(_z) * 1024;

	// sort values
	if (y < x) SWAP(x, y, temp)

	if (z < y) SWAP(y, z, temp)

	if (y < x) SWAP(x, y, temp)

		int dist = (z + 11 * (y >> 5) + (x >> 2));

	// compute distance with 8% error
	return((float)(dist >> 10));
}

float Fast_Distance_3D(Vector3d *v)
{
	// this function computes the distance from the origin to x,y,z

	int temp;  // used for swaping
	int x, y, z; // used for algorithm

	// make sure values are all positive
	x = fabs((*v)(0)) * 1024;
	y = fabs((*v)(1)) * 1024;
	z = fabs((*v)(2)) * 1024;

	// sort values
	if (y < x) SWAP(x, y, temp)

	if (z < y) SWAP(y, z, temp)

	if (y < x) SWAP(x, y, temp)

	int dist = (z + 11 * (y >> 5) + (x >> 2));

	// compute distance with 8% error
	return((float)(dist >> 10));
}

float Fast_Distance_4D(Vector4d *v)
{
	// this function computes the distance from the origin to x,y,z

	int temp;  // used for swaping
	int x, y, z; // used for algorithm

	// make sure values are all positive
	x = fabs((*v)(0)) * 1024;
	y = fabs((*v)(1)) * 1024;
	z = fabs((*v)(2)) * 1024;

	// sort values
	if (y < x) SWAP(x, y, temp)

	if (z < y) SWAP(y, z, temp)

	if (y < x) SWAP(x, y, temp)

	int dist = (z + 11 * (y >> 5) + (x >> 2));

	return((float)(dist >> 10));
}

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;	// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	unsigned char	*bitmapImage;		// bitmapͼ������
	int	imageIdx = 0;		// ͼ��λ������
	unsigned char	tempRGB;	// ��������

	// �ԡ�������+����ģʽ���ļ�filename 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) return NULL;
	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// ����bitmap��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// �ر�bitmapͼ���ļ�
	fclose(filePtr);
	return bitmapImage;
}