/********************************************************************************
** Form generated from reading UI file 'imageclipform.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGECLIPFORM_H
#define UI_IMAGECLIPFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageClipForm
{
public:
    QLabel *lab_image;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *vlayout_box;
    QVBoxLayout *verticalLayout;
    QLabel *lab_top;
    QHBoxLayout *horizontalLayout;
    QLabel *lab_left;
    QVBoxLayout *image_vlayout;
    QLabel *lab_top_shadow;
    QHBoxLayout *image_hlayout;
    QLabel *lab_left_shadow;
    QVBoxLayout *image_vlayout2;
    QHBoxLayout *image_hlayout_top;
    QLabel *lab_top_left;
    QSpacerItem *horizontalSpacer;
    QLabel *lab_top_right;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *image_hlayout_bottom;
    QLabel *lab_bottom_left;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lab_bottom_right;
    QLabel *lab_right_shadow;
    QLabel *lab_bottom_shadow;
    QLabel *lab_right;
    QLabel *lab_bottom;
    QHBoxLayout *layout_bottom_bar;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btn_cancel;
    QPushButton *btn_rotate;
    QPushButton *btn_upload_2;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *ImageClipForm)
    {
        if (ImageClipForm->objectName().isEmpty())
            ImageClipForm->setObjectName(QStringLiteral("ImageClipForm"));
        ImageClipForm->resize(700, 500);
        lab_image = new QLabel(ImageClipForm);
        lab_image->setObjectName(QStringLiteral("lab_image"));
        lab_image->setGeometry(QRect(60, 480, 151, 31));
        verticalLayoutWidget_2 = new QWidget(ImageClipForm);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(9, 9, 681, 481));
        vlayout_box = new QVBoxLayout(verticalLayoutWidget_2);
        vlayout_box->setSpacing(10);
        vlayout_box->setObjectName(QStringLiteral("vlayout_box"));
        vlayout_box->setContentsMargins(12, 12, 12, 22);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lab_top = new QLabel(verticalLayoutWidget_2);
        lab_top->setObjectName(QStringLiteral("lab_top"));

        verticalLayout->addWidget(lab_top);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lab_left = new QLabel(verticalLayoutWidget_2);
        lab_left->setObjectName(QStringLiteral("lab_left"));

        horizontalLayout->addWidget(lab_left);

        image_vlayout = new QVBoxLayout();
        image_vlayout->setSpacing(0);
        image_vlayout->setObjectName(QStringLiteral("image_vlayout"));
        image_vlayout->setContentsMargins(0, 0, 0, 0);
        lab_top_shadow = new QLabel(verticalLayoutWidget_2);
        lab_top_shadow->setObjectName(QStringLiteral("lab_top_shadow"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lab_top_shadow->sizePolicy().hasHeightForWidth());
        lab_top_shadow->setSizePolicy(sizePolicy);

        image_vlayout->addWidget(lab_top_shadow);

        image_hlayout = new QHBoxLayout();
        image_hlayout->setSpacing(0);
        image_hlayout->setObjectName(QStringLiteral("image_hlayout"));
        lab_left_shadow = new QLabel(verticalLayoutWidget_2);
        lab_left_shadow->setObjectName(QStringLiteral("lab_left_shadow"));
        sizePolicy.setHeightForWidth(lab_left_shadow->sizePolicy().hasHeightForWidth());
        lab_left_shadow->setSizePolicy(sizePolicy);

        image_hlayout->addWidget(lab_left_shadow);

        image_vlayout2 = new QVBoxLayout();
        image_vlayout2->setObjectName(QStringLiteral("image_vlayout2"));
        image_hlayout_top = new QHBoxLayout();
        image_hlayout_top->setSpacing(0);
        image_hlayout_top->setObjectName(QStringLiteral("image_hlayout_top"));
        lab_top_left = new QLabel(verticalLayoutWidget_2);
        lab_top_left->setObjectName(QStringLiteral("lab_top_left"));
        sizePolicy.setHeightForWidth(lab_top_left->sizePolicy().hasHeightForWidth());
        lab_top_left->setSizePolicy(sizePolicy);

        image_hlayout_top->addWidget(lab_top_left);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        image_hlayout_top->addItem(horizontalSpacer);

        lab_top_right = new QLabel(verticalLayoutWidget_2);
        lab_top_right->setObjectName(QStringLiteral("lab_top_right"));

        image_hlayout_top->addWidget(lab_top_right);


        image_vlayout2->addLayout(image_hlayout_top);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        image_vlayout2->addItem(verticalSpacer);

        image_hlayout_bottom = new QHBoxLayout();
        image_hlayout_bottom->setSpacing(0);
        image_hlayout_bottom->setObjectName(QStringLiteral("image_hlayout_bottom"));
        lab_bottom_left = new QLabel(verticalLayoutWidget_2);
        lab_bottom_left->setObjectName(QStringLiteral("lab_bottom_left"));

        image_hlayout_bottom->addWidget(lab_bottom_left);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        image_hlayout_bottom->addItem(horizontalSpacer_2);

        lab_bottom_right = new QLabel(verticalLayoutWidget_2);
        lab_bottom_right->setObjectName(QStringLiteral("lab_bottom_right"));

        image_hlayout_bottom->addWidget(lab_bottom_right);


        image_vlayout2->addLayout(image_hlayout_bottom);


        image_hlayout->addLayout(image_vlayout2);

        lab_right_shadow = new QLabel(verticalLayoutWidget_2);
        lab_right_shadow->setObjectName(QStringLiteral("lab_right_shadow"));
        sizePolicy.setHeightForWidth(lab_right_shadow->sizePolicy().hasHeightForWidth());
        lab_right_shadow->setSizePolicy(sizePolicy);

        image_hlayout->addWidget(lab_right_shadow);


        image_vlayout->addLayout(image_hlayout);

        lab_bottom_shadow = new QLabel(verticalLayoutWidget_2);
        lab_bottom_shadow->setObjectName(QStringLiteral("lab_bottom_shadow"));
        sizePolicy.setHeightForWidth(lab_bottom_shadow->sizePolicy().hasHeightForWidth());
        lab_bottom_shadow->setSizePolicy(sizePolicy);

        image_vlayout->addWidget(lab_bottom_shadow);


        horizontalLayout->addLayout(image_vlayout);

        lab_right = new QLabel(verticalLayoutWidget_2);
        lab_right->setObjectName(QStringLiteral("lab_right"));

        horizontalLayout->addWidget(lab_right);


        verticalLayout->addLayout(horizontalLayout);

        lab_bottom = new QLabel(verticalLayoutWidget_2);
        lab_bottom->setObjectName(QStringLiteral("lab_bottom"));

        verticalLayout->addWidget(lab_bottom);


        vlayout_box->addLayout(verticalLayout);

        layout_bottom_bar = new QHBoxLayout();
        layout_bottom_bar->setObjectName(QStringLiteral("layout_bottom_bar"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_bottom_bar->addItem(horizontalSpacer_3);

        btn_cancel = new QPushButton(verticalLayoutWidget_2);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));

        layout_bottom_bar->addWidget(btn_cancel);

        btn_rotate = new QPushButton(verticalLayoutWidget_2);
        btn_rotate->setObjectName(QStringLiteral("btn_rotate"));

        layout_bottom_bar->addWidget(btn_rotate);

        btn_upload_2 = new QPushButton(verticalLayoutWidget_2);
        btn_upload_2->setObjectName(QStringLiteral("btn_upload_2"));
        btn_upload_2->setCursor(QCursor(Qt::PointingHandCursor));

        layout_bottom_bar->addWidget(btn_upload_2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_bottom_bar->addItem(horizontalSpacer_4);


        vlayout_box->addLayout(layout_bottom_bar);

        verticalLayoutWidget_2->raise();
        lab_image->raise();

        retranslateUi(ImageClipForm);

        QMetaObject::connectSlotsByName(ImageClipForm);
    } // setupUi

    void retranslateUi(QWidget *ImageClipForm)
    {
        ImageClipForm->setWindowTitle(QApplication::translate("ImageClipForm", "Form", 0));
        lab_image->setText(QString());
        lab_top->setText(QString());
        lab_left->setText(QString());
        lab_top_shadow->setText(QString());
        lab_left_shadow->setText(QString());
        lab_top_left->setText(QString());
        lab_top_right->setText(QString());
        lab_bottom_left->setText(QString());
        lab_bottom_right->setText(QString());
        lab_right_shadow->setText(QString());
        lab_bottom_shadow->setText(QString());
        lab_right->setText(QString());
        lab_bottom->setText(QString());
        btn_cancel->setText(QApplication::translate("ImageClipForm", "\345\217\226\346\266\210", 0));
        btn_rotate->setText(QApplication::translate("ImageClipForm", "\346\227\213\350\275\254", 0));
        btn_upload_2->setText(QApplication::translate("ImageClipForm", "\344\270\212\344\274\240", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageClipForm: public Ui_ImageClipForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGECLIPFORM_H
