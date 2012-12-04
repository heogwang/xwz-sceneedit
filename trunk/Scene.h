#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <map>
#include <QObject>
#include <limits>
#include "BaseStruct.h"
#include "trimesh/Vec.h"
#include "Material.h"
#include "Model.h"
#include "trimesh/Box.h"
#include "trimesh/bsphere.h"

using namespace std;

// ������Material��ע��
#ifndef DefMaterial
#define  DefMaterial
#endif
/*
	��Ӧһ���������������Ժ��Ƕ��ģ��
*/

class Model;

class Scene : public QObject
{
	 Q_OBJECT

public:
    Scene(QObject* parent=0);
	~Scene(void);

	// ��Ҫ����
public:
	string scenePath; // obj���ڵ�·����E://abc//a.obj
	string dirPath; //  ��·�����磺E://abc//��

	point *points;
	int Vsize; // ��ĸ���

	vnormal *vnormals;
	int Vnsize; // �������

	vector<Face*> faces;

#ifdef DefMaterial
	string mtlPath;

	vtexture* vtextures;
	int Vtsize; // ��������

	Material *materials;
	int MtlSize;

	int TextureNum; // �����εĸ�������Ӧusemtl����һ��

	vector<int> usemtlSlice; // ÿ�γ���usemtlʱ��faces�ĸ�����
	vector<int> mtlMark; // ��usemtlSlice��Ӧ��usemtlSlice[i]~~usemtlSlice[i+1]��Ӧ��mtlΪ��mtlMark[i];
#endif

	int modelSize; // ��ģ�͸���������ǽ��
	//Byte* mvisible;  // ��ģ���Ƿ�ɼ�
	vector<Model*> sceneModels; //�����е�ģ��


	int** relationTable;  // ��ϵ����
	map<string,int> ModelMap; // Tag : Model_index
	map<string,vector<int>> RelationMap; // Tag : Relationship

	box bbox; //��Χ��
	BSphere bsphere; // ��Χ��

	// ��Ҫ��������
public:
    bool readScene(const char* filename);
	void DrawScene();
	void need_bbox();
	void need_bsphere();
	void DrawTest();
	void DrawSimpleScene(); //ֱ�ӻ���ģ�ͣ�����model
	void SaveScene(); // ���泡���ļ�
	// ��������
public:
	// return false,û���ҵ���Ӧ��ʽ��������
    bool readHelper(const char* filename);
	// ��ȡobj��ʽ��Scene
    bool read_obj(const char* filename);
	// �ҵ�·�����ļ��е�ַ
	void ExtractBasePath(const char* filename);

	// �ָ��ɶ�����㹹�ɵ���Ƭ
	void tess(const vector<int> &thisv,const vector<int> &thisvn);

#ifdef DefMaterial
	// ��ȡmtl�����ļ���Ϣ
	void LoadMtl(string mtlPath);
	// �ָ��ɶ�����㹹�ɵ���Ƭ,��������
	void tess(const vector<int> &thisv,const vector<int> &thisvt,const vector<int> &thisvn);
#endif
	// ����ģ�͵Ľ���Faces
	void CompleteModelSetting();
	// �ҵ�ģ�͵�Tag
	static string FindModelTag(string name);
	// ������ϵ��
	void BuildRelationTable();
};

#endif