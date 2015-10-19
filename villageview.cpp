#include "mainmenu.h"
#include "villageview.h"
#include "time.h"

#include <QDebug>

extern int screenWidth;
extern int screenHeight;
extern QGraphicsView *view;

double scaleX = 0.0;
double scaleY = 0.0;

int population = 10;
int capital = 1000;
int wood = 0;
int stone = 0;
int iron = 0;
int buildings[5] = {0,0,0,0,0};
bool upgradeBuildingsSceneGen = true;


villageView::villageView()
{
    genViewScene = new QGraphicsScene(view);
    genViewScene->setSceneRect(0,0,screenWidth, screenHeight);

    view->setScene(genViewScene);
    //villageBackground = new QGraphicsPixmapItem();
    //villageBackground->setPixmap(QPixmap(":/new/Graphics/Graphics/VillageBackground.png").scaled(screenWidth, screenHeight));
    //scaleX = 1680.0 / villageBackground->pixmap().width(); // get scale to reposition all items on screen according to ratio between original png and rescaled one
    //scaleY = 1050.0 / villageBackground->pixmap().height(); // read above
    //genViewScene->addItem(villageBackground);

    villageBuildSlot1 = new QPushButton("Upgrade1", view);
    villageBuildSlot1->setGeometry(screenWidth /15, screenHeight /5, screenWidth /20, screenHeight /20);
    villageBuildSlot1->show();

    connect(villageBuildSlot1, SIGNAL(released()), this, SLOT(upgradeBuilding()));

    updateTextTimer = new QTimer();
    connect(updateTextTimer, SIGNAL(timeout()), this, SLOT(updateText()));
    updateTextTimer->start(1000);

    populationText = new QGraphicsTextItem();
    populationText->setPos(screenWidth /15,screenHeight /10);
    populationText->setFont(QFont("times", 12));

    genViewScene->addItem(populationText);

    //view->show();

}

void villageView::upgradeBuilding()
{
//remember to hide villageBackground
    villageBuildSlot1->hide();
    if(upgradeBuildingsSceneGen == true){
        buildingsScene = new QGraphicsScene();
        buildingsScene->setSceneRect(0,0,screenWidth, screenHeight);
        welcomeBuildingSceneText = new QGraphicsTextItem();
        warehouseText = new QGraphicsTextItem();
        granary = new QGraphicsTextItem();
        upgradeBuildingsSceneGen = false;
    }
    view->setScene(buildingsScene);
    welcomeBuildingSceneText->setPos(screenWidth /100, screenHeight /100);
    welcomeBuildingSceneText->setPlainText(QString("Welcome to the building selector. To build one of these buildings, select the build button."));
    buildingsScene->addItem(welcomeBuildingSceneText);

}

void villageView::updateText()
{
    populationText->setPlainText(QString("Population: ") + QString::number(population));
}
