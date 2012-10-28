#ifndef QSCENEDISPLAY_H
#define QSCENEDISPLAY_H

#include <QGLWidget>
#include <QPoint>
#include <QMouseEvent>
#include "gl/GL.h"
#include "gl/GLU.h"
#include "gl/glut.h"
#include "trimesh/Vec.h"
#include "Scene.h"
#include "arcball.h"

namespace Ui {
	class QSceneDisplay;
}

class QSceneDisplay : public QGLWidget
{
	Q_OBJECT

public:
	QSceneDisplay(QWidget *parent = 0);
	~QSceneDisplay();

	// 自定义属性
public:
	double xangle;
	double yangle;
	double scale;
    QPoint lbtnDown; // 左键按下时，屏幕点的位置
	QPoint rbtnDown; // 鼠标右键按下
	float zNear;
	float zFar;
	vec eyeold;
	vec eye; // 视线的方向
	vec up; // 向上的方向
	float radius;  // 视线所在球的半径
	//GLfloat eye[4];
	GLfloat plane[4];
	//double dx; // 鼠标移动产生的dx
	//double dy; // 鼠标移动产生的dy
	//arcvec eye;
	//arcvec center;
	//arcvec up;

	Scene *scene;






	// 自定义方法
public:
	void DrawCoodinates();


public slots:
	void SetDisProperty(point center, float r);
	void SetCamera();
	void SetDisScene(Scene* scene);

signals:
	void DrawScene();
	//void SetCamera(GLfloat* eye,double scale,double aspect);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width,int height);

	// 事件处理
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);



private:
};

#endif // QSCENEDISPLAY_H
