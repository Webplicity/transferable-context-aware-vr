/********************************************************************************
** Form generated from reading UI file 'overlaywidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OVERLAYWIDGET_H
#define UI_OVERLAYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OverlayWidget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *spinBoxLabel;
    QSpinBox *spinBoxSpinBox;
    QLabel *checkBoxesLabel;
    QCheckBox *checkBoxesCheckBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *OverlayWidget)
    {
        if (OverlayWidget->objectName().isEmpty())
            OverlayWidget->setObjectName(QString::fromUtf8("OverlayWidget"));
        OverlayWidget->resize(191, 194);
        verticalLayoutWidget = new QWidget(OverlayWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 181, 111));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(16);
        font.setStyleStrategy(QFont::PreferAntialias);
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        formLayoutWidget = new QWidget(OverlayWidget);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(14, 110, 161, 51));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        spinBoxLabel = new QLabel(formLayoutWidget);
        spinBoxLabel->setObjectName(QString::fromUtf8("spinBoxLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, spinBoxLabel);

        spinBoxSpinBox = new QSpinBox(formLayoutWidget);
        spinBoxSpinBox->setObjectName(QString::fromUtf8("spinBoxSpinBox"));
        spinBoxSpinBox->setFrame(false);
        spinBoxSpinBox->setValue(50);

        formLayout->setWidget(0, QFormLayout::FieldRole, spinBoxSpinBox);

        checkBoxesLabel = new QLabel(formLayoutWidget);
        checkBoxesLabel->setObjectName(QString::fromUtf8("checkBoxesLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, checkBoxesLabel);

        checkBoxesCheckBox = new QCheckBox(formLayoutWidget);
        checkBoxesCheckBox->setObjectName(QString::fromUtf8("checkBoxesCheckBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, checkBoxesCheckBox);

        pushButton = new QPushButton(OverlayWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(110, 170, 75, 23));
        pushButton_2 = new QPushButton(OverlayWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(0, 170, 75, 23));

        retranslateUi(OverlayWidget);

        QMetaObject::connectSlotsByName(OverlayWidget);
    } // setupUi

    void retranslateUi(QWidget *OverlayWidget)
    {
        OverlayWidget->setWindowTitle(QApplication::translate("OverlayWidget", "Hello World Overlay", nullptr));
        label->setText(QApplication::translate("OverlayWidget", "Hello World! This is an OpenVR System Overlay.", nullptr));
        spinBoxLabel->setText(QApplication::translate("OverlayWidget", "Spin Box:", nullptr));
        checkBoxesLabel->setText(QApplication::translate("OverlayWidget", "Check Boxes:", nullptr));
        pushButton->setText(QApplication::translate("OverlayWidget", "Quit Overlay", nullptr));
        pushButton_2->setText(QApplication::translate("OverlayWidget", "Do Nothing", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OverlayWidget: public Ui_OverlayWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OVERLAYWIDGET_H
