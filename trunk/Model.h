#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "Scene.h"
#include "BaseStruct.h"
using namespace std;
/*
��Ӧһ��ģ�ͣ�Ӧ�ú������Լ��İ�Χ�С���Χ�������
*/
class Scene;

class Model
{
public:
	Model(void);
	~Model(void);

	// �Զ�������
public:
	int faceStart;  // ��Ӧ��Object��Face���俪ʼ��ֵ����������ΪfaceStart<= x < faceEnd����ֵΪ��Ӧ��indexֵ��
	int faceEnd;  // ��Ӧ��Object��Face����Ľ�����ֵ
	string name; // ��Ӧ��������ƣ���ʾΪTag0Num
	string tag; // ��Ӧ���������磺Chair��Desk��etc

	// �Զ��巽��
public:
	void DrawModel(Scene* scene);
};

#endif