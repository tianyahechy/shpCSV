#include "ogrsf_frmts.h"
#include "ogr_api.h"
#include "gdal_priv.h"
#include <iostream>
#include <map>
#include <fstream>

//��ȡʸ�� ,
void readVectorFile(const char * pszVectorFileName, 
	std::map<int, std::vector<std::string>>& mapFeatureSet,
	std::vector<std::string>& strDefnVec
	)
{
	//Ϊ��֧������·������������Ĵ���
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//Ϊ��֧�������������ƣ���������Ĵ���
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//ע����������
	GDALAllRegister();
	//������
	GDALDataset * poDS = (GDALDataset*)GDALOpenEx(pszVectorFileName, GDAL_OF_VECTOR, NULL, NULL, NULL);
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}
	printf("������Դ�ɹ���\n");
	//��ȡͼ�����
	int layerCount = poDS->GetLayerCount();
	std::cout << "������" << layerCount << std::endl;

	//��ȡ��һ��ͼ��
	OGRLayer * poLayer = poDS->GetLayer(0);
	if (poLayer == NULL)
	{
		std::cout << "��ȡͼ��ʧ��" << std::endl;
		return;
	}
	//��ͼ����г�ʼ����
	poLayer->ResetReading();
	//��ȡͼ���е����Ա��ͷ�����
	std::cout << "���Ա�ṹ��Ϣ��" << std::endl;
	OGRFeatureDefn * poDefn = poLayer->GetLayerDefn();
	int iFieldCount = poDefn->GetFieldCount();
	for (int i = 0; i < iFieldCount; i++)
	{
		OGRFieldDefn * poField = poDefn->GetFieldDefn(i);
		std::string strFieldName = poField->GetNameRef();
		std::string strFieldTypeName = poField->GetFieldTypeName(poField->GetType());
		int fieldWidth = poField->GetWidth();
		int precision = poField->GetPrecision();
		std::cout << "��������" << strFieldName << std::endl;
		std::cout << "���ͣ�" << strFieldTypeName << std::endl;
		std::cout << "��ȣ�" << fieldWidth << std::endl;
		std::cout << "���ȣ�" << precision << std::endl;
		strDefnVec.push_back(strFieldName);

	}
	//���ͼ���е�Ҫ�ظ���
	int featureCount = poLayer->GetFeatureCount();
	std::cout << "ͼ��Ҫ�ظ���:" << featureCount << std::endl;

	//����ͼ���е�Ҫ��,��ȡ��������
	for (int i = 0; i < featureCount; i++)
	{
		std::cout << "��ǰ�����" << i << "��Ҫ��" << std::endl;
		OGRFeature * poFeature = poLayer->GetFeature(i);
		if (poFeature == NULL)
		{
			continue;
		}
		std::vector<std::string> strFieldNameVec;
		strFieldNameVec.clear();
		for (int j = 0; j < iFieldCount; j++)
		{
			OGRFieldDefn * poFieldDefn = poDefn->GetFieldDefn(j);
			const char* strFieldDefnName = poFieldDefn->GetNameRef();
			//std::cout << strFieldDefnName << std::endl;
			std::string strFieldName = poFeature->GetFieldAsString(j);
			//std::cout << strFieldName << std::endl;
			strFieldNameVec.push_back(strFieldName);
		}
		mapFeatureSet.insert(std::pair<int, std::vector<std::string>>(i, strFieldNameVec));
		strFieldNameVec.clear();
	}
	GDALClose(poDS);

}

