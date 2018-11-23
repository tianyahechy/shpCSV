#include "ogrsf_frmts.h"
#include "ogr_api.h"
#include "gdal_priv.h"
#include <iostream>
#include <map>
#include <fstream>

//读取矢量 ,
void readVectorFile(const char * pszVectorFileName, 
	std::map<int, std::vector<std::string>>& mapFeatureSet,
	std::vector<std::string>& strDefnVec
	)
{
	//为了支持中文路径，加上下面的代码
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//为了支持中文属性名称，加上下面的代码
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//注册所有驱动
	GDALAllRegister();
	//打开数据
	GDALDataset * poDS = (GDALDataset*)GDALOpenEx(pszVectorFileName, GDAL_OF_VECTOR, NULL, NULL, NULL);
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}
	printf("打开数据源成功！\n");
	//获取图层个数
	int layerCount = poDS->GetLayerCount();
	std::cout << "层数：" << layerCount << std::endl;

	//获取第一个图层
	OGRLayer * poLayer = poDS->GetLayer(0);
	if (poLayer == NULL)
	{
		std::cout << "获取图层失败" << std::endl;
		return;
	}
	//对图层进行初始化，
	poLayer->ResetReading();
	//获取图层中的属性表表头并输出
	std::cout << "属性表结构信息：" << std::endl;
	OGRFeatureDefn * poDefn = poLayer->GetLayerDefn();
	int iFieldCount = poDefn->GetFieldCount();
	for (int i = 0; i < iFieldCount; i++)
	{
		OGRFieldDefn * poField = poDefn->GetFieldDefn(i);
		std::string strFieldName = poField->GetNameRef();
		std::string strFieldTypeName = poField->GetFieldTypeName(poField->GetType());
		int fieldWidth = poField->GetWidth();
		int precision = poField->GetPrecision();
		std::cout << "属性名：" << strFieldName << std::endl;
		std::cout << "类型：" << strFieldTypeName << std::endl;
		std::cout << "宽度：" << fieldWidth << std::endl;
		std::cout << "精度：" << precision << std::endl;
		strDefnVec.push_back(strFieldName);

	}
	//输出图层中的要素个数
	int featureCount = poLayer->GetFeatureCount();
	std::cout << "图层要素个数:" << featureCount << std::endl;

	//遍历图层中的要素,读取到集合中
	for (int i = 0; i < featureCount; i++)
	{
		std::cout << "当前处理第" << i << "个要素" << std::endl;
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

//读取矢量 ,
void readVectorFileWithID(
	const char * pszVectorFileName,
	int &mapSetID,  //图元集合的ID
	bool & bHaveReadDefn,//是否读取属性定义，如果没有，则读取；如果读过了，则不再读取
	std::map<int, std::vector<std::string>>& mapFeatureSet,
	std::vector<std::string>& strDefnVec
	)
{
	//为了支持中文路径，加上下面的代码
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//为了支持中文属性名称，加上下面的代码
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//注册所有驱动
	GDALAllRegister();
	//打开数据
	GDALDataset * poDS = (GDALDataset*)GDALOpenEx(pszVectorFileName, GDAL_OF_VECTOR, NULL, NULL, NULL);
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}
	printf("打开数据源成功！\n");
	//获取图层个数
	int layerCount = poDS->GetLayerCount();
	std::cout << "层数：" << layerCount << std::endl;

	//获取第一个图层
	OGRLayer * poLayer = poDS->GetLayer(0);
	if (poLayer == NULL)
	{
		std::cout << "获取图层失败" << std::endl;
		return;
	}
	//对图层进行初始化，
	poLayer->ResetReading();
	//获取图层中的属性表表头并输出
	std::cout << "属性表结构信息：" << std::endl;
	OGRFeatureDefn * poDefn = poLayer->GetLayerDefn();
	int iFieldCount = poDefn->GetFieldCount();
	//潘段是否读取过属性类型定义，如果没有读过，则读取
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
			std::cout << "属性名：" << strFieldName << std::endl;
			std::cout << "类型：" << strFieldTypeName << std::endl;
			std::cout << "宽度：" << fieldWidth << std::endl;
			std::cout << "精度：" << precision << std::endl;
			strDefnVec.push_back(strFieldName);

		}
	}

	//输出图层中的要素个数
	int featureCount = poLayer->GetFeatureCount();
	std::cout << "图层要素个数:" << featureCount << std::endl;

	//遍历图层中的要素,读取到集合中
	for (int i = 0; i < featureCount; i++)
	{
		std::cout << "当前处理第" << i << "个要素" << std::endl;
		OGRFeature * poFeature = poLayer->GetFeature(i);
		if (poFeature == NULL)
		{
			continue;
		}
		//几何体
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


//读取矢量文件中的多边形 ,
void getPolygonVectorFromShpFile(
	const char * pszVectorFileName,
	std::vector<OGRPolygon*>& polygonVector
	)
{
	//为了支持中文路径，加上下面的代码
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//为了支持中文属性名称，加上下面的代码
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//注册所有驱动
	GDALAllRegister();
	//打开数据
	GDALDataset * poDS = (GDALDataset*)GDALOpenEx(pszVectorFileName, GDAL_OF_VECTOR, NULL, NULL, NULL);
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}
	printf("打开数据源成功！\n");
	//获取图层个数
	int layerCount = poDS->GetLayerCount();
	std::cout << "层数：" << layerCount << std::endl;

	//获取第一个图层
	OGRLayer * poLayer = poDS->GetLayer(0);
	if (poLayer == NULL)
	{
		std::cout << "获取图层失败" << std::endl;
		return;
	}
	//对图层进行初始化，
	poLayer->ResetReading();

	//输出图层中的要素个数
	int featureCount = poLayer->GetFeatureCount();
	std::cout << "图层要素个数:" << featureCount << std::endl;

	//遍历图层中的要素,将多边形读取到集合中
	for (int i = 0; i < featureCount; i++)
	{
		std::cout << "当前处理第" << i << "个要素" << std::endl;
		OGRFeature * poFeature = poLayer->GetFeature(i);
		if (poFeature == NULL)
		{
			continue;
		}
		//几何体
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


//读取矢量文件中的图元 ,
void getGeometryVectorFromShpFile(
	const char * pszVectorFileName,
	std::vector<OGRGeometry*>& geometryVector
	)
{
	//为了支持中文路径，加上下面的代码
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//为了支持中文属性名称，加上下面的代码
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//注册所有驱动
	GDALAllRegister();
	//打开数据
	GDALDataset * poDS = (GDALDataset*)GDALOpenEx(pszVectorFileName, GDAL_OF_VECTOR, NULL, NULL, NULL);
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}
	printf("打开数据源成功！\n");
	//获取图层个数
	int layerCount = poDS->GetLayerCount();
	std::cout << "层数：" << layerCount << std::endl;

	//获取第一个图层
	OGRLayer * poLayer = poDS->GetLayer(0);
	if (poLayer == NULL)
	{
		std::cout << "获取图层失败" << std::endl;
		return;
	}
	//对图层进行初始化，
	poLayer->ResetReading();

	//输出图层中的要素个数
	int featureCount = poLayer->GetFeatureCount();
	std::cout << "图层要素个数:" << featureCount << std::endl;

	//遍历图层中的要素,将多边形读取到集合中
	for (int i = 0; i < featureCount; i++)
	{
		std::cout << "当前处理第" << i << "个要素" << std::endl;
		OGRFeature * poFeature = poLayer->GetFeature(i);
		if (poFeature == NULL)
		{
			continue;
		}
		//几何体
		OGRGeometry * poGeometry = poFeature->GetGeometryRef();
		if (poGeometry == NULL)
		{
			continue;
		}

		geometryVector.push_back(poGeometry);

	}
	GDALClose(poDS);

}
//读取矢量文件中的多边形 ,
void getPolygonAndMulitiPolygonVectorFromShpFile(
	const char * pszVectorFileName,
	std::vector<OGRPolygon*>& polygonVector,
	std::vector<OGRMultiPolygon*> &multiPolygonVector
	)
{
	//为了支持中文路径，加上下面的代码
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//为了支持中文属性名称，加上下面的代码
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//注册所有驱动
	GDALAllRegister();
	//打开数据
	GDALDataset * poDS = (GDALDataset*)GDALOpenEx(pszVectorFileName, GDAL_OF_VECTOR, NULL, NULL, NULL);
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}
	printf("打开数据源成功！\n");
	//获取图层个数
	int layerCount = poDS->GetLayerCount();
	std::cout << "层数：" << layerCount << std::endl;

	//获取第一个图层
	OGRLayer * poLayer = poDS->GetLayer(0);
	if (poLayer == NULL)
	{
		std::cout << "获取图层失败" << std::endl;
		return;
	}
	//对图层进行初始化，
	poLayer->ResetReading();

	//输出图层中的要素个数
	int featureCount = poLayer->GetFeatureCount();
	std::cout << "图层要素个数:" << featureCount << std::endl;

	//遍历图层中的要素,将多边形读取到集合中
	for (int i = 0; i < featureCount; i++)
	{
		std::cout << "当前处理第" << i << "个要素" << std::endl;
		OGRFeature * poFeature = poLayer->GetFeature(i);
		if (poFeature == NULL)
		{
			continue;
		}
		//几何体
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
//写文件
void writeCSV(
	std::string strCSVName,
	std::map<int, std::vector<std::string>> mapFeatureSet,
	std::vector<std::string> strDefnVec
	)
{
	//判断属性定义是否为空
	int defnNumber = strDefnVec.size();
	if (defnNumber == 0)
	{
		return;
	}
	std::ofstream outFile;
	outFile.open(strCSVName, std::ios::out);
	//写excel头
	std::string strOutputDefn = "";
	for (int i = 0; i < defnNumber-1; i++)
	{
		std::string strDefn = strDefnVec[i]; 
		strOutputDefn += strDefn;
		strOutputDefn += ',';
		
	}
	strOutputDefn += strDefnVec[defnNumber - 1];
	outFile << strOutputDefn << std::endl;

	//写主体
	int excelBodyNumber = mapFeatureSet.size();
	//如果为空，则返回
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
//创建shp文件
void createShpVector(std::vector<OGRPolygon*> polygonVector, std::string strShpFileName)
{
	//如果数组为空，则返回
	int theSize = polygonVector.size();
	if (theSize == 0)
	{
		return;
	}
	//为了支持中文路径，加上下面的代码
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//为了支持中文属性名称，加上下面的代码
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//注册所有驱动
	GDALAllRegister();
	//创建shp文件
	const char * pszDriverName = "ESRI Shapefile";
	GDALDriver * poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (!poDriver)
	{
		std::cout << pszDriverName << "驱动不可用！" << std::endl;
		return;
	}
	std::cout << "驱动可用!" << std::endl;
	//创建数据集
	GDALDataset * poDS = poDriver->Create(strShpFileName.c_str(), 0, 0, 0, GDALDataType::GDT_Unknown, NULL);
	if (poDS == NULL)
	{
		std::cout << "创建矢量文件" << strShpFileName << "失败!" << std::endl;
		return;
	}
	std::cout << "创建矢量文件" << strShpFileName << "成功!" << std::endl;
	//创建图层
	OGRLayer * poLayer = poDS->CreateLayer("testLayer", NULL, wkbPolygon, NULL);
	if (poLayer == NULL)
	{
		std::cout << "创建图层失败!" << std::endl;
		return;
	}
	std::cout << "创建图层成功！" << std::endl;
	OGRFeatureDefn * poDefn = poLayer->GetLayerDefn();
	//创建要素
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

//创建shp文件
void createShpVectorByGeometryVector(
	std::vector<OGRGeometry*> geometryVector,
	std::string strShpFileName)
{
	//如果数组为空，则返回
	int theSize = geometryVector.size();
	if (theSize == 0)
	{
		return;
	}
	//为了支持中文路径，加上下面的代码
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//为了支持中文属性名称，加上下面的代码
	CPLSetConfigOption("SHAPE_ENCODING", "NO");
	//注册所有驱动
	GDALAllRegister();
	//创建shp文件
	const char * pszDriverName = "ESRI Shapefile";
	GDALDriver * poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (!poDriver)
	{
		std::cout << pszDriverName << "驱动不可用！" << std::endl;
		return;
	}
	std::cout << "驱动可用!" << std::endl;
	//创建数据集
	GDALDataset * poDS = poDriver->Create(strShpFileName.c_str(), 0, 0, 0, GDALDataType::GDT_Unknown, NULL);
	if (poDS == NULL)
	{
		std::cout << "创建矢量文件" << strShpFileName << "失败!" << std::endl;
		return;
	}
	std::cout << "创建矢量文件" << strShpFileName << "成功!" << std::endl;
	//创建图层
	OGRLayer * poLayer = poDS->CreateLayer("testLayer", NULL, wkbUnknown, NULL);
	if (poLayer == NULL)
	{
		std::cout << "创建图层失败!" << std::endl;
		return;
	}
	std::cout << "创建图层成功！" << std::endl;
	OGRFeatureDefn * poDefn = poLayer->GetLayerDefn();
	//创建要素
	for (int i = 0; i < theSize; i++)
	{
		OGRFeature * theFeature = OGRFeature::CreateFeature(poDefn);
		theFeature->SetGeometry(geometryVector[i]);
		poLayer->CreateFeature(theFeature);
		OGRFeature::DestroyFeature(theFeature);
	}

	GDALClose(poDS);
}

//合并图元数组得到新图元数组
bool unionElement(std::vector<OGRPolygon*>& polygonVector)
{
	//如果数目<=1，则返回
	int theSize = polygonVector.size();
	if (theSize <= 1)
	{
		return false;
	}
	//如果大于1，则设置两个图元数组
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
	//判断相并标志位是否为真
	std::vector<bool> bUnionVec;
	bUnionVec.clear();
	bool bInterSect = false;
	//如果发生了相交动作，则可以继续循环下去
	do
	{
		//循环前，先赋值是否相交为false
		bInterSect = false;
		polygonVecTemp1.clear();
		polygonVecTemp1 = polygonVecTemp2;
		polygonVecTemp2.clear();
		size1 = polygonVecTemp1.size();
		bUnionVec.clear();
		//标志位先置为false,即没有相并
		for (int i = 0; i < size1; i++)
		{
			bUnionVec.push_back(false);
		}
		//第一个跟其他进行相交则相并计算
		for (int i = 0; i < size1 - 1; i++)
		{
			//如果已经相并，则返回
			if (bUnionVec[i])
			{
				continue;
			}
			for (int j = i+1; j < size1; j++)
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
				//如果两者相交，则相交为true，并将该值放入polygonVecTemp2
				if (polygon1->Intersect(polygon2))
				{
					OGRGeometry * polygonUnion = polygon1->Union(polygon2);
					if (polygonUnion)
					{
						bInterSect = true;
						polygonVecTemp2.push_back(polygonUnion);
						//标志位置为true
						bUnionVec[i] = true;
						bUnionVec[j] = true;
					}
				}
			}
		}
		//把没有相交的图元也加上
		for (int i = 0; i < size1; i++)
		{
			//如果该位相并，则不再加上
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
bool unionGeometryVector(std::vector<OGRGeometry*>& geometryVector)
{
	//如果数目<=1，则返回
	int theSize = geometryVector.size();
	if (theSize <= 1)
	{
		return false;
	}
	//如果大于1，则设置两个图元数组
	std::vector<OGRGeometry*> polygonVecTemp1;
	polygonVecTemp1.clear();
	std::vector<OGRGeometry*> polygonVecTemp2;
	polygonVecTemp2.clear();
	polygonVecTemp2 = geometryVector;
	int size1 = polygonVecTemp1.size();
	int size2 = polygonVecTemp2.size();
	//判断相并标志位是否为真
	std::vector<bool> bUnionVec;
	bUnionVec.clear();
	bool bInterSect = false;
	//如果发生了相交动作，则可以继续循环下去
	do
	{
		//循环前，先赋值是否相交为false
		bInterSect = false;
		polygonVecTemp1.clear();
		polygonVecTemp1 = polygonVecTemp2;
		polygonVecTemp2.clear();
		size1 = polygonVecTemp1.size();
		bUnionVec.clear();
		//标志位先置为false,即没有相并
		for (int i = 0; i < size1; i++)
		{
			bUnionVec.push_back(false);
		}
		//第一个跟其他进行相交则相并计算
		for (int i = 0; i < size1 - 1; i++)
		{
			//如果已经相并，则返回
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
				//如果两者相交，则相交为true，并将该值放入polygonVecTemp2
				if (polygon1->Intersect(polygon2))
				{
					OGRGeometry * polygonUnion = polygon1->Union(polygon2);
					if (polygonUnion)
					{
						bInterSect = true;
						polygonVecTemp2.push_back(polygonUnion);
						//标志位置为true
						bUnionVec[i] = true;
						bUnionVec[j] = true;
					}
				}
			}
		}
		//把没有相交的图元也加上
		for (int i = 0; i < size1; i++)
		{
			//如果该位相并，则不再加上
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
int main()
{

	std::vector<OGRPolygon* > polygonVector;
	polygonVector.clear();
	std::vector<OGRMultiPolygon*> multiPolygonVector;
	multiPolygonVector.clear();

	std::vector<OGRGeometry*> geometryVector;
	geometryVector.clear();
	//读取cut1_output.shp到cut11_output.shp
	for (int i = 1; i < 12; i++)
	{
		std::string strID = std::to_string(i);
		std::string strShpName = "E:\\software\\TestData_71345\\TestData\\result\\ShpResult\\cut" + strID + "_OutShp.shp";
		//getPolygonAndMulitiPolygonVectorFromShpFile(strShpName.c_str(),polygonVector,multiPolygonVector);
		getGeometryVectorFromShpFile(strShpName.c_str(), geometryVector);
	}
	
	//计算是否相交,相交则相并
	//std::vector<OGRPolygon*> resultPolygonVector =  unionElements(polygonVector);
	//unionElement(polygonVector);
	unionGeometryVector(geometryVector);
	//创建一个shp文件，添加上这些
	std::string strResultShpFileName = "E:\\resultVector_union4.shp";
	//createShpVector(resultPolygonVector, strResultShpFileName);
	//createShpVector(polygonVector, strResultShpFileName);
	createShpVectorByGeometryVector(geometryVector, strResultShpFileName);

	//const char * pszVectorFileName = "E:\\poyangcut\\poyangcut\\shp\\poyanghu.shp";
	//const char * pszVectorFileName = "E:\\shpExcel\\PonitShp\\AllWater.shp";

	//std::map<int, std::vector<std::string>> mapFeatureSet;
	//mapFeatureSet.clear();
	//std::vector<std::string> strDefnVec;
	//strDefnVec.clear();
	//readVectorFile(pszVectorFileName, mapFeatureSet, strDefnVec);
	/*
	std::cout << "输出excel头" << std::endl;
	for (int i = 0; i < strDefnVec.size(); i++)
	{
		std::string strDefn = strDefnVec[i];
		std::cout << strDefn << std::endl;
	}
	//输出集合
	std::cout << "输出excel内容" << std::endl;
	std::map<int, std::vector<std::string>>::iterator
		iterCurSet = mapFeatureSet.begin(),
		iterEndSet = mapFeatureSet.end();
	for (; iterCurSet != iterEndSet; iterCurSet++)
	{
		int id = iterCurSet->first;
		std::cout << "第" << id << "个元素" << std::endl;
		std::vector<std::string> featureVec = iterCurSet->second;
		for (int i = 0; i < featureVec.size(); i++)
		{
			std::string theFeatureName = featureVec[i];
			std::cout << theFeatureName << std::endl;
		}
	}
	*/
	//输出到.csv
	//std::string strCSVFileName = "E:\\tifAll.csv";
	//writeCSV(strCSVFileName, mapFeatureSet, strDefnVec);

	//清空资源
	//mapFeatureSet.clear();
	//strDefnVec.clear();
	return 0;
}