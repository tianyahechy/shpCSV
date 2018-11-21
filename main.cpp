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
int main()
{

	std::vector<OGRPolygon* > polygonVector;
	polygonVector.clear();
	//读取cut1_output.shp到cut11_output.shp
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