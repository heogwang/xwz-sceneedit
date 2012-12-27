#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"

#define DATAGENERATE

#ifdef DATAGENERATE
#include "PointDataManager.h"
#endif

int main(int argc, char *argv[])
{
#ifdef DATAGENERATE
	//POINTACTION actions[] = {CALCPOINTS,SAVEPOINTS};
	POINTACTION actions[] = {CALCPOINTS,SAVEPOINTS,SAVED2,SAVESECTOR,SAVESHELL,SAVESECSHELL};
	//POINTACTION actions[] = {VALID2};
	PointDataManager *m = new PointDataManager("D:\\geometry\\QSceneEdit\\conferenceroom\\",actions,sizeof(actions)/sizeof(POINTACTION));
	delete m;
	return 0;
#endif

#ifdef BNSTRAINING
	LoadScenes();
#endif
	QApplication a(argc, argv);
	// ∑¿÷π÷–Œƒ¬“¬Î
	QTextCodec *codec = QTextCodec::codecForName("GB2312");   

	QTextCodec::setCodecForLocale(codec);   
	QTextCodec::setCodecForCStrings(codec);   
	QTextCodec::setCodecForTr(codec);
    MainWindow w;
    //w.show();
	w.showMaximized(); 
    return a.exec();
}
