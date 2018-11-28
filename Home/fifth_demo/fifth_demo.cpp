
// first_demo.cpp: �������̨Ӧ�ó������ڵ㡣

#include "stdafx.h"
#include <iostream>
#include "gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

using namespace std;

int main() {

	const char *srcPath = "./American_MUL.bmp";        //����ͼ��·��
	const char *srcPath1 = "./American_PAN.bmp";       //����ͼ��·��
	const char *dstPath = "./res7.tif";                //���ͼ��·��

	float tran1[3][3] = { { 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f }, 
	{ -sqrt(2.0f) / 6.0f, -sqrt(2.0f) / 6.0f, sqrt(2.0f) / 3.0f }, { 1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f), 0 } };

	//IHS == > RGB ��任����
	float tran2[3][3] = { { 1.0f, -1.0f / sqrt(2.0f), 1.0f / sqrt(2.0f) }, 
	{ 1.0f, -1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f) }, { 1.0f, sqrt(2.0f), 0 } };

	GDALDataset *poSrcDs;                       //����ͼ��
	GDALDataset *poSrcDs1;                      //����ͼ��
	GDALDataset *poDstDs;                       //���ͼ��
	int imgXlen = 0;                            //ͼ����
	int imgYlen = 0;                            //ͼ��߶�
	int bandNum = 0;                            //ͼ�񲨶���

	float *buffTmp1;
	float *trans_in[3];                         //�м����, �洢 R G B, ÿһ�д�С imgXlen * imgYlen, 3��
	float *trans_res[3];                        //�м����
	float *trans_res1[3];                       //���

	GDALAllRegister();                          //ע������
	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	poSrcDs1 = (GDALDataset*)GDALOpenShared(srcPath1, GA_ReadOnly);

	//��ȡͼ��߶�,���,�벨������
	imgXlen = poSrcDs->GetRasterXSize();
	imgYlen = poSrcDs->GetRasterYSize();
	bandNum = poSrcDs->GetRasterCount();

	//�����ȡ�Ľ��
	cout << " Image X Length: " << imgXlen << endl;
	cout << " Image Y Length: " << imgYlen << endl;
	cout << " Band Number: " << bandNum << endl;

	//����ͼ��ĸ߶ȺͿ�ȷ����ڴ�
	for (int i = 0; i < bandNum; ++i) {
		trans_in[i] = (float*)CPLMalloc(imgXlen * imgYlen * sizeof(float));
		trans_res[i] = (float*)CPLMalloc(imgXlen * imgYlen * sizeof(float));
		trans_res1[i] = (float*)CPLMalloc(imgXlen * imgYlen * sizeof(float));
	}
	buffTmp1 = (float*)CPLMalloc(imgXlen * imgYlen * sizeof(float));
	//�������ͼ��
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//һ�������ε�����, һ�������ε����
	for (int i = 0; i < bandNum; ++i) {
		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, trans_in[i], imgXlen, imgYlen, GDT_Float32, 0, 0);
	}
	//PAN�е�һ��ͨ���е�ֵ
	poSrcDs1->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp1, imgXlen, imgYlen, GDT_Float32, 0, 0);
	//���� RGB ==> IHS ���任
	//trans_in����ֵ
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < imgXlen * imgYlen; ++j) {
			for (int k = 0; k < 3; ++k) {
				trans_res[i][j] += (tran1[i][k] * trans_in[k][j]);
			}
		}
	}
	//���� PAN �滻 I ����
	trans_res[0] = buffTmp1;
	//��任
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < imgXlen * imgYlen; ++j) {
			for (int k = 0; k < 3; ++k) {
				trans_res1[i][j] += (tran2[i][k] * trans_res[k][j]);
			}
		}
		poDstDs->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, trans_res1[i], imgXlen, imgYlen, GDT_Float32, 0, 0);
	}

	CPLFree(buffTmp1);               //����ڴ�
	GDALClose(poDstDs);              //�ر�dataset
	GDALClose(poSrcDs);
	GDALClose(poSrcDs1);

	system("pause");
	return 0;
}

