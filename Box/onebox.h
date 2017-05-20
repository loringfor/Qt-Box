#ifndef ONEBOX_H
#define ONEBOX_H

#include <QGraphicsObject>
#include <QPainter>

class OneBox:public QGraphicsObject
{
    Q_OBJECT
public:
    OneBox(const QColor &color=Qt::red);
    ~OneBox();
    QRectF boundingRect() const;//虚函数重写，定义项的外边界为矩形
    void paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget); //画图
    QPainterPath shape() const;

private:
    QColor brushColor; //笔刷颜色
};

#endif // ONEBOX_H
