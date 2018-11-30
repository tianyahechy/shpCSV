#include "myShp.h"


myShp::myShp()
{
}


myShp::~myShp()
{
}

//��ȡʸ�� ,
void myShp::readVectorFile(const char * pszVectorFileName,
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
	//��ȡͼ���е����Ա���ͷ�����
	std::cout << "���Ա��ṹ��Ϣ��" << std::endl;
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
		std::cout << "���ȣ�" << fieldWidth << std::endl;
		std::cout << "���ȣ�" << precision << std::endl;
		strDefnVec.push_back(strFieldName);

	}
	//���ͼ���е�Ҫ�ظ���
	int featureCount = poLayer->GetFeatureCount();
	std::cout << "ͼ��Ҫ�ظ���:" << featureCount << std::endl;

	//����ͼ���е�Ҫ��,��ȡ��������
	for (int i = 0; i < featureCount; i++)
	{
		std::cout << "��ǰ������" << i << "��Ҫ��" << std::endl;
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
void  myShp::readVectorFileWithID(
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
	//��ȡͼ���е����Ա���ͷ�����
	std::cout << "���Ա��ṹ��Ϣ��" << std::endl;
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
			std::cout << "���ȣ�" << fieldWidth << std::endl;
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
		std::cout << "��ǰ������" << i << "��Ҫ��" << std::endl;
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
void myShp::getPolygonVectorFromShpFile(
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
		std::cout << "��ǰ������" << i << "��Ҫ��" << std::endl;
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

//��ȡʸ���ļ��е�ͼԪ ,
void myShp::getGeometryVectorFromShpFile(
	const char * pszVectorFileName,
	std::vector<OGRGeometry*>& geometryVector
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
		std::cout << "��ǰ������" << i << "��Ҫ��" << std::endl;
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

		geometryVector.push_back(poGeometry);

	}
	GDALClose(poDS);

}
//��ȡʸ���ļ��еĶ���� ,
void myShp::getPolygonAndMulitiPolygonVectorFromShpFile(
	const char * pszVectorFileName,
	std::vector<OGRPolygon*>& polygonVector,
	std::vector<OGRMultiPolygon*> &multiPolygonVector
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
		std::cout << "��ǰ������" << i << "��Ҫ��" << std::endl;
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
			multiPolygonVector.push_back(theMulti);
		}

	}
	GDALClose(poDS);

}
//д�ļ�
void myShp::writeCSV(
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
	for (int i = 0; i < defnNumber - 1; i++)
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
//����shp�ļ�
void myShp::createShpVector(std::vector<OGRPolygon*> polygonVector, std::string strShpFileName)
{
	//�������Ϊ�գ��򷵻�
	int theSize = polygonVector.size();
	if (theSize == 0)
	{
		return;
	}
	//Ϊ��֧������·������������Ĵ���
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//Ϊ��֧�������������ƣ���������Ĵ���
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//ע����������
	GDALAllRegister();
	//����shp�ļ�
	const char * pszDriverName = "ESRI Shapefile";
	GDALDriver * poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (!poDriver)
	{
		std::cout << pszDriverName << "���������ã�" << std::endl;
		return;
	}
	std::cout << "��������!" << std::endl;
	//�������ݼ�
	GDALDataset * poDS = poDriver->Create(strShpFileName.c_str(), 0, 0, 0, GDALDataType::GDT_Unknown, NULL);
	if (poDS == NULL)
	{
		std::cout << "����ʸ���ļ�" << strShpFileName << "ʧ��!" << std::endl;
		return;
	}
	std::cout << "����ʸ���ļ�" << strShpFileName << "�ɹ�!" << std::endl;
	//����ͼ��
	OGRLayer * poLayer = poDS->CreateLayer("testLayer", NULL, wkbPolygon, NULL);
	if (poLayer == NULL)
	{
		std::cout << "����ͼ��ʧ��!" << std::endl;
		return;
	}
	std::cout << "����ͼ��ɹ���" << std::endl;
	OGRFeatureDefn * poDefn = poLayer->GetLayerDefn();
	//����Ҫ��
	for (int i = 0; i < theSize; i++)
	{
		OGRFeature * theFeature = OGRFeature::CreateFeature(poDefn);
		OGRPolygon * thePolygon = polygonVector[i];
		theFeature->SetGeometry(thePolygon);
		poLayer->CreateFeature(theFeature);
		OGRFeature::DestroyFeature(theFeature);
	}

	GDALClose(poDS);
}

//����shp�ļ�
void myShp::createShpVectorByGeometryVector(
	std::vector<OGRGeometry*> geometryVector,
	std::string strShpFileName)
{
	//�������Ϊ�գ��򷵻�
	int theSize = geometryVector.size();
	if (theSize == 0)
	{
		return;
	}
	//Ϊ��֧������·������������Ĵ���
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//Ϊ��֧�������������ƣ���������Ĵ���
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//ע����������
	GDALAllRegister();
	//����shp�ļ�
	const char * pszDriverName = "ESRI Shapefile";
	GDALDriver * poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (!poDriver)
	{
		std::cout << pszDriverName << "���������ã�" << std::endl;
		return;
	}
	std::cout << "��������!" << std::endl;
	//�������ݼ�
	GDALDataset * poDS = poDriver->Create(strShpFileName.c_str(), 0, 0, 0, GDALDataType::GDT_Unknown, NULL);
	if (poDS == NULL)
	{
		std::cout << "����ʸ���ļ�" << strShpFileName << "ʧ��!" << std::endl;
		return;
	}
	std::cout << "����ʸ���ļ�" << strShpFileName << "�ɹ�!" << std::endl;
	//����ͼ��
	OGRLayer * poLayer = poDS->CreateLayer("testLayer", NULL, wkbUnknown, NULL);
	if (poLayer == NULL)
	{
		std::cout << "����ͼ��ʧ��!" << std::endl;
		return;
	}
	std::cout << "����ͼ��ɹ���" << std::endl;
	OGRFeatureDefn * poDefn = poLayer->GetLayerDefn();
	//����Ҫ��
	for (int i = 0; i < theSize; i++)
	{
		OGRFeature * theFeature = OGRFeature::CreateFeature(poDefn);
		theFeature->SetGeometry(geometryVector[i]);
		poLayer->CreateFeature(theFeature);
		OGRFeature::DestroyFeature(theFeature);
	}

	GDALClose(poDS);
}

//�ϲ�ͼԪ����õ���ͼԪ����
bool myShp::unionElement(std::vector<OGRPolygon*>& polygonVector)
{
	//�����Ŀ<=1���򷵻�
	int theSize = polygonVector.size();
	if (theSize <= 1)
	{
		return false;
	}
	//�������1������������ͼԪ����
	std::vector<OGRGeometry*> polygonVecTemp1;
	polygonVecTemp1.clear();
	std::vector<OGRGeometry*> polygonVecTemp2;
	polygonVecTemp2.clear();
	for (int i = 0; i < theSize; i++)
	{
		polygonVecTemp2.push_back(polygonVector[i]);
	}
	int size1 = polygonVecTemp1.size();
	int size2 = polygonVecTemp2.size();
	//�ж��ಢ��־λ�Ƿ�Ϊ��
	std::vector<bool> bUnionVec;
	bUnionVec.clear();
	bool bInterSect = false;
	//����������ཻ����������Լ���ѭ����ȥ
	do
	{
		//ѭ��ǰ���ȸ�ֵ�Ƿ��ཻΪfalse
		bInterSect = false;
		polygonVecTemp1.clear();
		polygonVecTemp1 = polygonVecTemp2;
		polygonVecTemp2.clear();
		size1 = polygonVecTemp1.size();
		bUnionVec.clear();
		//��־λ����Ϊfalse,��û���ಢ
		for (int i = 0; i < size1; i++)
		{
			bUnionVec.push_back(false);
		}
		//��һ�������������ཻ���ಢ����
		for (int i = 0; i < size1 - 1; i++)
		{
			//����Ѿ��ಢ���򷵻�
			if (bUnionVec[i])
			{
				continue;
			}
			for (int j = i + 1; j < size1; j++)
			{
				if (bUnionVec[j])
				{
					continue;
				}
				OGRGeometry * polygon1 = polygonVecTemp1[i];
				OGRGeometry * polygon2 = polygonVecTemp1[j];
				if (!polygon1 || !polygon2)
				{
					continue;
				}
				//��������ཻ�����ཻΪtrue��������ֵ����polygonVecTemp2
				if (polygon1->Intersect(polygon2))
				{
					OGRGeometry * polygonUnion = polygon1->Union(polygon2);
					if (polygonUnion)
					{
						bInterSect = true;
						polygonVecTemp2.push_back(polygonUnion);
						//��־λ��Ϊtrue
						bUnionVec[i] = true;
						bUnionVec[j] = true;
					}
				}
			}
		}
		//��û���ཻ��ͼԪҲ����
		for (int i = 0; i < size1; i++)
		{
			//�����λ�ಢ�����ټ���
			if (bUnionVec[i])
			{
				continue;
			}
			polygonVecTemp2.push_back(polygonVecTemp1[i]);
		}
	} while (bInterSect);

	polygonVector.clear();
	size1 = polygonVecTemp2.size();
	for (int i = 0; i < size1; i++)
	{
		OGRGeometry * theGeometry = polygonVecTemp2[i];
		OGRPolygon * thePolygon = (OGRPolygon *)theGeometry;
		polygonVector.push_back(thePolygon);
	}
	polygonVecTemp1.clear();
	polygonVecTemp2.clear();

}
bool myShp::unionGeometryVector(std::vector<OGRGeometry*>& geometryVector)
{
	//�����Ŀ<=1���򷵻�
	int theSize = geometryVector.size();
	if (theSize <= 1)
	{
		return false;
	}
	//�������1������������ͼԪ����
	std::vector<OGRGeometry*> polygonVecTemp1;
	polygonVecTemp1.clear();
	std::vector<OGRGeometry*> polygonVecTemp2;
	polygonVecTemp2.clear();
	polygonVecTemp2 = geometryVector;
	int size1 = polygonVecTemp1.size();
	int size2 = polygonVecTemp2.size();
	//�ж��ಢ��־λ�Ƿ�Ϊ��
	std::vector<bool> bUnionVec;
	bUnionVec.clear();
	bool bInterSect = false;
	//����������ཻ����������Լ���ѭ����ȥ
	do
	{
		//ѭ��ǰ���ȸ�ֵ�Ƿ��ཻΪfalse
		bInterSect = false;
		polygonVecTemp1.clear();
		polygonVecTemp1 = polygonVecTemp2;
		polygonVecTemp2.clear();
		size1 = polygonVecTemp1.size();
		bUnionVec.clear();
		//��־λ����Ϊfalse,��û���ಢ
		for (int i = 0; i < size1; i++)
		{
			bUnionVec.push_back(false);
		}
		//��һ�������������ཻ���ಢ����
		for (int i = 0; i < size1 - 1; i++)
		{
			//����Ѿ��ಢ���򷵻�
			if (bUnionVec[i])
			{
				continue;
			}
			for (int j = i + 1; j < size1; j++)
			{
				if (bUnionVec[j])
				{
					continue;
				}
				OGRGeometry * polygon1 = polygonVecTemp1[i];
				OGRGeometry * polygon2 = polygonVecTemp1[j];
				if (!polygon1 || !polygon2)
				{
					continue;
				}
				//��������ཻ�����ཻΪtrue��������ֵ����polygonVecTemp2
				if (polygon1->Intersect(polygon2))
				{
					OGRGeometry * polygonUnion = polygon1->Union(polygon2);
					if (polygonUnion)
					{
						bInterSect = true;
						polygonVecTemp2.push_back(polygonUnion);
						//��־λ��Ϊtrue
						bUnionVec[i] = true;
						bUnionVec[j] = true;
					}
				}
			}
		}
		//��û���ཻ��ͼԪҲ����
		for (int i = 0; i < size1; i++)
		{
			//�����λ�ಢ�����ټ���
			if (bUnionVec[i])
			{
				continue;
			}
			polygonVecTemp2.push_back(polygonVecTemp1[i]);
		}
	} while (bInterSect);

	geometryVector.clear();
	geometryVector = polygonVecTemp2;
	polygonVecTemp1.clear();
	polygonVecTemp2.clear();

}