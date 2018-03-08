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
	FILE *filePtr;	// 文件指针
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
	unsigned char	*bitmapImage;		// bitmap图像数据
	int	imageIdx = 0;		// 图像位置索引
	unsigned char	tempRGB;	// 交换变量

	// 以“二进制+读”模式打开文件filename 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) return NULL;
	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// 关闭bitmap图像文件
	fclose(filePtr);
	return bitmapImage;
}