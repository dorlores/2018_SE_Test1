// 2018_11_27.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "math.h"
#include<iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")


int main()
{
	GDALDataset * poSrcDS_1;
	GDALDataset * poSrcDS_2;
	GDALDataset * poDstDS;
	GDALDataset * poDstDS_1;
	int imgXlen, tmpYlen, i,imgYlen,count,j,tmpXlen,countX,t;
	const char * srcPath_1 = "Mul_large.tif";
	const char * srcPath_2 = "Pan_large.tif";
	const char * dstPath = "combinationRow.tif";
	const char * dstPath_1 = "combinationBlock.tif";
	float * bandR, *bandG, *bandB;
	float * bandH, *bandS, *bandP;
	GDALAllRegister();   //注册驱动
	poSrcDS_1 = (GDALDataset *)GDALOpenShared(srcPath_1, GA_ReadOnly);
	poSrcDS_2 = (GDALDataset *)GDALOpenShared(srcPath_2, GA_ReadOnly);
	imgXlen = poSrcDS_1->GetRasterXSize();
	imgYlen = poSrcDS_1->GetRasterYSize();
	tmpXlen = 256;
	tmpYlen = 256;
	countX = imgXlen / tmpXlen;
	count = imgYlen / tmpYlen;
	cout << "Image X Length: " << imgXlen << endl;
	cout << "Image Y Length: " << imgYlen << endl;
	/*bandR = (float*)CPLMalloc(imgXlen*tmpYlen * sizeof(float));
	bandG = (float*)CPLMalloc(imgXlen*tmpYlen * sizeof(float));
	bandB = (float*)CPLMalloc(imgXlen*tmpYlen * sizeof(float));
	bandP = (float*)CPLMalloc(imgXlen*tmpYlen * sizeof(float));
	bandH = (float*)CPLMalloc(imgXlen*tmpYlen * sizeof(float));
	bandS = (float*)CPLMalloc(imgXlen*tmpYlen * sizeof(float));

	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, 3, GDT_Byte, NULL);
	for (j = 0; j < count; j++) 
	{

		poSrcDS_1->GetRasterBand(1)->RasterIO(GF_Read, 0, j*tmpYlen, imgXlen, tmpYlen, bandR, imgXlen, tmpYlen, GDT_Float32, 0, 0);
		poSrcDS_1->GetRasterBand(2)->RasterIO(GF_Read, 0, j*tmpYlen, imgXlen, tmpYlen, bandG, imgXlen, tmpYlen, GDT_Float32, 0, 0);
		poSrcDS_1->GetRasterBand(3)->RasterIO(GF_Read, 0, j*tmpYlen, imgXlen, tmpYlen, bandB, imgXlen, tmpYlen, GDT_Float32, 0, 0);
		poSrcDS_2->GetRasterBand(1)->RasterIO(GF_Read, 0, j*tmpYlen, imgXlen, tmpYlen, bandP, imgXlen, tmpYlen, GDT_Float32, 0, 0);
		for (i = 0; i < imgXlen * tmpYlen; i++) {                                        //RGB -> IHS
			bandH[i] = -sqrt(2.0f) / 6.0f*bandR[i] - sqrt(2.0f) / 6.0f*bandG[i] + sqrt(2.0f) / 3.0f*bandB[i];
			bandS[i] = 1.0f / sqrt(2.0f)*bandR[i] - 1 / sqrt(2.0f)*bandG[i];

			bandR[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] + 1.0f / sqrt(2.0f)*bandS[i];
			bandG[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] - 1.0f / sqrt(2.0f)*bandS[i];
			bandB[i] = bandP[i] + sqrt(2.0f)*bandH[i];
		}

		poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, j*tmpYlen, imgXlen, tmpYlen, bandR, imgXlen, tmpYlen, GDT_Float32, 0, 0);
		poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 0, j*tmpYlen, imgXlen, tmpYlen, bandG, imgXlen, tmpYlen, GDT_Float32, 0, 0);
		poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 0, j*tmpYlen, imgXlen, tmpYlen, bandB, imgXlen, tmpYlen, GDT_Float32, 0, 0);
	}
	cout << "Combination By Row OK! " << endl;
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);*/

	
	bandR = (float*)CPLMalloc(tmpXlen*tmpYlen * sizeof(float));
	bandG = (float*)CPLMalloc(tmpXlen*tmpYlen * sizeof(float));
	bandB = (float*)CPLMalloc(tmpXlen*tmpYlen * sizeof(float));
	bandP = (float*)CPLMalloc(tmpXlen*tmpYlen * sizeof(float));
	bandH = (float*)CPLMalloc(tmpXlen*tmpYlen * sizeof(float));
	bandS = (float*)CPLMalloc(tmpXlen*tmpYlen * sizeof(float));

	poDstDS_1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath_1, imgXlen, imgYlen, 3, GDT_Byte, NULL);
	
	for (j = 0; j < count; j++)
	{
        for (t = 0; t < countX; t++) {
			poSrcDS_1->GetRasterBand(1)->RasterIO(GF_Read, t*tmpXlen, j*tmpYlen, tmpXlen, tmpYlen, bandR, tmpXlen, tmpYlen, GDT_Float32, 0, 0);
			poSrcDS_1->GetRasterBand(2)->RasterIO(GF_Read, t*tmpXlen, j*tmpYlen, tmpXlen, tmpYlen, bandG, tmpXlen, tmpYlen, GDT_Float32, 0, 0);
			poSrcDS_1->GetRasterBand(3)->RasterIO(GF_Read, t*tmpXlen, j*tmpYlen, tmpXlen, tmpYlen, bandB, tmpXlen, tmpYlen, GDT_Float32, 0, 0);
			poSrcDS_2->GetRasterBand(1)->RasterIO(GF_Read, t*tmpXlen, j*tmpYlen, tmpXlen, tmpYlen, bandP, tmpXlen, tmpYlen, GDT_Float32, 0, 0);
			for (i = 0; i < tmpXlen * tmpYlen; i++) {                                        //RGB -> IHS
				bandH[i] = -sqrt(2.0f) / 6.0f*bandR[i] - sqrt(2.0f) / 6.0f*bandG[i] + sqrt(2.0f) / 3.0f*bandB[i];
				bandS[i] = 1.0f / sqrt(2.0f)*bandR[i] - 1 / sqrt(2.0f)*bandG[i];

				bandR[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] + 1.0f / sqrt(2.0f)*bandS[i];
				bandG[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] - 1.0f / sqrt(2.0f)*bandS[i];
				bandB[i] = bandP[i] + sqrt(2.0f)*bandH[i];
			}

			poDstDS_1->GetRasterBand(1)->RasterIO(GF_Write, t*tmpXlen, j*tmpYlen, tmpXlen, tmpYlen, bandR, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
			poDstDS_1->GetRasterBand(2)->RasterIO(GF_Write, t*tmpXlen, j*tmpYlen, tmpXlen, tmpYlen, bandG, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
			poDstDS_1->GetRasterBand(3)->RasterIO(GF_Write, t*tmpXlen, j*tmpYlen, tmpXlen, tmpYlen, bandB, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
		}
	}
	cout << "Combination By Block OK! " << endl;
	//释放、关闭
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);
	//GDALClose(poDstDS);
	GDALClose(poSrcDS_1);
	GDALClose(poSrcDS_2);

	system("PAUSE");
	return 0;

}

