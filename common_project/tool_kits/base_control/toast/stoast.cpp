﻿#include "stoast.h"

//静态成员变量初始化。
QMutex SingleTonSToast::mutex;
QAtomicPointer</*SingleTon*/SToast> SingleTonSToast::instance = 0;


SToast::SToast(QWidget *parent) : QWidget(parent)
{
    //初始化成员
    msDuration = 5000;
    animation = new QPropertyAnimation(this, "windowOpacity");
	//this->setAttribute(Qt::WA_DeleteOnClose);
    //初始化窗口属性
    setWindowOpacity(0);  //达到比较好的淡出效果，需要将窗口设置完全透明，否则会有卡顿的效果
    this->setAttribute(Qt::WA_TranslucentBackground,true);  //设置背景透明
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);

    //窗口大小及位置属性
    setFixedSize(500,90);  //设置为固定窗口大小达到禁止调节窗口大小
    // Unbtun下有当前桌面扩展，显示的桌面只是逻辑上的一部分，所以看不到居中的效果
    //move((QApplication::desktop()->width() - width())/2,(QApplication::desktop()->height() - height())/2);
    // 此方方法可以解决居中显示的问题
    QRect curDesktopRc = QApplication::desktop()->screenGeometry(this);  //获取显示本窗口的屏幕区域
    move((curDesktopRc.width() - width())/2, (curDesktopRc.height() - height())/2); //屏幕居中显示

    //图标、字容器初始化
    labIcon = new QLabel();
    label = new QLabel();
	label->setStyleSheet("background: #b7000000;color: white;border-radius: 20px;");
    label->setFont(QFont("Microsoft YaHei",12));
    label->setAlignment(Qt::AlignCenter);
    label->adjustSize();        //自适应文本
    label->setWordWrap(true);   //自动折行
    label->setMargin(16);        //边距调整

    //初始化布局
    vLayout = new QVBoxLayout();  //承载文字
    vLayout->addWidget(label);
    vLayout->setAlignment(Qt::AlignCenter);

    hLayout = new QHBoxLayout();  //承载图标和vLayout
    //hLayout->addWidget(labIcon);  预留显示图标
    hLayout->addLayout(vLayout);
    hLayout->setAlignment(Qt::AlignCenter);
    this->setLayout(hLayout);

    //初始化定时器
    connect(&durationTimer, &QTimer::timeout, this, &SToast::timeOver);
}

SToast::~SToast()
{
    delete label; label = NULL;
    delete labIcon; labIcon = NULL;
    delete vLayout; vLayout = NULL;
    delete hLayout; hLayout = NULL;
}

int SToast::duration() const
{
    return msDuration;
}

void SToast::SetDuration(int msec)
{
    msDuration = msec;
}

void SToast::setMessageVDuration(QString msg, int msecDisplayTime/*=2500*/)
{
    // 待完善的功能
    // 1. 对消息进行排队显示，忽略用户指定的显示时间，采用默认的显示时间
    // 2. 对msg的长度适当限制

    this->show();

    if(!msg.isEmpty()){
        label->setText(msg);
    }
    else{
        label->setText("");
    }
    msDuration = msecDisplayTime;

    //淡出画效果
    fadeOutAnimation();

    //开始显示计时
    durationTimer.start(msDuration);
}

void SToast::fadeInAnimation()
{
    //界面动画，改变透明度的方式消失1 - 0渐变
    if(!animation)
        return;
    connect(animation, &QPropertyAnimation::finished, this, &SToast::fadeInAnimationFinished );

    animation->setDuration(1500);
    animation->setStartValue(0.8);
    animation->setEndValue(0);
    //animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}

void SToast::fadeOutAnimation()
{
    //界面动画，改变透明度的方式出现0 - 1渐变
    if(!animation)
        return;
    animation->setDuration(1500);
    animation->setStartValue(0);
    animation->setEndValue(0.8);
    animation->start();
}

void SToast::timeOver()
{
    durationTimer.stop();

    //测试
    //label->setText("显示时间到，你还有什么想说的吗？");

    //淡入动画效果
    fadeInAnimation();
}

void SToast::fadeInAnimationFinished()
{
    disconnect(animation, &QPropertyAnimation::finished, this, &SToast::fadeInAnimationFinished );
    this->close();
}

void SToast::paintEvent(QPaintEvent *event)
{
    //TODO: coding below


    QWidget::paintEvent(event);
}
