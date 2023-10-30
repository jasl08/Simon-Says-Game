#ifndef SIMONGAME_H
#define SIMONGAME_H

#include "mainwindow.h"
#include <QObject>
#include <stdlib.h>
#include <time.h>

class MainWindow;

class SimonGame : public QObject
{
    Q_OBJECT

public:
    SimonGame(MainWindow*);

public slots:
    void onStartNewGame();
    void checkPlayerButtonClicked(int);

signals:
    void detectedFlashingColor(int);
    void activateButtons();
    void playerLoses();

private:
    MainWindow *mainWindow;
    std::vector<int> listOfMoves;
    std::vector<int> playerInputMoves;
    int current_level;
    void selectNextColor();
    void flashColorButtons();
    bool playerInput;
    bool generatingMoves;
};

#endif // SIMON_H
