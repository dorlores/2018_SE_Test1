

// first_demo.cpp: �������̨Ӧ�ó������ڵ㡣
//
//����Ŀ����Ŀ⿽������ĿĿ¼��, �Ϳ��Բ������û���

#include "stdafx.h"
#include <iostream>
#include <utility>
#include "gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

using namespace std;

int main() {

	const char *srcPath = "./space.jpg";        //����ͼ��·��
	const char *srcPath1 = "./superman.jpg";
	const char *dstPath = "./res4.tif";         //���ͼ��·��

	GDALDataset *poSrcDs;                       //����̫��ͼ��
	GDALDataset *poSrcDs1;                      //���볬��ͼ��
	GDALDataset *poDstDs;                       //���ͼ��
	int imgXlen = 0;                            //ͼ����
	int imgYlen = 0;                            //ͼ��߶�
	int bandNum = 0;                            //ͼ�񲨶���

	GByte *buffTmpSuper[3];
	GByte *buffTmp;                             //ͼ���ڴ�洢

	GDALAllRegister();                          //ע������
	//��̫��ͼ��
	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	//�򿪳���ͼ��
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
	buffTmp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	
	for (int i = 0; i < bandNum; i++) {
		buffTmpSuper[i] = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
		poSrcDs1->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmpSuper[i], imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	//�������ͼ��
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//һ�������ε�����, һ�������ε����
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
	
	CPLFree(buffTmp);                //����ڴ�
	CPLFree(buffTmpSuper[0]);
	CPLFree(buffTmpSuper[1]);
	CPLFree(buffTmpSuper[2]);
	GDALClose(poDstDs);              //�ر�dataset
	GDALClose(poSrcDs);
	GDALClose(poSrcDs1);

	system("pause");
	return 0;
}

