#include "simongame.h"
#include <QTimer>

SimonGame::SimonGame(MainWindow* _mainWindow)
{
    mainWindow = _mainWindow;
    current_level = 1;
    playerInput = false;
    generatingMoves = true;

    QObject::connect(this, SIGNAL(detectedFlashingColor(int)), mainWindow, SLOT(displayFlashingColor(int)));
    QObject::connect(this, SIGNAL(activateButtons()), mainWindow, SLOT(setEnableButtons()));

    QObject::connect(this, SIGNAL(playerLoses()), mainWindow, SLOT(gameOver()));
}

void SimonGame::onStartNewGame()
{
    playerInput = false;
    generatingMoves = true;
    playerInputMoves.clear();
    selectNextColor();
}

void SimonGame::selectNextColor()
{
    int selectColor = rand() % 2;
    listOfMoves.push_back(selectColor);
    qDebug() << listOfMoves;
    flashColorButtons();
}

void SimonGame::flashColorButtons()
{
    int delay = 500; // Initial delay
    if (generatingMoves)
    {
        for (int color : listOfMoves)
        {
            QTimer::singleShot(delay, this, [this, color] {
                emit detectedFlashingColor(color);
            });

            delay += 1250;
        }

        QTimer::singleShot(delay, this, [this] {
            emit activateButtons();
        });

        generatingMoves = false;
        playerInput = true;
    }

}

void SimonGame::checkPlayerButtonClicked(int color)
{
    bool gameOver = false;
    if (playerInput)
    {
        playerInputMoves.push_back(color);

        int movesGenerated = 0;
        qDebug() << "PlayerInputMoves" << playerInputMoves;
        qDebug() << "ListOfMoves" << listOfMoves;

        for (int buttonsClicked : playerInputMoves)
        {
            if (buttonsClicked != listOfMoves[movesGenerated])
            {
                qDebug() << "YOU LOSE!";
                listOfMoves.clear();
                playerInputMoves.clear();
                gameOver = true;
                break;
            }

            movesGenerated += 1;
        }
        qDebug() << movesGenerated;

        if (gameOver)
        {
            emit playerLoses();
        }

        else if (playerInputMoves.size() == listOfMoves.size())
        {
            onStartNewGame();
        }
    }
}



