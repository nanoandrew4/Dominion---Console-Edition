#include "mainmenu.h"
#include "generalview.h"
#include "time.h"

#include <QDebug>

extern int screenWidth;
extern int screenHeight;
int population = 10;
int capital = 1000;
int wood = 0;
int stone = 0;
int iron = 0;

generalView::generalView()
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,screenWidth, screenHeight);

    setFixedSize(screenWidth, screenHeight);
    setScene(scene);


    testTimer = new QTimer();
    connect(testTimer, SIGNAL(timeout()), this, SLOT(test()));
    testTimer->start(1000);

    show();
}

void generalView::test()
{

}
