/********************************************************************************
** Form generated from reading UI file 'player_bar.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYER_BAR_H
#define UI_PLAYER_BAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "game/view/head_portrait/head_portrait.h"

QT_BEGIN_NAMESPACE

class Ui_PlayerBar
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    game::HeadPortrait *head_portrait;
    QLabel *lb_name;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *PlayerBar)
    {
        if (PlayerBar->objectName().isEmpty())
            PlayerBar->setObjectName(QStringLiteral("PlayerBar"));
        PlayerBar->resize(313, 164);
        verticalLayout = new QVBoxLayout(PlayerBar);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        head_portrait = new game::HeadPortrait(PlayerBar);
        head_portrait->setObjectName(QStringLiteral("head_portrait"));

        horizontalLayout->addWidget(head_portrait);

        lb_name = new QLabel(PlayerBar);
        lb_name->setObjectName(QStringLiteral("lb_name"));

        horizontalLayout->addWidget(lb_name);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        retranslateUi(PlayerBar);

        QMetaObject::connectSlotsByName(PlayerBar);
    } // setupUi

    void retranslateUi(QWidget *PlayerBar)
    {
        PlayerBar->setWindowTitle(QApplication::translate("PlayerBar", "Form", 0));
        lb_name->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PlayerBar: public Ui_PlayerBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYER_BAR_H
