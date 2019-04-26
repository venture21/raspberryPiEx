#ifndef COMBOBOXDIALOG_H
#define COMBOBOXDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ComboBoxDialog;
}

class ComboBoxDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ComboBoxDialog(QWidget *parent = 0);
    ~ComboBoxDialog();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::ComboBoxDialog *ui;
};

#endif // COMBOBOXDIALOG_H
