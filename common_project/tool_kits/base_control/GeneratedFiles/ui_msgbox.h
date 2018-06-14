/********************************************************************************
** Form generated from reading UI file 'msgbox.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MSGBOX_H
#define UI_MSGBOX_H

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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MsgBox
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QWidget *wid_title;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lb_title;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *btn_close;
    QVBoxLayout *verticalLayout;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *top_title;
    QSpacerItem *horizontalSpacer_8;
    QLabel *lb_exclamation;
    QLabel *lb_top_title;
    QSpacerItem *horizontalSpacer_9;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *lb_top_title1;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lab_msg;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_cancel;
    QPushButton *btn_ok;
    QPushButton *btn_tmp;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *MsgBox)
    {
        if (MsgBox->objectName().isEmpty())
            MsgBox->setObjectName(QStringLiteral("MsgBox"));
        MsgBox->resize(530, 288);
        verticalLayout_2 = new QVBoxLayout(MsgBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(6, 4, 6, 8);
        frame = new QFrame(MsgBox);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        wid_title = new QWidget(frame);
        wid_title->setObjectName(QStringLiteral("wid_title"));
        horizontalLayout_4 = new QHBoxLayout(wid_title);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(20, 0, 20, 0);
        lb_title = new QLabel(wid_title);
        lb_title->setObjectName(QStringLiteral("lb_title"));

        horizontalLayout_4->addWidget(lb_title);

        horizontalSpacer_7 = new QSpacerItem(310, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        btn_close = new QPushButton(wid_title);
        btn_close->setObjectName(QStringLiteral("btn_close"));

        horizontalLayout_4->addWidget(btn_close);


        verticalLayout_3->addWidget(wid_title);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 0, -1, -1);
        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_7 = new QHBoxLayout(frame_3);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        top_title = new QHBoxLayout();
        top_title->setObjectName(QStringLiteral("top_title"));
        top_title->setContentsMargins(-1, 10, -1, 10);
        horizontalSpacer_8 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        top_title->addItem(horizontalSpacer_8);

        lb_exclamation = new QLabel(frame_3);
        lb_exclamation->setObjectName(QStringLiteral("lb_exclamation"));

        top_title->addWidget(lb_exclamation);

        lb_top_title = new QLabel(frame_3);
        lb_top_title->setObjectName(QStringLiteral("lb_top_title"));

        top_title->addWidget(lb_top_title);

        horizontalSpacer_9 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        top_title->addItem(horizontalSpacer_9);


        horizontalLayout_7->addLayout(top_title);


        verticalLayout->addWidget(frame_3);

        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_2);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(23, 0, 23, 0);
        lb_top_title1 = new QLabel(frame_2);
        lb_top_title1->setObjectName(QStringLiteral("lb_top_title1"));
        lb_top_title1->setWordWrap(false);

        horizontalLayout_6->addWidget(lb_top_title1);


        horizontalLayout_5->addLayout(horizontalLayout_6);


        verticalLayout->addWidget(frame_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(23, -1, 23, -1);
        lab_msg = new QLabel(frame);
        lab_msg->setObjectName(QStringLiteral("lab_msg"));

        horizontalLayout_3->addWidget(lab_msg);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);


        verticalLayout_3->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));

        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(13);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_cancel = new QPushButton(frame);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));
        btn_cancel->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout->addWidget(btn_cancel);

        btn_ok = new QPushButton(frame);
        btn_ok->setObjectName(QStringLiteral("btn_ok"));
        btn_ok->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout->addWidget(btn_ok);

        btn_tmp = new QPushButton(frame);
        btn_tmp->setObjectName(QStringLiteral("btn_tmp"));

        horizontalLayout->addWidget(btn_tmp);

        horizontalSpacer_2 = new QSpacerItem(20, 15, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_2);


        verticalLayout_2->addWidget(frame);


        retranslateUi(MsgBox);

        QMetaObject::connectSlotsByName(MsgBox);
    } // setupUi

    void retranslateUi(QDialog *MsgBox)
    {
        MsgBox->setWindowTitle(QApplication::translate("MsgBox", "Dialog", 0));
        lb_title->setText(QString());
        btn_close->setText(QString());
        lb_exclamation->setText(QString());
        lb_top_title->setText(QString());
        lb_top_title1->setText(QString());
        lab_msg->setText(QString());
        btn_cancel->setText(QApplication::translate("MsgBox", "\345\217\226\346\266\210", 0));
        btn_ok->setText(QApplication::translate("MsgBox", "\347\241\256\345\256\232", 0));
        btn_tmp->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MsgBox: public Ui_MsgBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MSGBOX_H
