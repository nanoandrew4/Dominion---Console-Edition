#ifndef VILLAGEVIEW
#define VILLAGEVIEW

#include <QGraphicsScene>
#include <QObject>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QPushButton>

class villageView : public QObject{
    Q_OBJECT
public:
    villageView();
    QGraphicsScene *genViewScene;
    QGraphicsScene *buildingsScene;

    QTimer *upgradeBuildingTimer;
    QTimer *updateTextTimer;

    QPushButton *villageBuildSlot1;
    QPushButton *villageBuildSlot2;

    QGraphicsPixmapItem *villageBackground;

    QGraphicsTextItem *populationText;

    QGraphicsTextItem *welcomeBuildingSceneText;
    QGraphicsTextItem *warehouseText;
    QGraphicsTextItem *granary;
private:
private slots:
    void upgradeBuilding();
    void updateText();

};

#endif // VILLAGEVIEW

