#include <QtWidgets>
#include "window.h"

Window::Window()
{
    createSpinBoxes();  
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(spinBoxesGroup);
    setLayout(layout);

    setWindowTitle(tr("Spin Boxes"));
}

void Window::createSpinBoxes()
{
    spinBoxesGroup = new QGroupBox(tr("Spinboxes"));
    // 1st SpinBox
    QLabel *integerLabel = new QLabel(tr("Enter a value between "
         "%1 and %2:").arg(-20).arg(20));
    QSpinBox *integerSpinBox = new QSpinBox;
    integerSpinBox->setRange(-20, 20);
    integerSpinBox->setSingleStep(1);
    integerSpinBox->setValue(0);

    // 2nd SpinBox
    QLabel *zoomLabel = new QLabel(tr("Enter a zoom value between "
        "%1 and %2:").arg(0).arg(1000));
    QSpinBox *zoomSpinBox = new QSpinBox;
    zoomSpinBox->setRange(0, 1000);
    zoomSpinBox->setSingleStep(10);
    zoomSpinBox->setSuffix("%");
    zoomSpinBox->setSpecialValueText(tr("Automatic"));
    zoomSpinBox->setValue(100);

    // 3rd SpinBox
    QLabel *priceLabel = new QLabel(tr("Enter a price between "
        "%1 and %2:").arg(0).arg(999));
    QSpinBox *priceSpinBox = new QSpinBox;
    priceSpinBox->setRange(0, 999);
    priceSpinBox->setSingleStep(1);
    priceSpinBox->setPrefix("$");
    priceSpinBox->setValue(99);

    groupSeparatorSpinBox = new QSpinBox;
    groupSeparatorSpinBox->setRange(-99999999, 99999999);
    groupSeparatorSpinBox->setValue(1000);
    groupSeparatorSpinBox->setGroupSeparatorShown(true);
    QCheckBox *groupSeparatorChkBox = new QCheckBox;
    groupSeparatorChkBox->setText(tr("Show group separator"));
    groupSeparatorChkBox->setChecked(true);
    connect(groupSeparatorChkBox, &QCheckBox::toggled, groupSeparatorSpinBox,
            &QSpinBox::setGroupSeparatorShown);


    // 4th SpinBox
    QLabel *hexLabel = new QLabel(tr("Enter a value between "
        "%1 and %2:").arg('-' + QString::number(31, 16)).arg(QString::number(31, 16)));
    QSpinBox *hexSpinBox = new QSpinBox;
    hexSpinBox->setRange(-31, 31);
    hexSpinBox->setSingleStep(1);
    hexSpinBox->setValue(0);
    hexSpinBox->setDisplayIntegerBase(16);

    QVBoxLayout *spinBoxLayout = new QVBoxLayout;
    spinBoxLayout->addWidget(integerLabel);
    spinBoxLayout->addWidget(integerSpinBox);
    spinBoxLayout->addWidget(zoomLabel);
    spinBoxLayout->addWidget(zoomSpinBox);
    spinBoxLayout->addWidget(priceLabel);
    spinBoxLayout->addWidget(priceSpinBox);
    spinBoxLayout->addWidget(hexLabel);
    spinBoxLayout->addWidget(hexSpinBox);
    spinBoxLayout->addWidget(groupSeparatorChkBox);
    spinBoxLayout->addWidget(groupSeparatorSpinBox);
    spinBoxesGroup->setLayout(spinBoxLayout);
}