//��ȡʸ�� ,
void readVectorFileWithID(
	const char * pszVectorFileName,
	int &mapSetID,  //ͼԪ���ϵ�ID
	bool & bHaveReadDefn,//�Ƿ��ȡ���Զ��壬���û�У����ȡ����������ˣ����ٶ�ȡ
	std::map<int, std::vector<std::string>>& mapFeatureSet,
	std::vector<std::string>& strDefnVec
	)
{
	//Ϊ��֧������·������������Ĵ���
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//Ϊ��֧�������������ƣ���������Ĵ���
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//ע����������
	GDALAllRegister();
	//������
	GDALDataset * poDS = (GDALDataset*)GDALOpenEx(pszVectorFileName, GDAL_OF_VECTOR, NULL, NULL, NULL);
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}
	printf("������Դ�ɹ���\n");
	//��ȡͼ�����
	int layerCount = poDS->GetLayerCount();
	std::cout << "������" << layerCount << std::endl;

	//��ȡ��һ��ͼ��
	OGRLayer * poLayer = poDS->GetLayer(0);
	if (poLayer == NULL)
	{
		std::cout << "��ȡͼ��ʧ��" << std::endl;
		return;
	}
	//��ͼ����г�ʼ����
	poLayer->ResetReading();
	//��ȡͼ���е����Ա��ͷ�����
	std::cout << "���Ա�ṹ��Ϣ��" << std::endl;
	OGRFeatureDefn * poDefn = poLayer->GetLayerDefn();
	int iFieldCount = poDefn->GetFieldCount();
	//�˶��Ƿ��ȡ���������Ͷ��壬���û�ж��������ȡ
	if (!bHaveReadDefn)
	{
		bHaveReadDefn = true;
		for (int i = 0; i < iFieldCount; i++)
		{
			OGRFieldDefn * poField = poDefn->GetFieldDefn(i);
			std::string strFieldName = poField->GetNameRef();
			OGRFieldType theType = poField->GetType();
			std::string strFieldTypeName = poField->GetFieldTypeName(poField->GetType());
			int fieldWidth = poField->GetWidth();
			int precision = poField->GetPrecision();
			std::cout << "��������" << strFieldName << std::endl;
			std::cout << "���ͣ�" << strFieldTypeName << std::endl;
			std::cout << "��ȣ�" << fieldWidth << std::endl;
			std::cout << "���ȣ�" << precision << std::endl;
			strDefnVec.push_back(strFieldName);

		}
	}

	//���ͼ���е�Ҫ�ظ���
	int featureCount = poLayer->GetFeatureCount();
	std::cout << "ͼ��Ҫ�ظ���:" << featureCount << std::endl;

	//����ͼ���е�Ҫ��,��ȡ��������
	for (int i = 0; i < featureCount; i++)
	{
		std::cout << "��ǰ�����" << i << "��Ҫ��" << std::endl;
		OGRFeature * poFeature = poLayer->GetFeature(i);
		if (poFeature == NULL)
		{
			continue;
		}
		//������
		OGRGeometry * poGeometry = poFeature->GetGeometryRef();
		if (poGeometry == NULL)
		{
			continue;
		}

		OGRwkbGeometryType theGeometryType = poGeometry->getGeometryType();
		
		std::vector<std::string> strFieldNameVec;
		strFieldNameVec.clear();
		for (int j = 0; j < iFieldCount; j++)
		{
			OGRFieldDefn * poFieldDefn = poDefn->GetFieldDefn(j);
			const char* strFieldDefnName = poFieldDefn->GetNameRef();
			//std::cout << strFieldDefnName << std::endl;
			std::string strFieldName = poFeature->GetFieldAsString(j);
			//std::cout << strFieldName << std::endl;
			strFieldNameVec.push_back(strFieldName);
		}
		mapFeatureSet.insert(std::pair<int, std::vector<std::string>>(mapSetID, strFieldNameVec));
		mapSetID++;
		strFieldNameVec.clear();
	}
	GDALClose(poDS);

}


