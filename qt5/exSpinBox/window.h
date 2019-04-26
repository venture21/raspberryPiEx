#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

//class QDateTimeEdit;
class QSpinBox;
//class QDoubleSpinBox;
class QGroupBox;
class QLabel;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private:
    void createSpinBoxes();
    
    QGroupBox *spinBoxesGroup;
    QSpinBox *groupSeparatorSpinBox;

};

#endif // WINDOW_H
