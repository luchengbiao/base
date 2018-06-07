/********************************************************************************
** Form generated from reading UI file 'imageviewtoolform.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEVIEWTOOLFORM_H
#define UI_IMAGEVIEWTOOLFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_imageViewToolForm
{
public:
    QFrame *frame;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *viewToolLayout;
    QToolButton *btn_normal;
    QToolButton *btn_max;
    QToolButton *btn_restore;
    QToolButton *btn_magnify;
    QToolButton *btn_reduce;
    QToolButton *btn_rotate;
    QToolButton *btn_save;

    void setupUi(QWidget *imageViewToolForm)
    {
        if (imageViewToolForm->objectName().isEmpty())
            imageViewToolForm->setObjectName(QStringLiteral("imageViewToolForm"));
        imageViewToolForm->resize(510, 140);
        frame = new QFrame(imageViewToolForm);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 0, 381, 81));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayoutWidget = new QWidget(frame);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 371, 71));
        viewToolLayout = new QHBoxLayout(horizontalLayoutWidget);
        viewToolLayout->setSpacing(5);
        viewToolLayout->setObjectName(QStringLiteral("viewToolLayout"));
        viewToolLayout->setContentsMargins(20, 0, 0, 5);
        btn_normal = new QToolButton(horizontalLayoutWidget);
        btn_normal->setObjectName(QStringLiteral("btn_normal"));

        viewToolLayout->addWidget(btn_normal);

        btn_max = new QToolButton(horizontalLayoutWidget);
        btn_max->setObjectName(QStringLiteral("btn_max"));

        viewToolLayout->addWidget(btn_max);

        btn_restore = new QToolButton(horizontalLayoutWidget);
        btn_restore->setObjectName(QStringLiteral("btn_restore"));

        viewToolLayout->addWidget(btn_restore);

        btn_magnify = new QToolButton(horizontalLayoutWidget);
        btn_magnify->setObjectName(QStringLiteral("btn_magnify"));

        viewToolLayout->addWidget(btn_magnify);

        btn_reduce = new QToolButton(horizontalLayoutWidget);
        btn_reduce->setObjectName(QStringLiteral("btn_reduce"));

        viewToolLayout->addWidget(btn_reduce);

        btn_rotate = new QToolButton(horizontalLayoutWidget);
        btn_rotate->setObjectName(QStringLiteral("btn_rotate"));

        viewToolLayout->addWidget(btn_rotate);

        btn_save = new QToolButton(horizontalLayoutWidget);
        btn_save->setObjectName(QStringLiteral("btn_save"));

        viewToolLayout->addWidget(btn_save);


        retranslateUi(imageViewToolForm);

        QMetaObject::connectSlotsByName(imageViewToolForm);
    } // setupUi

    void retranslateUi(QWidget *imageViewToolForm)
    {
        imageViewToolForm->setWindowTitle(QApplication::translate("imageViewToolForm", "Form", 0));
        btn_normal->setText(QString());
        btn_max->setText(QString());
        btn_restore->setText(QString());
        btn_magnify->setText(QString());
        btn_reduce->setText(QString());
        btn_rotate->setText(QString());
        btn_save->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class imageViewToolForm: public Ui_imageViewToolForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEVIEWTOOLFORM_H
