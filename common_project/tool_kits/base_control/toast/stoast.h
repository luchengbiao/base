// 仿Android的toast提示框
// 适宜将此类实现为多线程下的单例模式

#ifndef STOAST_H
#define STOAST_H

//// SToast 部分用到
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QPainter>
#include <QPointer>
#include <QEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QPropertyAnimation>

//// SingleTonSToast 部分用到
#include <QMutex>
#include <QReadWriteLock>
#include <QAtomicPointer>
#include <QDebug>

class SToast : public QWidget
{
    Q_OBJECT
public:
    explicit SToast(QWidget *parent = 0);

    int					duration()const;
    void				SetDuration(int msec);  //持续显示时间，ms

	int					durationOfFade() const;
	void				setDurationOfFade(int msec);

	unsigned int		wordCountUpperToWrap() const;
	void				setWordCountUpperToWrap(unsigned int word_count);

signals:

public slots:
	void				setMessageVDuration(QString msg, 
											int msecDisplayTime = 2500, 
											const QWidget* widgetToBeAlignedOnCenter = nullptr);  //消息显示多少毫秒消失

	void				stop();

protected:
	virtual void		paintEvent(QPaintEvent *event) override;
	virtual	bool		eventFilter(QObject *, QEvent *) override;
	
	void				fadeInAnimation();   //淡出动画
	void				fadeOutAnimation();  //淡入动画

private slots:
    void				timeOver();  //定时器超时响应函数
    void				fadeInAnimationFinished();  //淡出动画结束响应槽

private:
	QHBoxLayout*		hLayout{ nullptr };
	QVBoxLayout*		vLayout{ nullptr };
	QLabel*				label{ nullptr };   //提示文字
	QLabel*				labIcon{ nullptr }; //图标
	int					msDuration{ 0 };  //单位ms
	int					msDurationOfFade{ 1500 };  //单位ms
	unsigned int		nWordCountUpperToWrap{ 10 };
	QTimer				durationTimer;
	QPropertyAnimation*	animation{ nullptr };
	QPointer<QWidget>	widgetToBeAligned{ nullptr };
};

class SingleTonSToast
{
public:
    /*! \brief 用于获得SingleTonSToast实例，使用单例模式。
     *  \return SingleTonSToast实例的引用。
     */
    static /*SingleTonSToast*/SToast &getInstance(void)
    {
        //使用双重检测。
        /*! testAndSetOrders操作保证在原子操作前和后的的内存访问
         * 不会被重新排序。
         */
        if(instance.testAndSetOrdered(0, 0)){  //第一次检测
            QMutexLocker locker(&mutex);//加互斥锁。
            instance.testAndSetOrdered(0, new /*SingleTonSToast*/SToast);//第二次检测。
        }

        return *instance;
    }

private:
    SingleTonSToast();  //禁止构造函数。
    SingleTonSToast(const SingleTonSToast &);//禁止拷贝构造函数。
    SingleTonSToast & operator=(const SingleTonSToast &);//禁止赋值拷贝函数。

    static QMutex mutex;            //实例互斥锁。
    static QAtomicPointer</*SingleTonSToast*/SToast> instance;  /*!<使用原子指针,默认初始化为0。*/
};

#endif // STOAST_H
