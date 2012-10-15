#ifndef QSCENEDISPLAY_H
#define QSCENEDISPLAY_H

#include <QGLWidget>
#include "gl/GL.h"
#include "gl/GLU.h"

namespace Ui {
	class QSceneDisplay;
}

class QSceneDisplay : public QGLWidget
{
	Q_OBJECT

public:
	QSceneDisplay(QWidget *parent = 0);
	~QSceneDisplay();

signals:
	void DrawScene();


protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width,int height);

private:
};

#endif // QSCENEDISPLAY_H
