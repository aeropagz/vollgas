#include "MainWindow.hpp"
#include "../commons/data.h"

int main(int argc, char *argv[]) {
    QApplication a (argc, argv);
    MainWindow *window = new MainWindow();
    window->show();
    int i =  a.exec(); //TODO: Ende abfangen
    printf("ENDE");
    return i;
}