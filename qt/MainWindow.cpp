#include "MainWindow.hpp"
#include <QCheckBox>
#include <QString>

MainWindow::MainWindow()
{

	wdg = new QWidget(this);//Hauptwidget
	mainRow = new QHBoxLayout();
	mainColumn = new QVBoxLayout();
	leftSide = new QVBoxLayout();
	rightSide = new QVBoxLayout();

	initCommon();
	initDirection();
	
	
	sliderLeft = new QSlider(Qt::Vertical);
	sliderLeft->setValue(20);
	sliderLeft->setMaximum(255);
	sliderRight = new QSlider(Qt::Vertical);
	sliderRight->setMaximum(255);
	sliderRight->setValue(20);
	
	leftSide->addWidget(sliderLeft);//den Slider hinzufügen
	rightSide->addWidget(sliderRight);//den Slider hinzufügen
	mainColumn->addWidget(new QLabel("Test Start Button"));//die Schaltfläche hinzufügen
	leftSide->addLayout(directionLayout);
	rightSide->addLayout(directionLayout);
	mainRow->addLayout(leftSide);
	mainRow->addLayout(rightSide);
	mainColumn->addLayout(mainRow);
		
	wdg->setLayout(mainColumn);//Das Layout dem zentralen Widget zuweisen
	this->setCentralWidget(wdg);//Das zentrale Widget dem Fenster zuweisen
	// QObject::connect(btnStart, SIGNAL (clicked()), this, SLOT(OnStartClicked()));//Das Sigal Click mit der Methode OnClick (siehe unten) verbinden
	QObject::connect(sliderLeft, SIGNAL(valueChanged(int)), this, SLOT(onSliderChanged(int)));
	QObject::connect(sliderRight, SIGNAL(valueChanged(int)), this, SLOT(onSliderChanged(int)));
	QObject::connect(forward, SIGNAL(stateChanged(int)), this, SLOT(directionChanged(int)));
	
	
	/*ToDO: Weitere Callbacks verbinden*/
	//Das Signal für einen geänderten Sliderwert ist valueChanged(int)
}

void MainWindow::initCommon() {
	printf("Init common\n");
	headlineFont = new QFont();
	headlineFont->setPointSize(28);
	headlineFont->setBold(true);
	bodyFont = new QFont();
	bodyFont->setPointSize(20);
	printf("End init common\n");
}

void MainWindow::initDirection() {
	printf("Init Dir\n");
	directionLayout = new QHBoxLayout();
	directionHeadline = new QLabel("Richtungssteuerung:");
	directionHeadline->setFont(*headlineFont);

	directionLabel = new QLabel();
	directionLabel->setFont(*bodyFont);
	forward = new QCheckBox();
	forward->setStyleSheet("QCheckBox::indicator { width:50px; height: 50px;}");
	updateLabels();
	directionLayout->addWidget(directionHeadline);
	directionLayout->addWidget(forward);
	directionLayout->addWidget(directionLabel);
	printf("End init dir\n");
}

void MainWindow::update() {
	updateLabels();
	this->repaint();
}

 void MainWindow::OnStartClicked()
{
	printf("Measurement started\n");
}

void MainWindow::OnEndClicked() {
	printf("Measurement paused\n");
}

void MainWindow::onSliderChanged(int value) {
	printf("Silder changed to %d\n", value);
}

void MainWindow::updateLabels() {
	if (Qt::Checked == forward->checkState()) {
	directionLabel->setText("Vorwärts/Rechts rum");
	} else {
		directionLabel->setText("Rückwärts/Links rum");
	}

}

void MainWindow::directionChanged(int state) {
	printf("Direction changed\n");
	update();
}
/*Callbacks ausprogrammieren*/    
