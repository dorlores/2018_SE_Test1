
// first_demo.cpp: 定义控制台应用程序的入口点。
//
//将项目所需的库拷贝到项目目录下, 就可以不用配置环境

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

using namespace std;

int convolution(const int n, const double times, vector<double> &buffer, vector<double> &kernel) {
	double res = 0;
	for (int i = 0; i < n * n; i++) {
		res += buffer[i] * kernel[i] * times;
	}
	int temp = 0;
	temp = (int)res;
	return temp < 0 ? 0 : temp > 255 ? 255 : temp;
}

void fourth_func(const char *srcPath, const char *dstPath, const int n, const double times, vector<double> &kernel) {

	GDALDataset *poSrcDs;                       //输入图像
	GDALDataset *poDstDs;                       //输出图像
	int imgXlen = 0;                            //图像宽度
	int imgYlen = 0;                            //图像高度
	int bandNum = 0;                            //图像波段数

	GByte *buffTmp;                             //图像内存存储
	GByte *buffTmp1;                            //卷积后的图像存储

	GDALAllRegister();                          //注册驱动
	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//获取图像高度,宽度,与波段数量
	imgXlen = poSrcDs->GetRasterXSize();
	imgYlen = poSrcDs->GetRasterYSize();
	bandNum = poSrcDs->GetRasterCount();

	//根据图像的高度和宽度分配内存
	buffTmp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	buffTmp1 = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	//初始化
	CPLRealloc(buffTmp1, imgXlen * imgYlen * sizeof(GByte));
	//创建输出图像
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//一个个波段的输入, 一个个波段的输出
	int edge = (n - 1) / 2;
	for (int i = 0; i < bandNum; i++) {
		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp1, imgXlen, imgYlen, GDT_Byte, 0, 0);
		//中间进行操作
 		vector<double> v;
		for (int i = edge; i < imgYlen - edge; i++) {
			for (int j = edge; j < imgXlen - edge; j++) {
				//将buffertmp中该点周围的相应的矩阵push到vector中, 调用函数, 进行运算
				for (int m = i; m < i + n; m++) {
					for (int k = (m - edge) * imgXlen + j - edge; k < (m - edge) * imgXlen + j - edge + n; k++) {
						v.push_back((double)buffTmp[k]);
					}
				}
				buffTmp1[i * imgXlen + j] = (GByte)convolution(n, times, v, kernel);
				v.clear();
			}
		}
		poDstDs->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	CPLFree(buffTmp);                //清除内存
	CPLFree(buffTmp1);
	GDALClose(poDstDs);              //关闭dataset
	GDALClose(poSrcDs);
}


int main() {

	const char *srcPath = "./lena.jpg";
	const char *dstPath = "./res5_6.tif";
	//kernel 矩阵的大小
	int n = 0;
	//倍数
	double times = 0;
	double temp = 0;
	cin >> n >> times;
	vector<double> kernel;
	for (int i = 0; i < n * n; i++) {
		cin >> temp;
		kernel.push_back(temp);
	}

	fourth_func(srcPath, dstPath, n, times, kernel);
	
	cout << "done..." << endl;
	system("pause");
	return 0;
}

