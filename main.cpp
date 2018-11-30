
#include "myShp.h"

int main()
{
	myShp theShp;
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
		theShp.getGeometryVectorFromShpFile(strShpName.c_str(), geometryVector);
	}
	
	//计算是否相交,相交则相并
	//std::vector<OGRPolygon*> resultPolygonVector =  unionElements(polygonVector);
	//unionElement(polygonVector);
	theShp.unionGeometryVector(geometryVector);
	//创建一个shp文件，添加上这些
	std::string strResultShpFileName = "E:\\resultVector_union4.shp";
	//createShpVector(resultPolygonVector, strResultShpFileName);
	//createShpVector(polygonVector, strResultShpFileName);
	theShp.createShpVectorByGeometryVector(geometryVector, strResultShpFileName);

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