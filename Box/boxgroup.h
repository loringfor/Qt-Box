#ifndef BOXGROUP_H
#define BOXGROUP_H
#include <QGraphicsItemGroup>
#include <QKeyEvent>
#include <QTransform>
#include <QTimer>
#include "onebox.h"


class BoxGroup :public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    enum BoxShape{IShape,JShape,LShape,OShape,SShape,TShape,ZShape,RandomShape}; //枚举7种方块的形状
    BoxGroup();
    ~BoxGroup();
    QRectF boundingRect() const;
    bool isColliding();
    void createBox(const QPointF &point=QPointF(0,0),BoxShape shape=RandomShape);//创建
    void clearBoxGroup(bool destroyBox=false);//清除方块组
    BoxShape getCurrentShape(){return currentShape;}//获取当前方块组的形状

signals:
    void needNewBox();
    void gameFinished();

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void moveOneStep();
    void startTimer(int interval);
    void stopTimer();

private:
    BoxShape currentShape;
    QTransform oldTransform;
    QTimer *timer;
};

#endif // BOXGROUP_H
