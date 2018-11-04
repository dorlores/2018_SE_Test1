
// first_demo.cpp: �������̨Ӧ�ó������ڵ㡣
//
//����Ŀ����Ŀ⿽������ĿĿ¼��, �Ϳ��Բ������û���

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

	GDALDataset *poSrcDs;                       //����ͼ��
	GDALDataset *poDstDs;                       //���ͼ��
	int imgXlen = 0;                            //ͼ����
	int imgYlen = 0;                            //ͼ��߶�
	int bandNum = 0;                            //ͼ�񲨶���

	GByte *buffTmp;                             //ͼ���ڴ�洢
	GByte *buffTmp1;                            //������ͼ��洢

	GDALAllRegister();                          //ע������
	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//��ȡͼ��߶�,���,�벨������
	imgXlen = poSrcDs->GetRasterXSize();
	imgYlen = poSrcDs->GetRasterYSize();
	bandNum = poSrcDs->GetRasterCount();

	//����ͼ��ĸ߶ȺͿ�ȷ����ڴ�
	buffTmp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	buffTmp1 = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	//��ʼ��
	CPLRealloc(buffTmp1, imgXlen * imgYlen * sizeof(GByte));
	//�������ͼ��
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//һ�������ε�����, һ�������ε����
	int edge = (n - 1) / 2;
	for (int i = 0; i < bandNum; i++) {
		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp1, imgXlen, imgYlen, GDT_Byte, 0, 0);
		//�м���в���
 		vector<double> v;
		for (int i = edge; i < imgYlen - edge; i++) {
			for (int j = edge; j < imgXlen - edge; j++) {
				//��buffertmp�иõ���Χ����Ӧ�ľ���push��vector��, ���ú���, ��������
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

	CPLFree(buffTmp);                //����ڴ�
	CPLFree(buffTmp1);
	GDALClose(poDstDs);              //�ر�dataset
	GDALClose(poSrcDs);
}


int main() {

	const char *srcPath = "./lena.jpg";
	const char *dstPath = "./res5_6.tif";
	//kernel ����Ĵ�С
	int n = 0;
	//����
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

