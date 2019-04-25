#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include "withQt.h"
#include "raspi.h"

void PiWidget::ledOn() /* LED를 켠다. */
{
	ledControl(1);
}

void PiWidget::ledOff() /* LED를 끈다. */
{
	ledControl(0);
}

PiWidget::PiWidget(QWidget* parent) : QWidget(parent)
{
	QLabel* labelLED = new QLabel("LED", this);

	/* 라디오 버튼 위젯을 배치하고 수평으로 레이아웃(QHBoxLayout) 설정 */
	QRadioButton* radioButtonOn = new QRadioButton("O&n", this);
	QRadioButton* radioButtonOff = new QRadioButton("O&ff", this);
	QHBoxLayout* horizontalLayout = new QHBoxLayout();
	horizontalLayout->addWidget(radioButtonOn);
	horizontalLayout->addWidget(radioButtonOff);

	/* 레이블과 푸시 버튼을 생성 */
	//QLabel* labelMusic = new QLabel("Music", this);
	//QPushButton* pushButtonPlay = new QPushButton("&Play", this);
	//QLabel* labelMotor = new QLabel("Motor", this);
	//QPushButton* pushButtonMotor = new QPushButton("&On", this);

	/* 각 위젯들과 QHBoxLayout 클래스를 QGridLayout으로 레아이웃 */
	QGridLayout* gridLayout = new QGridLayout(this);
	gridLayout->addWidget(labelLED, 0, 0, 1, 1);
	gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);
	//gridLayout->addWidget(labelMusic, 1, 0, 1, 1);
	//gridLayout->addWidget(pushButtonPlay, 1, 2, 1, 1);
	//gridLayout->addWidget(labelMotor, 2, 0, 1, 1);
	//gridLayout->addWidget(pushButtonMotor, 2, 2, 1, 1);

	/* 각 위젯들과 사용자 정의 슬롯을 연결 */
	connect(radioButtonOn, SIGNAL(clicked()), SLOT(ledOn()));
	connect(radioButtonOff, SIGNAL(clicked()), SLOT(ledOff()));
	//connect(pushButtonPlay, SIGNAL(clicked()), SLOT(musicPlay()));
	//connect(pushButtonMotor, SIGNAL(clicked()), SLOT(motorOn()));

	/* 위의 두 레이아웃 중 QGridLayout을 폼의 레이아웃으로 설정 */
	setLayout(gridLayout);
}