#include "Control.hpp"


Control::Control(std::string title) {
    setMinimumHeight(400);
    setMinimumWidth(250);

	labelFont = new QFont();
	labelFont->setPointSize(17);

    slider = new QSlider(Qt::Vertical);
    slider->setValue(0);
	slider->setMaximum(255);
	slider->setStyleSheet(styleSheet().append(QString("QSlider::groove:vertical {background: red;} QSlider::add-page:vertical {background: red;} QSlider::sub-page:vertical {background: white;} QSlider::handle:vertical {height: 10px; width: 10px; background: black; margin: -2 -4px;}")));

    label = new QLabel(QString::fromStdString(title));
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
	QObject::connect(forward, SIGNAL(stateChanged(int)), this, SLOT(directionChanged(int)));
}

void Control::updateLabels() {
	if (Qt::Checked == forward->checkState()) {
	directionLabel->setText("Vorwärts/Rechts rum");
	} else {
		directionLabel->setText("Rückwärts/Links rum");
	}

}

void Control::onSliderChanged(int value) {
	printf("Silder changed to %d\n", value);
}

void Control::directionChanged(int state) {
	printf("Direction changed\n");
	update();
}

void Control::update() {
	updateLabels();
	this->repaint();
}

