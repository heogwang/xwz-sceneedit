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

	int state; // ָʾ��ǰ��״̬����ƽ�ƻ���ѡ������

	int selectModel;  // ѡ�е�ģ�͵�index

	Scene *scene;

	// �Զ��巽��
public:
	void DrawCoodinates();
	void DrawScene();


public slots:
	void SetDisScene(Scene* scene);  // ������ĳ������������������в���
	void ChooseModelAction();  // ��Ӧѡ��ģ��Action

signals:

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width,int height);
	void ProcessSelection(int xPos,int yPos);
	void ProcessModels(GLuint *pSelectBuff);

	// �¼�����
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
	


private:
};

#endif // QSCENEDISPLAY_H
