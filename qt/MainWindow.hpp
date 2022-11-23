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

class MainWindow : public QMainWindow{
    Q_OBJECT
public:

	QFont *headlineFont;
	QFont *bodyFont;
	QWidget * wdg;

	QPushButton *btnStart;
	QSlider *sliderLeft;
	QSlider *sliderRight;

	QVBoxLayout *leftSide;
	QVBoxLayout *rightSide;
	QHBoxLayout *mainRow;
	QVBoxLayout *mainColumn;

	QLabel *directionHeadline;
	QCheckBox *forward;
	QLabel *directionLabel;
	QHBoxLayout *directionLayout;

	MainWindow();
public slots:
	void initCommon();
	void initDirection();
	void OnStartClicked();
	void OnEndClicked();
	void onSliderChanged(int);
	void update();
	void updateLabels();
	void directionChanged(int);
private:
    
};