//��ȡʸ���ļ��еĶ���� ,
void getPolygonVectorFromShpFile(
	const char * pszVectorFileName,
	std::vector<OGRPolygon*>& polygonVector
	)
{
	//Ϊ��֧������·������������Ĵ���
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//Ϊ��֧�������������ƣ���������Ĵ���
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//ע����������
	GDALAllRegister();
	//������
	GDALDataset * poDS = (GDALDataset*)GDALOpenEx(pszVectorFileName, GDAL_OF_VECTOR, NULL, NULL, NULL);
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}
	printf("������Դ�ɹ���\n");
	//��ȡͼ�����
	int layerCount = poDS->GetLayerCount();
	std::cout << "������" << layerCount << std::endl;

	//��ȡ��һ��ͼ��
	OGRLayer * poLayer = poDS->GetLayer(0);
	if (poLayer == NULL)
	{
		std::cout << "��ȡͼ��ʧ��" << std::endl;
		return;
	}
	//��ͼ����г�ʼ����
	poLayer->ResetReading();

	//���ͼ���е�Ҫ�ظ���
	int featureCount = poLayer->GetFeatureCount();
	std::cout << "ͼ��Ҫ�ظ���:" << featureCount << std::endl;

	//����ͼ���е�Ҫ��,������ζ�ȡ��������
	for (int i = 0; i < featureCount; i++)
	{
		std::cout << "��ǰ�����" << i << "��Ҫ��" << std::endl;
		OGRFeature * poFeature = poLayer->GetFeature(i);
		if (poFeature == NULL)
		{
			continue;
		}
		//������
		OGRGeometry * poGeometry = poFeature->GetGeometryRef();
		if (poGeometry == NULL)
		{
			continue;
		}

		OGRwkbGeometryType theGeometryType = poGeometry->getGeometryType();
		if (theGeometryType == wkbPolygon)
		{	
			OGRPolygon * thePolygon = (OGRPolygon*)poGeometry;
			polygonVector.push_back(thePolygon);
		}
		else if (theGeometryType == wkbMultiPolygon)
		{
			OGRMultiPolygon * theMulti = (OGRMultiPolygon*)poGeometry;
			for (int i = 0; i < theMulti->getNumGeometries(); i++)
			{
				OGRPolygon * thePolygon = (OGRPolygon*)theMulti->getGeometryRef(i);
				polygonVector.push_back(thePolygon);

			}
		}
	
	}
	GDALClose(poDS);

}
//д�ļ�
void writeCSV(
	std::string strCSVName,
	std::map<int, std::vector<std::string>> mapFeatureSet,
	std::vector<std::string> strDefnVec
	)
{
	//�ж����Զ����Ƿ�Ϊ��
	int defnNumber = strDefnVec.size();
	if (defnNumber == 0)
	{
		return;
	}
	std::ofstream outFile;
	outFile.open(strCSVName, std::ios::out);
	//дexcelͷ
	std::string strOutputDefn = "";
	for (int i = 0; i < defnNumber-1; i++)
	{
		std::string strDefn = strDefnVec[i]; 
		strOutputDefn += strDefn;
		strOutputDefn += ',';
		
	}
	strOutputDefn += strDefnVec[defnNumber - 1];
	outFile << strOutputDefn << std::endl;

	//д����
	int excelBodyNumber = mapFeatureSet.size();
	//���Ϊ�գ��򷵻�
	if (excelBodyNumber == 0)
	{
		return;
	}
	std::map<int, std::vector<std::string>>::iterator
		iterFeatureCur = mapFeatureSet.begin(),
		iterFeatureEnd = mapFeatureSet.end();
	for (; iterFeatureCur != iterFeatureEnd; iterFeatureCur++)
	{
		std::vector<std::string> strFeatureVec = iterFeatureCur->second;	
		int fieldCount = strFeatureVec.size();
		std::string strOutputFeature = "";
		for (int i = 0; i < fieldCount - 1; i++)
		{
			std::string strFeatureField = strFeatureVec[i];
			strOutputFeature += strFeatureField;
			strOutputFeature += ',';

		}
		strOutputFeature += strFeatureVec[fieldCount - 1];
		outFile << strOutputFeature << std::endl;

	}
	outFile.close();

}
int main()
{

	std::vector<OGRPolygon* > polygonVector;
	polygonVector.clear();
	//��ȡcut1_output.shp��cut11_output.shp
	for (int i = 1; i < 12; i++)
	{
		std::string strID = std::to_string(i);
		std::string strShpName = "E:\\ShpResult\\cut" + strID + "_OutShp.shp";
		getPolygonVectorFromShpFile(strShpName.c_str(),polygonVector);
	}
	
	//const char * pszVectorFileName = "E:\\poyangcut\\poyangcut\\shp\\poyanghu.shp";
	//const char * pszVectorFileName = "E:\\shpExcel\\PonitShp\\AllWater.shp";

	//std::map<int, std::vector<std::string>> mapFeatureSet;
	//mapFeatureSet.clear();
	//std::vector<std::string> strDefnVec;
	//strDefnVec.clear();
	//readVectorFile(pszVectorFileName, mapFeatureSet, strDefnVec);
	/*
	std::cout << "���excelͷ" << std::endl;
	for (int i = 0; i < strDefnVec.size(); i++)
	{
		std::string strDefn = strDefnVec[i];
		std::cout << strDefn << std::endl;
	}
	//�������
	std::cout << "���excel����" << std::endl;
	std::map<int, std::vector<std::string>>::iterator
		iterCurSet = mapFeatureSet.begin(),
		iterEndSet = mapFeatureSet.end();
	for (; iterCurSet != iterEndSet; iterCurSet++)
	{
		int id = iterCurSet->first;
		std::cout << "��" << id << "��Ԫ��" << std::endl;
		std::vector<std::string> featureVec = iterCurSet->second;
		for (int i = 0; i < featureVec.size(); i++)
		{
			std::string theFeatureName = featureVec[i];
			std::cout << theFeatureName << std::endl;
		}
	}
	*/
	//�����.csv
	//std::string strCSVFileName = "E:\\tifAll.csv";
	//writeCSV(strCSVFileName, mapFeatureSet, strDefnVec);

	//�����Դ
	//mapFeatureSet.clear();
	//strDefnVec.clear();
	return 0;
}