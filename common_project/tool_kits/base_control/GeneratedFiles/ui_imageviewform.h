/********************************************************************************
** Form generated from reading UI file 'imageviewform.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEVIEWFORM_H
#define UI_IMAGEVIEWFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_imageViewForm
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QToolButton *btn_close;
    QLabel *lab_image;
    QLabel *lab_num;
    QToolButton *btn_next;
    QToolButton *btn_pre;
    QToolButton *btn_close2;

    void setupUi(QWidget *imageViewForm)
    {
        if (imageViewForm->objectName().isEmpty())
            imageViewForm->setObjectName(QStringLiteral("imageViewForm"));
        imageViewForm->resize(700, 500);
        verticalLayout = new QVBoxLayout(imageViewForm);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(imageViewForm);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        btn_close = new QToolButton(frame);
        btn_close->setObjectName(QStringLiteral("btn_close"));
        btn_close->setGeometry(QRect(660, 0, 37, 18));
        lab_image = new QLabel(frame);
        lab_image->setObjectName(QStringLiteral("lab_image"));
        lab_image->setGeometry(QRect(200, 160, 251, 131));
        lab_num = new QLabel(frame);
        lab_num->setObjectName(QStringLiteral("lab_num"));
        lab_num->setGeometry(QRect(60, 60, 54, 12));
        btn_next = new QToolButton(frame);
        btn_next->setObjectName(QStringLiteral("btn_next"));
        btn_next->setGeometry(QRect(630, 230, 37, 18));
        btn_pre = new QToolButton(frame);
        btn_pre->setObjectName(QStringLiteral("btn_pre"));
        btn_pre->setGeometry(QRect(10, 230, 37, 18));
        btn_close2 = new QToolButton(frame);
        btn_close2->setObjectName(QStringLiteral("btn_close2"));
        btn_close2->setGeometry(QRect(600, 0, 37, 18));

        verticalLayout->addWidget(frame);


        retranslateUi(imageViewForm);

        QMetaObject::connectSlotsByName(imageViewForm);
    } // setupUi

    void retranslateUi(QWidget *imageViewForm)
    {
        imageViewForm->setWindowTitle(QApplication::translate("imageViewForm", "Form", 0));
        btn_close->setText(QApplication::translate("imageViewForm", "...", 0));
        lab_image->setText(QString());
        lab_num->setText(QString());
        btn_next->setText(QApplication::translate("imageViewForm", "...", 0));
        btn_pre->setText(QApplication::translate("imageViewForm", "...", 0));
        btn_close2->setText(QApplication::translate("imageViewForm", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class imageViewForm: public Ui_imageViewForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEVIEWFORM_H
