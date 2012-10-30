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

	// �Զ�������
public:
	float xangle; //�ۼƽǶ�ֵ
	float yangle; // �ۼƽǶ�ֵ
	float scale;
    QPoint btnDown; // �������ʱ����Ļ���λ��
	vec eye; // ���ߵķ���
	vec up; // ���ϵķ���
	float radius;  // ����������İ뾶
	GLfloat plane[4];

	Scene *scene;






	// �Զ��巽��
public:
	void DrawCoodinates();
	void DrawScene();


public slots:
	void SetCamera();
	void SetDisScene(Scene* scene);  // ������ĳ������������������в���


signals:

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width,int height);

	// �¼�����
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);



private:
};

#endif // QSCENEDISPLAY_H
