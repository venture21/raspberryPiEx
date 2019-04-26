#include "comboboxdialog.h"
#include "ui_comboboxdialog.h"
#include <stdio.h>

ComboBoxDialog::ComboBoxDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComboBoxDialog)
{
    ui->setupUi(this);
    // ComboBox의 Item을 추가 (*.ui에서 추가할 수도 있다)
//    for(int i = 0; i < 10; i++)
//    {
//        ui->comboBox->addItem("item " + QString::number(i));
//    }
}

ComboBoxDialog::~ComboBoxDialog()
{
    delete ui;
}

// Message popup when the button is clicked
void ComboBoxDialog::on_pushButton_clicked()
{
    QMessageBox::information(this, "Item Selection",
                             ui->comboBox->currentText());
}

void ComboBoxDialog::on_comboBox_currentIndexChanged(int index)
{
      char str[20];
//    int index;
//    QString str;
//    index = ui->comboBox->currentIndex();
    sprintf(str,"Index %d",index+1);
    ui->labelResult->setText(str);
}
