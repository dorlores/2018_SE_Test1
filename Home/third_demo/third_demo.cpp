

// first_demo.cpp: 定义控制台应用程序的入口点。
//
//将项目所需的库拷贝到项目目录下, 就可以不用配置环境

#include "stdafx.h"
#include <iostream>
#include <utility>
#include "gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

using namespace std;

int main() {

	const char *srcPath = "./space.jpg";        //输入图像路径
	const char *srcPath1 = "./superman.jpg";
	const char *dstPath = "./res4.tif";         //输出图像路径

	GDALDataset *poSrcDs;                       //输入太空图像
	GDALDataset *poSrcDs1;                      //输入超人图像
	GDALDataset *poDstDs;                       //输出图像
	int imgXlen = 0;                            //图像宽度
	int imgYlen = 0;                            //图像高度
	int bandNum = 0;                            //图像波段数

	GByte *buffTmpSuper[3];
	GByte *buffTmp;                             //图像内存存储

	GDALAllRegister();                          //注册驱动
	//打开太空图像
	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	//打开超人图像
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
	buffTmp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	
	for (int i = 0; i < bandNum; i++) {
		buffTmpSuper[i] = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
		poSrcDs1->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmpSuper[i], imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	//创建输出图像
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//一个个波段的输入, 一个个波段的输出
	for (int k = 0; k < bandNum; k++) {
		poSrcDs->GetRasterBand(k + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		for (int i = 0; i < imgYlen; i++) {
			for (int j = 0; j < imgXlen; j++) {
				if (buffTmpSuper[0][i * imgXlen + j] > 10 && buffTmpSuper[0][i * imgXlen + j] < 160 && 
					buffTmpSuper[1][i * imgXlen + j] > 100 && buffTmpSuper[1][i * imgXlen + j] < 220 &&
					buffTmpSuper[2][i * imgXlen + j] > 10 && buffTmpSuper[2][i * imgXlen + j] < 110) {
					continue;
				}
				buffTmp[i * imgXlen + j] = buffTmpSuper[k][i * imgXlen + j];
			}
		}
		poDstDs->GetRasterBand(k + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		cout << "... ... band " << k << " processing ... ..." << endl;
	}
	
	CPLFree(buffTmp);                //清除内存
	CPLFree(buffTmpSuper[0]);
	CPLFree(buffTmpSuper[1]);
	CPLFree(buffTmpSuper[2]);
	GDALClose(poDstDs);              //关闭dataset
	GDALClose(poSrcDs);
	GDALClose(poSrcDs1);

	system("pause");
	return 0;
}

