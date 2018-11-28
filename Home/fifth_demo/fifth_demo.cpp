
// first_demo.cpp: 定义控制台应用程序的入口点。

#include "stdafx.h"
#include <iostream>
#include "gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

using namespace std;

int main() {

	const char *srcPath = "./American_MUL.bmp";        //输入图像路径
	const char *srcPath1 = "./American_PAN.bmp";       //输入图像路径
	const char *dstPath = "./res7.tif";                //输出图像路径

	float tran1[3][3] = { { 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f }, 
	{ -sqrt(2.0f) / 6.0f, -sqrt(2.0f) / 6.0f, sqrt(2.0f) / 3.0f }, { 1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f), 0 } };

	//IHS == > RGB 逆变换矩阵
	float tran2[3][3] = { { 1.0f, -1.0f / sqrt(2.0f), 1.0f / sqrt(2.0f) }, 
	{ 1.0f, -1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f) }, { 1.0f, sqrt(2.0f), 0 } };

	GDALDataset *poSrcDs;                       //输入图像
	GDALDataset *poSrcDs1;                      //输入图像
	GDALDataset *poDstDs;                       //输出图像
	int imgXlen = 0;                            //图像宽度
	int imgYlen = 0;                            //图像高度
	int bandNum = 0;                            //图像波段数

	float *buffTmp1;
	float *trans_in[3];                         //中间矩阵, 存储 R G B, 每一行大小 imgXlen * imgYlen, 3行
	float *trans_res[3];                        //中间矩阵
	float *trans_res1[3];                       //结果

	GDALAllRegister();                          //注册驱动
	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	poSrcDs1 = (GDALDataset*)GDALOpenShared(srcPath1, GA_ReadOnly);

	//获取图像高度,宽度,与波段数量
	imgXlen = poSrcDs->GetRasterXSize();
	imgYlen = poSrcDs->GetRasterYSize();
	bandNum = poSrcDs->GetRasterCount();

	//输出获取的结果
	cout << " Image X Length: " << imgXlen << endl;
	cout << " Image Y Length: " << imgYlen << endl;
	cout << " Band Number: " << bandNum << endl;

	//根据图像的高度和宽度分配内存
	for (int i = 0; i < bandNum; ++i) {
		trans_in[i] = (float*)CPLMalloc(imgXlen * imgYlen * sizeof(float));
		trans_res[i] = (float*)CPLMalloc(imgXlen * imgYlen * sizeof(float));
		trans_res1[i] = (float*)CPLMalloc(imgXlen * imgYlen * sizeof(float));
	}
	buffTmp1 = (float*)CPLMalloc(imgXlen * imgYlen * sizeof(float));
	//创建输出图像
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//一个个波段的输入, 一个个波段的输出
	for (int i = 0; i < bandNum; ++i) {
		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, trans_in[i], imgXlen, imgYlen, GDT_Float32, 0, 0);
	}
	//PAN中第一个通道中的值
	poSrcDs1->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp1, imgXlen, imgYlen, GDT_Float32, 0, 0);
	//进行 RGB ==> IHS 正变换
	//trans_in中有值
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < imgXlen * imgYlen; ++j) {
			for (int k = 0; k < 3; ++k) {
				trans_res[i][j] += (tran1[i][k] * trans_in[k][j]);
			}
		}
	}
	//利用 PAN 替换 I 分量
	trans_res[0] = buffTmp1;
	//逆变换
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < imgXlen * imgYlen; ++j) {
			for (int k = 0; k < 3; ++k) {
				trans_res1[i][j] += (tran2[i][k] * trans_res[k][j]);
			}
		}
		poDstDs->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, trans_res1[i], imgXlen, imgYlen, GDT_Float32, 0, 0);
	}

	CPLFree(buffTmp1);               //清除内存
	GDALClose(poDstDs);              //关闭dataset
	GDALClose(poSrcDs);
	GDALClose(poSrcDs1);

	system("pause");
	return 0;
}

