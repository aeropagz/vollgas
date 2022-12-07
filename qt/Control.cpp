#include <stdio.h>
#include "Control.hpp"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
void sendToDriver(MotorData motorData);
#define WR_VALUE _IOW('a','a',MotorData*)
Control::Control(std::string title, int id) {
	motorData.id = id;
	motorData.speed = 0;
	motorData.direction = 0;
    setMinimumHeight(400);
    setMinimumWidth(250);

	labelFont = new QFont();
	labelFont->setPointSize(17);

    slider = new QSlider(Qt::Vertical);
    slider->setValue(0);
	slider->setMaximum(255);
	slider->setStyleSheet(styleSheet().append(QString("QSlider::groove:vertical {background: red;} QSlider::add-page:vertical {background: red;} QSlider::sub-page:vertical {background: white;} QSlider::handle:vertical {height: 10px; width: 10px; background: black; margin: -2 -4px;}")));

    label = new QLabel(QString::fromStdString(title).append(" (ID: ").append(QString::number(id)).append(")"));
	label->setFont(*labelFont);

    forward = new QCheckBox();
	forward->setStyleSheet("QCheckBox::indicator { width:30px; height: 30px;}");
	
	directionLabel = new QLabel();

    directionLayout = new QHBoxLayout();

	updateLabels();

	directionLayout->addWidget(forward, Qt::AlignLeft);
	directionLayout->addWidget(directionLabel, Qt::AlignLeft);

    column = new QVBoxLayout(this);
    column->addWidget(label, Qt::AlignCenter);
    column->addWidget(slider, Qt::AlignCenter);
    column->addLayout(directionLayout, Qt::AlignRight);

	QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderChanged(int)));
	QObject::connect(slider, SIGNAL(sliderReleased()), this, SLOT(onSliderReleased()));
	QObject::connect(forward, SIGNAL(stateChanged(int)), this, SLOT(directionChanged(int)));
}

void Control::updateLabels() {
	if (Qt::Checked == forward->checkState()) {
	directionLabel->setText("Vorwärts/Rechts rum");
	} else {
		directionLabel->setText("Rückwärts/Links rum");
	}

}

void Control::onSliderReleased() {
	printf("Silder released. ID: %d Speed: %d, Direction: %d\n", motorData.id, motorData.speed, motorData.direction);
	sendToDriver(motorData);
}

void Control::onSliderChanged(int value) {
	motorData.speed = value;
}

void Control::directionChanged(int state) {
	printf("Direction changed\n");
	motorData.direction = state;
	slider->setValue(0);
	sendToDriver(motorData);
	update();
}

void sendToDriver(MotorData motorData) {

int f = open( "/dev/VOLLGAS", O_RDWR);
	 if(f < 0) {
                printf("Cannot open device file...\n");
                return;
        }
 
	printf("WR_VALUE %x\n",WR_VALUE);     
    ioctl(f, WR_VALUE, (MotorData*) &motorData); 
  
	close(f);

}

void Control::update() {
	updateLabels();
	this->repaint();
}

