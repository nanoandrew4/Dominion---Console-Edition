#include "mainmenu.h"
#include "generalview.h"

extern int screenWidth;
extern int screenHeight;
QGraphicsScene *scene;

mainMenu::mainMenu()
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0, screenWidth, screenHeight);

    view = new QGraphicsView();
    view->setFixedSize(screenWidth, screenHeight);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setScene(scene);

    playButton = new QPushButton("Play", view);
    playButton->setGeometry(screenWidth /2 - screenWidth /30, screenHeight /3, screenWidth /15, screenHeight /15);
    connect(playButton, SIGNAL(released()), this, SLOT(playGame()));

    exitButton = new QPushButton("Exit", view);
    exitButton->setGeometry(screenWidth /2 - screenWidth /30, screenHeight *1.5/3, screenWidth /15, screenHeight /15);
    connect(exitButton, SIGNAL(released()), this, SLOT(exitGame()));

}

void mainMenu::playGame()
{
    delete playButton;
    delete exitButton;

    generalView *genView = new generalView();
    view->hide();
    //genView->setScene(scene);
    //genView->show();

}

void mainMenu::exitGame()
{
    delete playButton;
    delete exitButton;
    delete scene;
    exit(0);
}
