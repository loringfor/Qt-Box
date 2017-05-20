#include "onebox.h"

OneBox::OneBox(const QColor &color):brushColor(color)
{
}

OneBox::~OneBox(){}

QRectF OneBox::boundingRect() const
{
    float penWidth=1;
    return QRectF(-10-penWidth/2,-10-penWidth/2,20+penWidth,20+penWidth);
}

void OneBox::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    painter->drawPixmap(-10,-10,20,20,QPixmap(":/image/box.gif"));//单个方块的图
    painter->setBrush(brushColor);//设置笔刷

    QColor penColor=brushColor;
    penColor.setAlpha(20);//将颜色透明度降低
    painter->setPen(penColor);//用刚刚设置好的笔
    painter->drawRect(-10,-10,20,20);//画矩形


}
QPainterPath OneBox::shape() const
{
    QPainterPath path;
    path.addRect(-9.5,-9.5,19,19);
    return path;

}
