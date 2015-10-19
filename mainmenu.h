#ifndef MAINMENU
#define MAINMENU

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QObject>

class mainMenu : public QObject{
    Q_OBJECT
public:
    mainMenu();

    //QGraphicsScene *scene;
    //QGraphicsView *view;
    QPushButton *playButton;
    QPushButton *exitButton;
private:
private slots:
    void playGame();
    void exitGame();

};

#endif // MAINMENU

