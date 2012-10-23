#ifndef QSCENEDISPLAY_H
#define QSCENEDISPLAY_H

#include <QGLWidget>
#include <QPoint>
#include <QMouseEvent>
#include "gl/GL.h"
#include "gl/GLU.h"
#include "trimesh/Vec.h"

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
	double xangle;
	double yangle;
	double scale;
    QPoint lbtnDown; // �������ʱ����Ļ���λ��
	//float zNear;
	//float zFar;
	GLfloat *eye;







	// �Զ��巽��
public:
	void DrawCoodinates();


public slots:
	void SetDisProperty(point center, float r);


signals:
	void DrawScene();
	//void SetCamera(GLfloat* eye,double scale,double aspect);
	void SetCamera(GLfloat* eye,double scale,double aspect);

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
