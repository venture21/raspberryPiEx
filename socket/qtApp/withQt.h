#ifndef __CUSTOM_WIDGET__
#define __CUSTOM_WIDGET__

#include <QWidget>

class PiWidget : public QWidget { 
        Q_OBJECT
public:
    PiWidget(QWidget *parent = 0);

public slots:
    void ledOn( );
    void ledOff( ); 
   // void hc04Read( );
};

#endif // __CUSTOM_WIDGET__
