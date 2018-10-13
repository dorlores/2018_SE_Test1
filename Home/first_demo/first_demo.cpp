// first_demo.cpp: 定义控制台应用程序的入口点。

//

//将项目所需的库拷贝到项目目录下, 就可以不用配置环境



#include "stdafx.h"

#include <iostream>

#include "gdal/gdal_priv.h"

#pragma comment(lib, "gdal_i.lib")



using namespace std;



int main() {



	GDALDataset *poSrcDs;                       //输入图像

	GDALDataset *poDstDs;                       //输出图像

	int imgXlen = 0;                            //图像宽度

	int imgYlen = 0;                            //图像高度



	const char *srcPath = "./trees.jpg";        //输入图像路径

	const char *dstPath = "./res.tif";          //输出图像路径



	GByte *buffTmp;                             //图像内存存储

	int bandNum = 0;                            //图像波段数



	GDALAllRegister();                          //注册驱动

	//打开图像

	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);



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

	//创建输出图像

	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//一个个波段的输入, 一个个波段的输出

	for (int i = 0; i < bandNum; i++) {

		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);

		poDstDs->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);

		cout << "... ... band " << i << " processing ... ..." << endl;

	}



	CPLFree(buffTmp);                //清除内存

	GDALClose(poDstDs);              //关闭dataset

	GDALClose(poSrcDs);



	system("pause");

  return 0;

}
