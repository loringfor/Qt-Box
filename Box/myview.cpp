#include "myview.h"
#include <QMultimedia>

#include <QUrl>

MyView::MyView(QWidget *parent):
    QGraphicsView(parent)
{
    initView();
}

MyView::~MyView()
{

}

void MyView::initView(){
    setRenderHint(QPainter::Antialiasing);//使用抗锯齿渲染
    setCacheMode(CacheBackground);//设置缓存背景，这样可以加快渲染速度
    setWindowTitle(QStringLiteral("方块游戏"));//设置窗口标题
    setWindowIcon(QIcon(":/image/icon.png"));//设置图标
    setMinimumSize(810,510);//设置窗口最小尺寸
    setMaximumSize(810,510);//设置窗口最大尺寸，相当于把窗口尺寸定死了

    QGraphicsScene *scene=new QGraphicsScene;//新建一个场景对象
    scene->setSceneRect(5,5,800,500);
    scene->setBackgroundBrush(QPixmap(":/image/background.png"));//设置场景背景
    setScene(scene);//设置场景，相当于显示刚才设置的

    //分别加入4条边界线
    topLine=scene->addLine(197,47,403,47);
    bottomLine=scene->addLine(197,453,403,453);
    leftLine=scene->addLine(197,47,197,453);
    rightLine=scene->addLine(403,47,403,453);

    // 当前方块组和提示方块组
    boxGroup = new BoxGroup;
    connect(boxGroup, SIGNAL(needNewBox()), this, SLOT(clearFullRows()));
    connect(boxGroup, SIGNAL(gameFinished()), this, SLOT(gameOver()));
    scene->addItem(boxGroup);
    nextBoxGroup = new BoxGroup;
    scene->addItem(nextBoxGroup);

    // 得分文本
    gameScoreText = new QGraphicsTextItem("");
    scene->addItem(gameScoreText);
    gameScoreText->setFont(QFont("Times", 20, QFont::Bold));
    gameScoreText->setPos(650, 350);

    // 得分提示文本
    gameScoreTextTip = new QGraphicsTextItem(QStringLiteral("您的得分为:"));
    scene->addItem(gameScoreTextTip);
    gameScoreTextTip->setFont(QFont("Times", 20, QFont::Bold));
    gameScoreTextTip->setPos(450, 350);

    // 级别文本
    gameLevelText = new QGraphicsTextItem("");
    scene->addItem(gameLevelText);
    gameLevelText->setFont(QFont("Times", 30, QFont::Bold));
    gameLevelText->setPos(20, 150);
 
    // 设置初始为隐藏状态
    topLine->hide();
    bottomLine->hide();
    leftLine->hide();
    rightLine->hide();
    gameScoreText->hide();
    gameScoreTextTip->hide();
    gameLevelText->hide();

    // 黑色遮罩
    QWidget *mask = new QWidget;
    mask->setAutoFillBackground(true);
    mask->setPalette(QPalette(QColor(0, 0, 0, 80)));
    mask->resize(900, 600);
    maskWidget = scene->addWidget(mask,Qt::Widget);
    maskWidget->setPos(-50, -50);
    //设置其Z值为1，这样可以处于Z值为0的图形项上面
    maskWidget->setZValue(1);


    // 游戏欢迎文本
    gameWelcomeText = new QGraphicsTextItem("");
    scene->addItem(gameWelcomeText);
    gameWelcomeText->setHtml(QStringLiteral("<font color=red>俄罗斯方块游戏</font>"));
    gameWelcomeText->setFont(QFont("Times", 30, QFont::Bold));
    gameWelcomeText->setPos(250, 100);
    gameWelcomeText->setZValue(2);

    // 游戏暂停文本
    gamePausedText = new QGraphicsTextItem("");
    scene->addItem(gamePausedText);
    gamePausedText->setHtml(QStringLiteral("<font color=white>游戏暂停中！</font>"));
    gamePausedText->setFont(QFont("Times", 30, QFont::Bold));
    gamePausedText->setPos(300, 100);
    gamePausedText->setZValue(2);
    gamePausedText->hide();

    // 游戏结束文本
    gameOverText = new QGraphicsTextItem("");
    scene->addItem(gameOverText);
    gameOverText->setHtml(QStringLiteral("<font color=red>Game Over!</font>"));
    gameOverText->setFont(QFont("Times", 30, QFont::Bold));
    gameOverText->setPos(320, 100);
    gameOverText->setZValue(2);
    gameOverText->hide();

    QPalette palette;
    palette.setColor(QPalette::ButtonText, Qt::black);
    // 帮助面板
    QWidget *help = new QWidget;
    QPushButton *helpCloseButton = new QPushButton(QStringLiteral("关   闭"), help);
    helpCloseButton->setPalette(palette);
    helpCloseButton->move(120, 300);
    connect(helpCloseButton, SIGNAL(clicked()), help, SLOT(hide()));
    help->setAutoFillBackground(true);
    help->setPalette(QPalette(QColor(0, 0, 0, 150)));
    help->resize(350, 400);
    QGraphicsWidget *helpWidget = scene->addWidget(help);
    helpWidget->setPos(250, 50);
    helpWidget->setZValue(3);
    helpWidget->hide();

    QLabel *helpLabel = new QLabel(help);
    helpLabel->setText(QStringLiteral("<h3><font color=red>游戏操作</font><br>‘左键’‘右键’‘下键’分别<br>实现左右下移动，‘上键’实现<br>变换，'空格'实现快速下移。<br><br> <font color=yellow>向玉林作品<br>学习所用，盗版必究！</font></h3>"));
    helpLabel->setAlignment(Qt::AlignCenter);
    helpLabel->move(30, 80);


    // 游戏中使用的按钮

    QPushButton *button1 = new QPushButton(QStringLiteral("开    始"));
    QPushButton *button3 = new QPushButton(QStringLiteral("帮    助"));
    QPushButton *button4 = new QPushButton(QStringLiteral("退    出"));
    QPushButton *button5 = new QPushButton(QStringLiteral("重新开始"));
    QPushButton *button6 = new QPushButton(QStringLiteral("暂    停"));
    QPushButton *button7 = new QPushButton(QStringLiteral("主 菜 单"));
    QPushButton *button8 = new QPushButton(QStringLiteral("返回游戏"));
    QPushButton *button9 = new QPushButton(QStringLiteral("结束游戏"));


    connect(button1, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(button3, SIGNAL(clicked()), help, SLOT(show()));
    connect(button4, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(button5, SIGNAL(clicked()), this, SLOT(restartGame()));
    connect(button6, SIGNAL(clicked()), this, SLOT(pauseGame()));
    connect(button7, SIGNAL(clicked()), this, SLOT(finishGame()));
    connect(button8, SIGNAL(clicked()), this, SLOT(returnGame()));
    connect(button9, SIGNAL(clicked()), this, SLOT(finishGame()));

    startButton = scene->addWidget(button1);
    helpButton = scene->addWidget(button3);
    exitButton = scene->addWidget(button4);
    restartButton = scene->addWidget(button5);
    pauseButton = scene->addWidget(button6);
    showMenuButton = scene->addWidget(button7);
    returnButton = scene->addWidget(button8);
    finishButton = scene->addWidget(button9);

    startButton->setPos(370, 200);
    helpButton->setPos(370, 275);
    exitButton->setPos(370, 350);
    restartButton->setPos(600, 150);
    pauseButton->setPos(600, 200);
    showMenuButton->setPos(600, 250);
    returnButton->setPos(370, 200);
    finishButton->setPos(370, 250);

    startButton->setZValue(2);
    helpButton->setZValue(2);
    exitButton->setZValue(2);
    restartButton->setZValue(2);
    returnButton->setZValue(2);
    finishButton->setZValue(2);

    restartButton->hide();
    finishButton->hide();
    pauseButton->hide();
    showMenuButton->hide();
    returnButton->hide();

    //定义背景音乐
    playlist0=new QMediaPlaylist();
    playlist0->addMedia(QUrl::fromLocalFile("..\\Box\\music\\startMusic.wav"));
    //playlist->addMedia(QUrl::fromLocalFile("C:\\Users\\Loring\\Desktop\\Box\\music\\background01.mp3"));
    //playlist->setCurrentIndex(1);
    playlist0->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    player0 = new QMediaPlayer;
    player0->setVolume(40);
    player0->setPlaylist(playlist0);
    player0->play();
}


// 开始游戏
void MyView::startGame()
{
    gameWelcomeText->hide();
    startButton->hide();
   // optionButton->hide();
    helpButton->hide();
    exitButton->hide();
    maskWidget->hide();
    initGame();

    player0->pause();//初始的背景音乐暂停
    playlist1=new QMediaPlaylist();//新建另外一背景音乐
    playlist1->addMedia(QUrl::fromLocalFile("..\\Box\\music\\background.wav"));
    playlist1->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    player1 = new QMediaPlayer;
    player1->setPlaylist(playlist1);
    player1->setVolume(40);
    player1->play();
}

// 初始化游戏
void MyView::initGame()
{
    boxGroup->createBox(QPointF(300, 90));
    boxGroup->setFocus();
    boxGroup->startTimer(INITSPEED);
    gameSpeed = INITSPEED;
    nextBoxGroup->createBox(QPointF(500, 70));

    scene()->setBackgroundBrush(QPixmap(":/image/background01.png"));
    gameScoreText->setHtml(QStringLiteral("<font color=red>0</font>"));
    gameLevelText->setHtml(QStringLiteral("<font color=green>第<br>一<br>幕</font>"));

    restartButton->show();
    pauseButton->show();
    showMenuButton->show();
    gameScoreText->show();
    gameScoreTextTip->show();
    gameLevelText->show();
    topLine->show();
    bottomLine->show();
    leftLine->show();
    rightLine->show();
    // 可能以前返回主菜单时隐藏了boxGroup
    boxGroup->show();

}

// 清空满行
void MyView::clearFullRows()
{
    // 获取比一行方格较大的矩形中包含的所有小方块
    for (int y = 429; y > 50; y -= 20) {
        QList<QGraphicsItem *> list = scene()->items(199, y, 202, 22, Qt::ContainsItemShape,Qt::AscendingOrder);
        // 如果该行已满
        if (list.count() == 10) {
            foreach (QGraphicsItem *item, list) {
                OneBox *box = (OneBox*) item;

                // 在2-2中添加的代码
                QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect;
                box->setGraphicsEffect(blurEffect);
                QPropertyAnimation *animation = new QPropertyAnimation(box, "scale");
                animation->setEasingCurve(QEasingCurve::OutBounce);
                animation->setDuration(250);
                animation->setStartValue(4);
                animation->setEndValue(0.25);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                connect(animation, SIGNAL(finished()), box, SLOT(deleteLater()));

                //清除慢行时的一个音效
                QMediaPlayer *player = new QMediaPlayer;
                player->setMedia(QUrl::fromLocalFile("..\\Box\\music\\boom.wav"));
                player->setVolume(80);
                player->play();
            }
            // 保存满行的位置
            rows << y;
        }
    }
    // 如果有满行，下移满行上面的各行再出现新的方块组
    // 如果没有满行，则直接出现新的方块组
    if(rows.count() > 0) {
        QTimer::singleShot(400, this, SLOT(moveBox()));
    }
    else {
        boxGroup->createBox(QPointF(300, 90), nextBoxGroup->getCurrentShape());
        // 清空并销毁提示方块组中的所有小方块
        nextBoxGroup->clearBoxGroup(true);
        nextBoxGroup->createBox(QPointF(500, 70));
    }

}


// 下移满行上面的所有小方块
void MyView::moveBox()
{
    // 从位置最靠上的满行开始
    for (int i = rows.count(); i > 0;  --i) {
        int row = rows.at(i - 1);
        foreach (QGraphicsItem *item, scene()->items(199, 49, 202, row - 47, Qt::ContainsItemShape,Qt::AscendingOrder)) {
            item->moveBy(0, 20);
        }
    }
    // 更新分数
    updateScore(rows.count());
    // 将满行列表清空为0
    rows.clear();
    // 等所有行下移以后再出现新的方块组
    boxGroup->createBox(QPointF(300, 90), nextBoxGroup->getCurrentShape());
    nextBoxGroup->clearBoxGroup(true);
    nextBoxGroup->createBox(QPointF(500, 70));
}

// 更新分数
void MyView::updateScore(const int fullRowNum)
{
    int score = fullRowNum * 100;
    int currentScore = gameScoreText->toPlainText().toInt();
    currentScore += score;
    // 显示当前分数
    gameScoreText->setHtml(QStringLiteral("<font color=red>%1</font>").arg(currentScore));
    // 判断级别
    if (currentScore < 1000)
    {
        // 第一级,什么都不用做
    }
    else if (currentScore < 2000 && currentScore>900)
    {
        // 第二级
         gameLevelText->setHtml(QStringLiteral("<font color=white>第<br>二<br>幕</font>"));
         scene()->setBackgroundBrush(QPixmap(":/image/background02.png"));
         gameSpeed = 350;
         boxGroup->stopTimer();
         boxGroup->startTimer(gameSpeed);
         player1->setPlaybackRate(1.3);
         // 在2-5中添加的代码
         //if (QFileInfo(backgroundMusic->currentSource().fileName()).baseName()
          //       != "background02") {
          //   backgroundMusic->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background02.mp3"));
          //   backgroundMusic->play();
        // }
    }
    else if (currentScore < 3000 && currentScore>1900) {
        // 添加下一个级别的设置
        gameLevelText->setHtml(QStringLiteral("<font color=white>第<br>三<br>幕</font>"));
        scene()->setBackgroundBrush(QPixmap(":/image/background03.png"));
        gameSpeed = 200;
        boxGroup->stopTimer();
        boxGroup->startTimer(gameSpeed);
        player1->setPlaybackRate(1.5);
    }
    else {
        gameLevelText->setHtml(QStringLiteral("<font color=white>第<br>四<br>幕</font>"));
        scene()->setBackgroundBrush(QPixmap(":/image/background04.jpg"));
        gameSpeed = 120;
        boxGroup->stopTimer();
        boxGroup->startTimer(gameSpeed);
        player1->setPlaybackRate(1.8);
    }

}

void MyView::gameOver()
{
    //QMessageBox::about(this,"提示","Game Over!");
    pauseButton->hide();
    showMenuButton->hide();
    maskWidget->show();
    gameOverText->show();
    restartButton->setPos(370, 200);
    finishButton->show();
}

// 重新开始游戏
void MyView::restartGame()
{
    maskWidget->hide();
    gameOverText->hide();
    finishButton->hide();
    restartButton->setPos(600, 150);

    // 销毁提示方块组和当前方块移动区域中的所有小方块
    nextBoxGroup->clearBoxGroup(true);
    boxGroup->clearBoxGroup();
    boxGroup->hide();
    foreach (QGraphicsItem *item, scene()->items(199, 49, 202, 402, Qt::ContainsItemShape,Qt::AscendingOrder)) {
        // 先从场景中移除小方块，因为使用deleteLater()是在返回主事件循环后才销毁
        // 小方块的，为了在出现新的方块组时不发生碰撞，所以需要先从场景中移除小方块
        scene()->removeItem(item);
        OneBox *box = (OneBox*) item;
        box->deleteLater();
    }
    initGame();
}

// 结束当前游戏
void MyView::finishGame()
{
    gameOverText->hide();
    finishButton->hide();
    restartButton->setPos(600, 150);
    restartButton->hide();
    pauseButton->hide();
    showMenuButton->hide();
    gameScoreText->hide();
    gameScoreTextTip->hide();
    gameLevelText->hide();

    topLine->hide();
    bottomLine->hide();
    leftLine->hide();
    rightLine->hide();

    nextBoxGroup->clearBoxGroup(true);
    boxGroup->clearBoxGroup();
    boxGroup->hide();

    foreach (QGraphicsItem *item, scene()->items(199, 49, 202, 402, Qt::ContainsItemShape,Qt::AscendingOrder)) {
        OneBox *box = (OneBox*) item;
        box->deleteLater();
    }

    // 可能是在进行游戏时按下“主菜单”按钮
    maskWidget->show();
    gameWelcomeText->show();
    startButton->show();
    //optionButton->show();
    helpButton->show();
    exitButton->show();
    scene()->setBackgroundBrush(QPixmap(":/image/background.png"));
    player0->play();
    player1->pause();
}

// 暂停游戏
void MyView::pauseGame()
{
    boxGroup->stopTimer();
    restartButton->hide();
    pauseButton->hide();
    showMenuButton->hide();
    maskWidget->show();
    gamePausedText->show();
    returnButton->show();
    player1->pause();
}


// 返回游戏，处于暂停状态时
void MyView::returnGame()
{
    returnButton->hide();
    gamePausedText->hide();
    maskWidget->hide();
    restartButton->show();
    pauseButton->show();
    showMenuButton->show();
    boxGroup->startTimer(gameSpeed);
    player1->play();
}

void MyView::keyPressEvent(QKeyEvent *event)
{
    //如果正在进行游戏，当键盘按下时总是方块组获得焦点
    if(pauseButton->isVisible())
        boxGroup->setFocus();
    else
        boxGroup->clearFocus();
    QGraphicsView::keyPressEvent(event);
}

void MyView::aboutToFinish(){
  // player->enque
}


