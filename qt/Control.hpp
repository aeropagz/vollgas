
#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <string>
#include <QLabel>
#include <QCheckBox>
#include <QString>
#include "../commons/data.h"

class Control : public QWidget
{

    Q_OBJECT
public:
    
    QSlider *slider;
    QLabel *label;
    QFont *labelFont;

    QCheckBox *forward;
    QLabel *directionLabel;
    QHBoxLayout *directionLayout;

    QVBoxLayout *column;

    MotorData motorData;

    Control(std::string title, int id);

protected:

signals:
public slots:
void updateLabels();
void onSliderReleased();
void onSliderChanged(int value);
void directionChanged(int state);
void update();
void sendToDriver();
};