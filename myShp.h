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

	//��ȡʸ�� ,
	void readVectorFile(const char * pszVectorFileName,
		std::map<int, std::vector<std::string>>& mapFeatureSet,
		std::vector<std::string>& strDefnVec
		);
	//��ȡʸ�� ,
	void readVectorFileWithID(
		const char * pszVectorFileName,
		int &mapSetID,  //ͼԪ���ϵ�ID
		bool & bHaveReadDefn,//�Ƿ��ȡ���Զ��壬���û�У����ȡ����������ˣ����ٶ�ȡ
		std::map<int, std::vector<std::string>>& mapFeatureSet,
		std::vector<std::string>& strDefnVec
		);
	//��ȡʸ���ļ��еĶ���� ,
	void getPolygonVectorFromShpFile(
		const char * pszVectorFileName,
		std::vector<OGRPolygon*>& polygonVector
		);

	//��ȡʸ���ļ��е�ͼԪ ,
	void getGeometryVectorFromShpFile(
		const char * pszVectorFileName,
		std::vector<OGRGeometry*>& geometryVector
		);
	//��ȡʸ���ļ��еĶ���� ,
	void getPolygonAndMulitiPolygonVectorFromShpFile(
		const char * pszVectorFileName,
		std::vector<OGRPolygon*>& polygonVector,
		std::vector<OGRMultiPolygon*> &multiPolygonVector
		);
	//д�ļ�
	void writeCSV(
		std::string strCSVName,
		std::map<int, std::vector<std::string>> mapFeatureSet,
		std::vector<std::string> strDefnVec
		);
	void createShpVector(
		std::vector<OGRPolygon*> polygonVector, 
		std::string strShpFileName);
	//����shp�ļ�
	void createShpVectorByGeometryVector(
		std::vector<OGRGeometry*> geometryVector,
		std::string strShpFileName);
	// �ϲ�ͼԪ����õ���ͼԪ����
	bool unionElement(std::vector<OGRPolygon*>& polygonVector);
	bool unionGeometryVector(std::vector<OGRGeometry*>& geometryVector);
};