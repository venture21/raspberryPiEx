#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include "withQt.h"
#include "raspi.h"

void PiWidget::ledOn() /* LED�� �Ҵ�. */
{
	ledControl(1);
}

void PiWidget::ledOff() /* LED�� ����. */
{
	ledControl(0);
}

PiWidget::PiWidget(QWidget* parent) : QWidget(parent)
{
	QLabel* labelLED = new QLabel("LED", this);

	/* ���� ��ư ������ ��ġ�ϰ� �������� ���̾ƿ�(QHBoxLayout) ���� */
	QRadioButton* radioButtonOn = new QRadioButton("O&n", this);
	QRadioButton* radioButtonOff = new QRadioButton("O&ff", this);
	QHBoxLayout* horizontalLayout = new QHBoxLayout();
	horizontalLayout->addWidget(radioButtonOn);
	horizontalLayout->addWidget(radioButtonOff);

	/* ���̺�� Ǫ�� ��ư�� ���� */
	//QLabel* labelMusic = new QLabel("Music", this);
	//QPushButton* pushButtonPlay = new QPushButton("&Play", this);
	//QLabel* labelMotor = new QLabel("Motor", this);
	//QPushButton* pushButtonMotor = new QPushButton("&On", this);

	/* �� ������� QHBoxLayout Ŭ������ QGridLayout���� �����̿� */
	QGridLayout* gridLayout = new QGridLayout(this);
	gridLayout->addWidget(labelLED, 0, 0, 1, 1);
	gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);
	//gridLayout->addWidget(labelMusic, 1, 0, 1, 1);
	//gridLayout->addWidget(pushButtonPlay, 1, 2, 1, 1);
	//gridLayout->addWidget(labelMotor, 2, 0, 1, 1);
	//gridLayout->addWidget(pushButtonMotor, 2, 2, 1, 1);

	/* �� ������� ����� ���� ������ ���� */
	connect(radioButtonOn, SIGNAL(clicked()), SLOT(ledOn()));
	connect(radioButtonOff, SIGNAL(clicked()), SLOT(ledOff()));
	//connect(pushButtonPlay, SIGNAL(clicked()), SLOT(musicPlay()));
	//connect(pushButtonMotor, SIGNAL(clicked()), SLOT(motorOn()));

	/* ���� �� ���̾ƿ� �� QGridLayout�� ���� ���̾ƿ����� ���� */
	setLayout(gridLayout);
}