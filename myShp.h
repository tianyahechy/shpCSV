#pragma once
#include "ogrsf_frmts.h"
#include "ogr_api.h"
#include "gdal_priv.h"
#include <iostream>
#include <map>
#include <fstream>

class myShp
{
public:
	myShp();
	~myShp();

public:

	//读取矢量 ,
	void readVectorFile(const char * pszVectorFileName,
		std::map<int, std::vector<std::string>>& mapFeatureSet,
		std::vector<std::string>& strDefnVec
		);
	//读取矢量 ,
	void readVectorFileWithID(
		const char * pszVectorFileName,
		int &mapSetID,  //图元集合的ID
		bool & bHaveReadDefn,//是否读取属性定义，如果没有，则读取；如果读过了，则不再读取
		std::map<int, std::vector<std::string>>& mapFeatureSet,
		std::vector<std::string>& strDefnVec
		);
	//读取矢量文件中的多边形 ,
	void getPolygonVectorFromShpFile(
		const char * pszVectorFileName,
		std::vector<OGRPolygon*>& polygonVector
		);

	//读取矢量文件中的图元 ,
	void getGeometryVectorFromShpFile(
		const char * pszVectorFileName,
		std::vector<OGRGeometry*>& geometryVector
		);
	//读取矢量文件中的多边形 ,
	void getPolygonAndMulitiPolygonVectorFromShpFile(
		const char * pszVectorFileName,
		std::vector<OGRPolygon*>& polygonVector,
		std::vector<OGRMultiPolygon*> &multiPolygonVector
		);
	//写文件
	void writeCSV(
		std::string strCSVName,
		std::map<int, std::vector<std::string>> mapFeatureSet,
		std::vector<std::string> strDefnVec
		);
	void createShpVector(
		std::vector<OGRPolygon*> polygonVector, 
		std::string strShpFileName);
	//创建shp文件
	void createShpVectorByGeometryVector(
		std::vector<OGRGeometry*> geometryVector,
		std::string strShpFileName);
	// 合并图元数组得到新图元数组
	bool unionElement(std::vector<OGRPolygon*>& polygonVector);
	bool unionGeometryVector(std::vector<OGRGeometry*>& geometryVector);
};