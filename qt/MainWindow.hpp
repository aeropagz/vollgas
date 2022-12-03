#include <QApplication>
#include <QLabel>
#include <QPicture>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QSlider>
#include <QCheckBox>
#include <math.h>
#include <string>
#include <Control.hpp>

class MainWindow : public QMainWindow{
    Q_OBJECT
public:

	QFont *headlineFont;
	QFont *subtitleFont;
	QWidget * wdg;


	QVBoxLayout *leftSide;
	QVBoxLayout *rightSide;
	QHBoxLayout *mainRow;
	QVBoxLayout *mainColumn;
	

	Control *leftMotor;
	Control *rightMotor;


	MainWindow();
public slots:
	void initCommon();
private:
    
};
