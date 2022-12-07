#include "MainWindow.hpp"

void sendToDriver(MotorData);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *window = new MainWindow();
    window->show();
    int i = a.exec(); // TODO: Ende abfangen

    for (int j = 1; j <= 2; j++)
    {
        MotorData data;
        data.id = j;
        data.speed = 0;
        data.direction = 1;
        sendToDriver(data);
    }

    return i;
}