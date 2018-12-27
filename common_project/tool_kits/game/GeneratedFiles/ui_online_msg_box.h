/********************************************************************************
** Form generated from reading UI file 'online_msg_box.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONLINE_MSG_BOX_H
#define UI_ONLINE_MSG_BOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_OnlineMsgBox
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLabel *lb_title;
    QPushButton *btn_close;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QLabel *lb_message;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btn_confirm;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_6;
    QLabel *lb_countdown;
    QSpacerItem *horizontalSpacer_5;

    void setupUi(QDialog *OnlineMsgBox)
    {
        if (OnlineMsgBox->objectName().isEmpty())
            OnlineMsgBox->setObjectName(QStringLiteral("OnlineMsgBox"));
        OnlineMsgBox->resize(400, 300);
        verticalLayout_2 = new QVBoxLayout(OnlineMsgBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(16, 18, 16, 18);
        frame = new QFrame(OnlineMsgBox);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(14, 0, 14, 10);
        lb_title = new QLabel(frame);
        lb_title->setObjectName(QStringLiteral("lb_title"));

        horizontalLayout->addWidget(lb_title);

        btn_close = new QPushButton(frame);
        btn_close->setObjectName(QStringLiteral("btn_close"));

        horizontalLayout->addWidget(btn_close);


        verticalLayout_2->addWidget(frame);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 0);
        lb_message = new QLabel(OnlineMsgBox);
        lb_message->setObjectName(QStringLiteral("lb_message"));
        lb_message->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lb_message);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 18, -1, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        btn_confirm = new QPushButton(OnlineMsgBox);
        btn_confirm->setObjectName(QStringLiteral("btn_confirm"));

        horizontalLayout_4->addWidget(btn_confirm);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);


        horizontalLayout_2->addLayout(verticalLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, -1, -1, 0);
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        lb_countdown = new QLabel(OnlineMsgBox);
        lb_countdown->setObjectName(QStringLiteral("lb_countdown"));

        horizontalLayout_3->addWidget(lb_countdown);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_3);


        retranslateUi(OnlineMsgBox);

        QMetaObject::connectSlotsByName(OnlineMsgBox);
    } // setupUi

    void retranslateUi(QDialog *OnlineMsgBox)
    {
        OnlineMsgBox->setWindowTitle(QApplication::translate("OnlineMsgBox", "Dialog", 0));
        lb_title->setText(QString());
        btn_close->setText(QString());
        lb_message->setText(QString());
        btn_confirm->setText(QString());
        lb_countdown->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class OnlineMsgBox: public Ui_OnlineMsgBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONLINE_MSG_BOX_H
