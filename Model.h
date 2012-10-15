#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "Scene.h"
#include "BaseStruct.h"
using namespace std;
/*
对应一个模型，应该涵盖了自己的包围盒、包围球等属性
*/
class Scene;

class Model
{
public:
	Model(void);
	~Model(void);

	// 自定义属性
public:
	int faceStart;  // 对应该Object的Face区间开始的值，假设区间为faceStart<= x < faceEnd；该值为对应的index值。
	int faceEnd;  // 对应该Object的Face区间的结束的值
	string name; // 对应物体的名称，表示为Tag0Num
	string tag; // 对应物体的类别，如：Chair，Desk，etc

	// 自定义方法
public:
	void DrawModel(Scene* scene);
};

#endif