/********************************************************************************
** Form generated from reading UI file 'go_basket_o2o.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GO_BASKET_O2O_H
#define UI_GO_BASKET_O2O_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "game/view/player_bar/player_bar.h"

QT_BEGIN_NAMESPACE

class Ui_GoBasketO2O
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lb_title;
    QWidget *wdgt_space_under_titile;
    QWidget *top_player_wrapper;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    game::PlayerBar *top_player;
    QSpacerItem *horizontalSpacer;
    QWidget *wdgt_space_under_top_player;
    QHBoxLayout *layout_top_basket_wrapper;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *layout_top_basket;
    QSpacerItem *horizontalSpacer_5;
    QWidget *wdgt_space_between_side;
    QHBoxLayout *layout_bottom_basket_wrapper;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *layout_bottom_basket;
    QSpacerItem *horizontalSpacer_7;
    QWidget *wdgt_space_over_bottom_player;
    QWidget *wdgt_auto_taking_pieces_wrapper;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_10;
    QCheckBox *chk_auto_taking_pieces;
    QLabel *lb_auto_taking_pieces;
    QSpacerItem *horizontalSpacer_9;
    QWidget *bottom_player_wrapper;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    game::PlayerBar *bottom_player;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *GoBasketO2O)
    {
        if (GoBasketO2O->objectName().isEmpty())
            GoBasketO2O->setObjectName(QStringLiteral("GoBasketO2O"));
        GoBasketO2O->resize(325, 634);
        verticalLayout = new QVBoxLayout(GoBasketO2O);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 16, 0, 0);
        lb_title = new QLabel(GoBasketO2O);
        lb_title->setObjectName(QStringLiteral("lb_title"));
        lb_title->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lb_title);

        wdgt_space_under_titile = new QWidget(GoBasketO2O);
        wdgt_space_under_titile->setObjectName(QStringLiteral("wdgt_space_under_titile"));

        verticalLayout->addWidget(wdgt_space_under_titile);

        top_player_wrapper = new QWidget(GoBasketO2O);
        top_player_wrapper->setObjectName(QStringLiteral("top_player_wrapper"));
        horizontalLayout_2 = new QHBoxLayout(top_player_wrapper);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        top_player = new game::PlayerBar(top_player_wrapper);
        top_player->setObjectName(QStringLiteral("top_player"));

        horizontalLayout_2->addWidget(top_player);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addWidget(top_player_wrapper);

        wdgt_space_under_top_player = new QWidget(GoBasketO2O);
        wdgt_space_under_top_player->setObjectName(QStringLiteral("wdgt_space_under_top_player"));

        verticalLayout->addWidget(wdgt_space_under_top_player);

        layout_top_basket_wrapper = new QHBoxLayout();
        layout_top_basket_wrapper->setSpacing(0);
        layout_top_basket_wrapper->setObjectName(QStringLiteral("layout_top_basket_wrapper"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_top_basket_wrapper->addItem(horizontalSpacer_6);

        layout_top_basket = new QHBoxLayout();
        layout_top_basket->setObjectName(QStringLiteral("layout_top_basket"));

        layout_top_basket_wrapper->addLayout(layout_top_basket);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_top_basket_wrapper->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(layout_top_basket_wrapper);

        wdgt_space_between_side = new QWidget(GoBasketO2O);
        wdgt_space_between_side->setObjectName(QStringLiteral("wdgt_space_between_side"));

        verticalLayout->addWidget(wdgt_space_between_side);

        layout_bottom_basket_wrapper = new QHBoxLayout();
        layout_bottom_basket_wrapper->setSpacing(0);
        layout_bottom_basket_wrapper->setObjectName(QStringLiteral("layout_bottom_basket_wrapper"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_bottom_basket_wrapper->addItem(horizontalSpacer_8);

        layout_bottom_basket = new QHBoxLayout();
        layout_bottom_basket->setObjectName(QStringLiteral("layout_bottom_basket"));

        layout_bottom_basket_wrapper->addLayout(layout_bottom_basket);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_bottom_basket_wrapper->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(layout_bottom_basket_wrapper);

        wdgt_space_over_bottom_player = new QWidget(GoBasketO2O);
        wdgt_space_over_bottom_player->setObjectName(QStringLiteral("wdgt_space_over_bottom_player"));

        verticalLayout->addWidget(wdgt_space_over_bottom_player);

        wdgt_auto_taking_pieces_wrapper = new QWidget(GoBasketO2O);
        wdgt_auto_taking_pieces_wrapper->setObjectName(QStringLiteral("wdgt_auto_taking_pieces_wrapper"));
        horizontalLayout = new QHBoxLayout(wdgt_auto_taking_pieces_wrapper);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(1);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        chk_auto_taking_pieces = new QCheckBox(wdgt_auto_taking_pieces_wrapper);
        chk_auto_taking_pieces->setObjectName(QStringLiteral("chk_auto_taking_pieces"));

        horizontalLayout_4->addWidget(chk_auto_taking_pieces);

        lb_auto_taking_pieces = new QLabel(wdgt_auto_taking_pieces_wrapper);
        lb_auto_taking_pieces->setObjectName(QStringLiteral("lb_auto_taking_pieces"));

        horizontalLayout_4->addWidget(lb_auto_taking_pieces);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);


        horizontalLayout->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(wdgt_auto_taking_pieces_wrapper);

        bottom_player_wrapper = new QWidget(GoBasketO2O);
        bottom_player_wrapper->setObjectName(QStringLiteral("bottom_player_wrapper"));
        horizontalLayout_3 = new QHBoxLayout(bottom_player_wrapper);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        bottom_player = new game::PlayerBar(bottom_player_wrapper);
        bottom_player->setObjectName(QStringLiteral("bottom_player"));

        horizontalLayout_3->addWidget(bottom_player);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout->addWidget(bottom_player_wrapper);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(GoBasketO2O);

        QMetaObject::connectSlotsByName(GoBasketO2O);
    } // setupUi

    void retranslateUi(QWidget *GoBasketO2O)
    {
        GoBasketO2O->setWindowTitle(QApplication::translate("GoBasketO2O", "Form", 0));
        lb_title->setText(QApplication::translate("GoBasketO2O", "\346\225\231\345\255\246\346\250\241\345\274\217", 0));
        chk_auto_taking_pieces->setText(QString());
        lb_auto_taking_pieces->setText(QApplication::translate("GoBasketO2O", "\350\207\252\345\212\250\346\217\220\345\255\220", 0));
    } // retranslateUi

};

namespace Ui {
    class GoBasketO2O: public Ui_GoBasketO2O {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GO_BASKET_O2O_H
