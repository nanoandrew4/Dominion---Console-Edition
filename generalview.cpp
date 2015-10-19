#include "mainmenu.h"
#include "generalview.h"
#include "time.h"

#include <QDebug>

extern int screenWidth;
extern int screenHeight;
extern QGraphicsView *view;

int population = 10;
int capital = 1000;
int wood = 0;
int stone = 0;
int iron = 0;

generalView::generalView()
{
    genViewScene = new QGraphicsScene(view);
    genViewScene->setSceneRect(0,0,screenWidth, screenHeight);

    view->setScene(genViewScene);

    testTimer = new QTimer();
    connect(testTimer, SIGNAL(timeout()), this, SLOT(test()));
    testTimer->start(1000);

    populationText = new QGraphicsTextItem();
    populationText->setPlainText(QString("Population: ") + QString::number(population));
    populationText->setPos(50,50);
    genViewScene->addItem(populationText);

   //show();
}

void generalView::test()
{

}
