/********************************************************************************
** Form generated from reading UI file 'target_chess_to_promote.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TARGET_CHESS_TO_PROMOTE_H
#define UI_TARGET_CHESS_TO_PROMOTE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TargetChessToPromateDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *fr_main;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *chess_rook;
    QPushButton *chess_knight;
    QPushButton *chess_bishop;
    QPushButton *chess_queen;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *TargetChessToPromateDialog)
    {
        if (TargetChessToPromateDialog->objectName().isEmpty())
            TargetChessToPromateDialog->setObjectName(QStringLiteral("TargetChessToPromateDialog"));
        TargetChessToPromateDialog->resize(452, 198);
        verticalLayout = new QVBoxLayout(TargetChessToPromateDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        fr_main = new QFrame(TargetChessToPromateDialog);
        fr_main->setObjectName(QStringLiteral("fr_main"));
        fr_main->setFrameShape(QFrame::StyledPanel);
        fr_main->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(fr_main);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(31);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        chess_rook = new QPushButton(fr_main);
        chess_rook->setObjectName(QStringLiteral("chess_rook"));

        horizontalLayout->addWidget(chess_rook);

        chess_knight = new QPushButton(fr_main);
        chess_knight->setObjectName(QStringLiteral("chess_knight"));

        horizontalLayout->addWidget(chess_knight);

        chess_bishop = new QPushButton(fr_main);
        chess_bishop->setObjectName(QStringLiteral("chess_bishop"));

        horizontalLayout->addWidget(chess_bishop);

        chess_queen = new QPushButton(fr_main);
        chess_queen->setObjectName(QStringLiteral("chess_queen"));

        horizontalLayout->addWidget(chess_queen);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(fr_main);


        retranslateUi(TargetChessToPromateDialog);

        QMetaObject::connectSlotsByName(TargetChessToPromateDialog);
    } // setupUi

    void retranslateUi(QDialog *TargetChessToPromateDialog)
    {
        TargetChessToPromateDialog->setWindowTitle(QApplication::translate("TargetChessToPromateDialog", "Dialog", 0));
        chess_rook->setText(QString());
        chess_knight->setText(QString());
        chess_bishop->setText(QString());
        chess_queen->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TargetChessToPromateDialog: public Ui_TargetChessToPromateDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TARGET_CHESS_TO_PROMOTE_H
