/********************************************************************************
** Form generated from reading UI file 'chess_basket_o2o.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHESS_BASKET_O2O_H
#define UI_CHESS_BASKET_O2O_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "game/view/player_bar/player_bar.h"

QT_BEGIN_NAMESPACE

class Ui_ChessBasketO2O
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
    QHBoxLayout *layout_top_chesses;
    QSpacerItem *horizontalSpacer_6;
    QGridLayout *gridLayout_top_chesses;
    QSpacerItem *horizontalSpacer_5;
    QWidget *wdgt_space_between_side;
    QHBoxLayout *layout_bottom_chesses;
    QSpacerItem *horizontalSpacer_8;
    QGridLayout *gridLayout_bottom_chesses;
    QSpacerItem *horizontalSpacer_7;
    QWidget *wdgt_space_over_bottom_player;
    QWidget *bottom_player_wrapper;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    game::PlayerBar *bottom_player;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ChessBasketO2O)
    {
        if (ChessBasketO2O->objectName().isEmpty())
            ChessBasketO2O->setObjectName(QStringLiteral("ChessBasketO2O"));
        ChessBasketO2O->resize(355, 325);
        verticalLayout = new QVBoxLayout(ChessBasketO2O);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 16, 0, 0);
        lb_title = new QLabel(ChessBasketO2O);
        lb_title->setObjectName(QStringLiteral("lb_title"));
        lb_title->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lb_title);

        wdgt_space_under_titile = new QWidget(ChessBasketO2O);
        wdgt_space_under_titile->setObjectName(QStringLiteral("wdgt_space_under_titile"));

        verticalLayout->addWidget(wdgt_space_under_titile);

        top_player_wrapper = new QWidget(ChessBasketO2O);
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

        wdgt_space_under_top_player = new QWidget(ChessBasketO2O);
        wdgt_space_under_top_player->setObjectName(QStringLiteral("wdgt_space_under_top_player"));

        verticalLayout->addWidget(wdgt_space_under_top_player);

        layout_top_chesses = new QHBoxLayout();
        layout_top_chesses->setSpacing(0);
        layout_top_chesses->setObjectName(QStringLiteral("layout_top_chesses"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_top_chesses->addItem(horizontalSpacer_6);

        gridLayout_top_chesses = new QGridLayout();
        gridLayout_top_chesses->setSpacing(1);
        gridLayout_top_chesses->setObjectName(QStringLiteral("gridLayout_top_chesses"));

        layout_top_chesses->addLayout(gridLayout_top_chesses);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_top_chesses->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(layout_top_chesses);

        wdgt_space_between_side = new QWidget(ChessBasketO2O);
        wdgt_space_between_side->setObjectName(QStringLiteral("wdgt_space_between_side"));

        verticalLayout->addWidget(wdgt_space_between_side);

        layout_bottom_chesses = new QHBoxLayout();
        layout_bottom_chesses->setSpacing(0);
        layout_bottom_chesses->setObjectName(QStringLiteral("layout_bottom_chesses"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_bottom_chesses->addItem(horizontalSpacer_8);

        gridLayout_bottom_chesses = new QGridLayout();
        gridLayout_bottom_chesses->setSpacing(1);
        gridLayout_bottom_chesses->setObjectName(QStringLiteral("gridLayout_bottom_chesses"));

        layout_bottom_chesses->addLayout(gridLayout_bottom_chesses);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_bottom_chesses->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(layout_bottom_chesses);

        wdgt_space_over_bottom_player = new QWidget(ChessBasketO2O);
        wdgt_space_over_bottom_player->setObjectName(QStringLiteral("wdgt_space_over_bottom_player"));

        verticalLayout->addWidget(wdgt_space_over_bottom_player);

        bottom_player_wrapper = new QWidget(ChessBasketO2O);
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


        retranslateUi(ChessBasketO2O);

        QMetaObject::connectSlotsByName(ChessBasketO2O);
    } // setupUi

    void retranslateUi(QWidget *ChessBasketO2O)
    {
        ChessBasketO2O->setWindowTitle(QApplication::translate("ChessBasketO2O", "Form", 0));
        lb_title->setText(QApplication::translate("ChessBasketO2O", "\346\225\231\345\255\246\346\250\241\345\274\217", 0));
    } // retranslateUi

};

namespace Ui {
    class ChessBasketO2O: public Ui_ChessBasketO2O {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHESS_BASKET_O2O_H
