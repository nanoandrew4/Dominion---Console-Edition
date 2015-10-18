#ifndef GENERALVIEW
#define GENERALVIEW

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

class generalView : public QGraphicsView{
    Q_OBJECT
public:
    generalView();
    QGraphicsScene *scene;
    QTimer *testTimer;
private:
private slots:
    void test();

};

#endif // GENERALVIEW

