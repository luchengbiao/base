/********************************************************************************
** Form generated from reading UI file 'create_new_board.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATE_NEW_BOARD_H
#define UI_CREATE_NEW_BOARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "game/view/combo_box/combo_box.h"

QT_BEGIN_NAMESPACE

class Ui_CreateNewBoardDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *main_layout;
    QFrame *fr_title;
    QHBoxLayout *horizontalLayout;
    QLabel *lb_title;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_close;
    QFrame *fr_contents;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *lb_chess_name;
    QLineEdit *lineEdit;
    QLabel *lb_chess_mode;
    game::UIComboBox *com_mode;
    QLabel *lb_chess_status;
    game::UIComboBox *com_status;
    QLabel *lb_grids;
    game::UIComboBox *com_grids;
    QLabel *lb_first_operator;
    game::UIComboBox *com_first_operator;
    QLabel *lb_black_operator;
    game::UIComboBox *com_black_operator;
    QLabel *lb_white_operator;
    game::UIComboBox *com_white_operator;
    QFrame *fr_button;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_cancel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_new;

    void setupUi(QWidget *CreateNewBoardDialog)
    {
        if (CreateNewBoardDialog->objectName().isEmpty())
            CreateNewBoardDialog->setObjectName(QStringLiteral("CreateNewBoardDialog"));
        CreateNewBoardDialog->resize(579, 585);
        verticalLayout_2 = new QVBoxLayout(CreateNewBoardDialog);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        main_layout = new QVBoxLayout();
        main_layout->setSpacing(0);
        main_layout->setObjectName(QStringLiteral("main_layout"));
        main_layout->setContentsMargins(-1, -1, -1, 0);
        fr_title = new QFrame(CreateNewBoardDialog);
        fr_title->setObjectName(QStringLiteral("fr_title"));
        horizontalLayout = new QHBoxLayout(fr_title);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(24, 0, 12, 0);
        lb_title = new QLabel(fr_title);
        lb_title->setObjectName(QStringLiteral("lb_title"));

        horizontalLayout->addWidget(lb_title);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_close = new QPushButton(fr_title);
        btn_close->setObjectName(QStringLiteral("btn_close"));

        horizontalLayout->addWidget(btn_close);


        main_layout->addWidget(fr_title);

        fr_contents = new QFrame(CreateNewBoardDialog);
        fr_contents->setObjectName(QStringLiteral("fr_contents"));
        verticalLayout = new QVBoxLayout(fr_contents);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setHorizontalSpacing(12);
        formLayout->setVerticalSpacing(0);
        formLayout->setContentsMargins(48, 30, 48, -1);
        lb_chess_name = new QLabel(fr_contents);
        lb_chess_name->setObjectName(QStringLiteral("lb_chess_name"));

        formLayout->setWidget(0, QFormLayout::LabelRole, lb_chess_name);

        lineEdit = new QLineEdit(fr_contents);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

        lb_chess_mode = new QLabel(fr_contents);
        lb_chess_mode->setObjectName(QStringLiteral("lb_chess_mode"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lb_chess_mode);

        com_mode = new game::UIComboBox(fr_contents);
        com_mode->setObjectName(QStringLiteral("com_mode"));

        formLayout->setWidget(1, QFormLayout::FieldRole, com_mode);

        lb_chess_status = new QLabel(fr_contents);
        lb_chess_status->setObjectName(QStringLiteral("lb_chess_status"));

        formLayout->setWidget(2, QFormLayout::LabelRole, lb_chess_status);

        com_status = new game::UIComboBox(fr_contents);
        com_status->setObjectName(QStringLiteral("com_status"));

        formLayout->setWidget(2, QFormLayout::FieldRole, com_status);

        lb_grids = new QLabel(fr_contents);
        lb_grids->setObjectName(QStringLiteral("lb_grids"));

        formLayout->setWidget(3, QFormLayout::LabelRole, lb_grids);

        com_grids = new game::UIComboBox(fr_contents);
        com_grids->setObjectName(QStringLiteral("com_grids"));

        formLayout->setWidget(3, QFormLayout::FieldRole, com_grids);

        lb_first_operator = new QLabel(fr_contents);
        lb_first_operator->setObjectName(QStringLiteral("lb_first_operator"));

        formLayout->setWidget(4, QFormLayout::LabelRole, lb_first_operator);

        com_first_operator = new game::UIComboBox(fr_contents);
        com_first_operator->setObjectName(QStringLiteral("com_first_operator"));

        formLayout->setWidget(4, QFormLayout::FieldRole, com_first_operator);

        lb_black_operator = new QLabel(fr_contents);
        lb_black_operator->setObjectName(QStringLiteral("lb_black_operator"));

        formLayout->setWidget(5, QFormLayout::LabelRole, lb_black_operator);

        com_black_operator = new game::UIComboBox(fr_contents);
        com_black_operator->setObjectName(QStringLiteral("com_black_operator"));

        formLayout->setWidget(5, QFormLayout::FieldRole, com_black_operator);

        lb_white_operator = new QLabel(fr_contents);
        lb_white_operator->setObjectName(QStringLiteral("lb_white_operator"));

        formLayout->setWidget(6, QFormLayout::LabelRole, lb_white_operator);

        com_white_operator = new game::UIComboBox(fr_contents);
        com_white_operator->setObjectName(QStringLiteral("com_white_operator"));

        formLayout->setWidget(6, QFormLayout::FieldRole, com_white_operator);


        verticalLayout->addLayout(formLayout);


        main_layout->addWidget(fr_contents);

        fr_button = new QFrame(CreateNewBoardDialog);
        fr_button->setObjectName(QStringLiteral("fr_button"));
        horizontalLayout_2 = new QHBoxLayout(fr_button);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(92, -1, 92, -1);
        btn_cancel = new QPushButton(fr_button);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));

        horizontalLayout_2->addWidget(btn_cancel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btn_new = new QPushButton(fr_button);
        btn_new->setObjectName(QStringLiteral("btn_new"));

        horizontalLayout_2->addWidget(btn_new);


        main_layout->addWidget(fr_button);


        verticalLayout_2->addLayout(main_layout);


        retranslateUi(CreateNewBoardDialog);

        QMetaObject::connectSlotsByName(CreateNewBoardDialog);
    } // setupUi

    void retranslateUi(QWidget *CreateNewBoardDialog)
    {
        CreateNewBoardDialog->setWindowTitle(QApplication::translate("CreateNewBoardDialog", "Form", 0));
        lb_title->setText(QApplication::translate("CreateNewBoardDialog", "\346\226\260\345\273\272\346\243\213\345\261\200", 0));
        btn_close->setText(QString());
        lb_chess_name->setText(QApplication::translate("CreateNewBoardDialog", "\346\243\213\345\261\200\345\220\215\347\247\260", 0));
        lb_chess_mode->setText(QApplication::translate("CreateNewBoardDialog", "\346\243\213\347\233\230\346\250\241\345\274\217", 0));
        lb_chess_status->setText(QApplication::translate("CreateNewBoardDialog", "\346\243\213\347\233\230\345\270\203\345\261\200", 0));
        lb_grids->setText(QApplication::translate("CreateNewBoardDialog", "\346\243\213\350\267\257\351\200\211\346\213\251", 0));
        lb_first_operator->setText(QApplication::translate("CreateNewBoardDialog", "\346\243\213\345\255\220\345\205\210\350\241\214", 0));
        com_first_operator->clear();
        com_first_operator->insertItems(0, QStringList()
         << QApplication::translate("CreateNewBoardDialog", "\351\273\221\346\226\271", 0)
         << QApplication::translate("CreateNewBoardDialog", "\347\231\275\346\226\271", 0)
        );
        lb_black_operator->setText(QApplication::translate("CreateNewBoardDialog", "\351\273\221\346\226\271", 0));
        lb_white_operator->setText(QApplication::translate("CreateNewBoardDialog", "\347\231\275\346\226\271", 0));
        btn_cancel->setText(QApplication::translate("CreateNewBoardDialog", "\345\217\226\346\266\210", 0));
        btn_new->setText(QApplication::translate("CreateNewBoardDialog", "\346\226\260\345\273\272", 0));
    } // retranslateUi

};

namespace Ui {
    class CreateNewBoardDialog: public Ui_CreateNewBoardDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATE_NEW_BOARD_H
