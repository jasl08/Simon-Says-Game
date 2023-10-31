/*
------------------------------------------------------------------------------
Project: Simon Game
Team Members:
    - Alex Qi
    - Jason Lin

GitHub Usernames:
    - Qibola
    - jasl08

Creative Element:
This version of the classic Simon Game lights up green whenever a player finishes a level, and lights up red when they inevitably make a mistake and lose.
The main creative element is that the buttons make a unique sound whenever they light up and are clicked on.
------------------------------------------------------------------------------
*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
