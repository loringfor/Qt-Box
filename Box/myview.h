#ifndef MYVIEW_H
#define MYVIEW_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QLabel>
#include <QFont>
#include <QIcon>
#include <QPen>
#include "onebox.h"
#include "boxgroup.h"

// 游戏的初始速度
static const float INITSPEED = 500;

class MyView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MyView(QWidget *parent=0);
    ~MyView();

public slots:
    void startGame();
    void clearFullRows();
    void moveBox();
    void gameOver();

    void restartGame();
    void finishGame();
    void pauseGame();
    void returnGame();

    void aboutToFinish();

private:
    float gameSpeed;
    BoxGroup *boxGroup;
    BoxGroup *nextBoxGroup;
    void initView();
    void initGame();
    QGraphicsLineItem *topLine;
    QGraphicsLineItem *bottomLine;
    QGraphicsLineItem *leftLine;
    QGraphicsLineItem *rightLine;
    void updateScore(const int fullRowNum=0);
    QList<int> rows;

    QGraphicsTextItem *gameScoreTextTip; //分数提示文本
    QGraphicsTextItem *gameScoreText; //分数文本
    QGraphicsTextItem *gameLevelText; //等级文本
    // 遮罩面板
    QGraphicsProxyWidget *maskWidget;
    // 各种按钮
    QGraphicsWidget *startButton;
    QGraphicsWidget *finishButton;
    QGraphicsWidget *restartButton;
    QGraphicsWidget *pauseButton;
    QGraphicsWidget *returnButton;
    QGraphicsWidget *helpButton;
    QGraphicsWidget *exitButton;
    QGraphicsWidget *showMenuButton;
    // 各种文本
    QGraphicsTextItem *gameWelcomeText;
    QGraphicsTextItem *gamePausedText;
    QGraphicsTextItem *gameOverText;

    QMediaPlaylist *playlist0;
    QMediaPlayer *player0;
    QMediaPlaylist *playlist1;
    QMediaPlayer *player1;


protected:
    void keyPressEvent(QKeyEvent *event);

};

#endif // MYVIEW_H
