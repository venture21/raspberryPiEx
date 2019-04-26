#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>

class CustomWidget : public QWidget {
    Q_OBJECT
  public:
    CustomWidget(QWidget *parent = 0);

  signals:
    void widgetClicked();

  public slots:
    void processClick();
};

#endif // CUSTOMWIDGET_H
