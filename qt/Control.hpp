
#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <string>
#include <QLabel>
#include <QCheckBox>
#include <QString>

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

    Control(std::string title);

protected:

signals:
public slots:
void updateLabels();
void onSliderChanged(int);
void directionChanged(int state);
void update();
};