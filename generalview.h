#ifndef GENERALVIEW
#define GENERALVIEW

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsTextItem>

class generalView : public QGraphicsView{
    Q_OBJECT
public:
    generalView();
    QGraphicsScene *genViewScene;
    QTimer *testTimer;

    QGraphicsTextItem *populationText;
private:
private slots:
    void test();

};

#endif // GENERALVIEW

