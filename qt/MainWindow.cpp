#include "MainWindow.hpp"
#include <QCheckBox>
#include <QString>

MainWindow::MainWindow()
{

	wdg = new QWidget(this);//Hauptwidget
	this->setFixedSize(500, 575); //(width, height)
	mainRow = new QHBoxLayout();
	mainColumn = new QVBoxLayout();
	leftSide = new QVBoxLayout();
	rightSide = new QVBoxLayout();
	leftMotor = new Control("Motor links", 1);
	rightMotor = new Control("Motor rechts", 2);

	initCommon();
	
	
	
	leftSide->addWidget(leftMotor);
	rightSide->addWidget(rightMotor);
	QLabel *headline = new QLabel("Motorsteuerung\nHardwarenahe Programmierung\nBK110");
	headline->setFont(*headlineFont);
	QLabel *subtitle = new QLabel("Klaas Pelzer & Nico Petersen");
	subtitle->setFont(*subtitleFont);
	mainColumn->addWidget(headline, Qt::AlignCenter);
	mainColumn->addWidget(subtitle, Qt::AlignCenter);
	mainRow->addLayout(leftSide, Qt::AlignLeft);
	mainRow->addLayout(rightSide, Qt::AlignRight);
	mainColumn->addLayout(mainRow);
		
	wdg->setLayout(mainColumn);//Das Layout dem zentralen Widget zuweisen
	this->setCentralWidget(wdg);//Das zentrale Widget dem Fenster zuweisen
	// QObject::connect(btnStart, SIGNAL (clicked()), this, SLOT(OnStartClicked()));//Das Sigal Click mit der Methode OnClick (siehe unten) verbinden
	// QObject::connect(sliderLeft, SIGNAL(valueChanged(int)), this, SLOT(onSliderChanged(int)));
	
	
	
	/*ToDO: Weitere Callbacks verbinden*/
	//Das Signal für einen geänderten Sliderwert ist valueChanged(int)
}

void MainWindow::initCommon() {
	printf("Init common\n");
	headlineFont = new QFont();
	headlineFont->setPointSize(20);
	headlineFont->setBold(true);
	subtitleFont = new QFont();
	subtitleFont->setPointSize(19);
	printf("End init common\n");
}



/*Callbacks ausprogrammieren*/    
