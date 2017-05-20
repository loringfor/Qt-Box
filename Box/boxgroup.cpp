#include "boxgroup.h"
#include<QTransform>

BoxGroup::BoxGroup()
{
    setFlags(QGraphicsItem::ItemIsFocusable);//标志中的所有标志都被使能
    oldTransform=transform();//保存变换矩阵，当BoxGroup进行旋转后，可以使用它来恢复
    timer=new QTimer(this);//设置定时器，用来进行方块组自动下移
    connect(timer,SIGNAL(timeout()),this,SLOT(moveOneStep()));
    currentShape=RandomShape;
}

BoxGroup::~BoxGroup(){}

QRectF BoxGroup::boundingRect() const
{
    float penWidth=1;
    return QRectF(-40-penWidth/2,-40-penWidth/2,80+penWidth,80+penWidth);
}

void BoxGroup::keyPressEvent(QKeyEvent *event)
{
    static int k=1;
    switch (event->key()) {
    case Qt::Key_Down:
        moveBy(0,20);//下移一格
        if(isColliding()){
            moveBy(0,-20);//上移一格
            clearBoxGroup();//将小方块从方块组中移除到场景中
            emit needNewBox();//需要显示新的方块
        }
        break;
    case Qt::Key_Left:
        moveBy(-20,0);//左移一格
        if(isColliding())
            moveBy(20,0);//右移一格
        break;
    case Qt::Key_Right:
        moveBy(20,0);//右移一格
        if(isColliding())
            moveBy(-20,0);//左移一格
        break;
    case Qt::Key_Up:
        setRotation(90*k);//按一次旋转90°
        if(isColliding())
           setRotation(-90*k);
        k++;
        break;
    case Qt::Key_Space:
        moveBy(0, 20);
        while (!isColliding()) {
            moveBy(0, 20);
        }
        moveBy(0, -20);
        clearBoxGroup();
        emit needNewBox();
        break;
    }
}

bool BoxGroup::isColliding()
{
    QList<QGraphicsItem*>itemList=childItems();
    QGraphicsItem *item;
    foreach (item, itemList) {
        if(item->collidingItems().count()>1)
            return true;
    }
    return false;
}

// 删除方块组中的所有小方块
void BoxGroup::clearBoxGroup(bool destroyBox)
{
    QList<QGraphicsItem *> itemList = childItems();
    QGraphicsItem *item;
    foreach (item, itemList) {
        removeFromGroup(item);
        if (destroyBox) {
            OneBox *box = (OneBox*) item;
            box->deleteLater();
        }
    }
}

// 创建方块
void BoxGroup::createBox(const QPointF &point, BoxShape shape)
{
    static const QColor colorTable[7] = {
                QColor(200, 0, 0, 100), QColor(255, 200, 0, 100),
                QColor(0, 0, 200, 100), QColor(0, 200, 0, 100),
                QColor(0, 200, 255, 100), QColor(200, 0, 255, 100),
                QColor(150, 100, 100, 100)
    };

    int shapeID = shape;

    if (shape == RandomShape) {
        // 产生0-6之间的随机数
        shapeID = qrand() % 7;
    }

    QColor color = colorTable[shapeID];

    QList<OneBox* > list;
    //恢复方块组的变换矩阵
    setTransform(oldTransform);

    for (int i = 0; i < 4; ++i) {
        OneBox *temp = new OneBox(color);
        list << temp;
        addToGroup(temp);
    }

    switch (shapeID) {
    case IShape :
        currentShape = IShape;
        list.at(0)->setPos(-30, -10);
        list.at(1)->setPos(-10, -10);
        list.at(2)->setPos(10, -10);
        list.at(3)->setPos(30, -10);
        break;

    case JShape :
        currentShape = JShape;
        list.at(0)->setPos(10, -10);
        list.at(1)->setPos(10, 10);
        list.at(2)->setPos(-10, 30);
        list.at(3)->setPos(10, 30);
        break;

    case LShape :
        currentShape = LShape;
        list.at(0)->setPos(-10, -10);
        list.at(1)->setPos(-10, 10);
        list.at(2)->setPos(-10, 30);
        list.at(3)->setPos(10, 30);
        break;

    case OShape :
        currentShape = OShape;
        list.at(0)->setPos(-10, -10);
        list.at(1)->setPos(10, -10);
        list.at(2)->setPos(-10, 10);
        list.at(3)->setPos(10, 10);
        break;

    case SShape :
        currentShape = SShape;
        list.at(0)->setPos(10, -10);
        list.at(1)->setPos(30, -10);
        list.at(2)->setPos(-10, 10);
        list.at(3)->setPos(10, 10);
        break;

    case TShape :
        currentShape = TShape;
        list.at(0)->setPos(-10, -10);
        list.at(1)->setPos(10, -10);
        list.at(2)->setPos(30, -10);
        list.at(3)->setPos(10, 10);
        break;

    case ZShape :
        currentShape = ZShape;
        list.at(0)->setPos(-10, -10);
        list.at(1)->setPos(10, -10);
        list.at(2)->setPos(10, 10);
        list.at(3)->setPos(30, 10);
        break;

    default : break;
    }
    // 设置位置
    setPos(point);
    // 如果开始就发生碰撞，说明已经结束游戏
    if (isColliding()) {
        stopTimer();
        emit gameFinished();
    }
}

// 开启定时器
void BoxGroup::startTimer(int interval)
{
    timer->start(interval);
}

// 向下移动一步
void BoxGroup::moveOneStep()
{
    moveBy(0, 20);
    if (isColliding()) {
        moveBy(0, -20);
        // 将小方块从方块组中移除到场景中
        clearBoxGroup();
        emit needNewBox();
    }
}

// 停止定时器
void BoxGroup::stopTimer()
{
    timer->stop();
}

