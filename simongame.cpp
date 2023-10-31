#include "simongame.h"
#include "mainwindow.h"
#include <QTimer>
#include <QRandomGenerator>

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
    emit showProgressBar();
    selectNextColor();
}

void SimonGame::selectNextColor()
{
    int selectColor = QRandomGenerator::global()->bounded(2);
    listOfMoves.push_back(selectColor);
    flashColorButtons();
}

void SimonGame::flashColorButtons()
{
    int baseDelay = 800; // Base delay for the speed-up effect.
    int adjustedDelay = baseDelay - ((current_level - 1) * 35); // Reduce delay by 25ms for every level.

    if (adjustedDelay < 325) adjustedDelay = 325; // Minimum delay of 200ms.

    if (generatingMoves)
    {
        int totalDelay = 0; // This will keep track of the cumulative delay for each color.

        for (int color : listOfMoves)
        {
            QTimer::singleShot(totalDelay, this, [this, color] {
                emit detectedFlashingColor(color);
            });
            totalDelay += adjustedDelay;
        }

        QTimer::singleShot(totalDelay, this, [this] {
            emit activateButtons();
        });

        generatingMoves = false;
        playerInput = true;
        current_level += 1;
    }
}

void SimonGame::checkPlayerButtonClicked(int color)
{
    bool gameOver = false;

    // After recording the player's move:
    playerInputMoves.push_back(color);

    // Emit the signal for updating the progress bar right after:
    emit playerProgressUpdated(playerInputMoves.size(), listOfMoves.size());

    int movesGenerated = 0;

    for (int buttonsClicked : playerInputMoves)
    {
        if (buttonsClicked != listOfMoves[movesGenerated])
        {
            listOfMoves.clear();
            playerInputMoves.clear();
            gameOver = true;
            break;
        }

        movesGenerated += 1;
    }

    if (gameOver)
    {
        emit playerLoses();
    }
    else if (playerInputMoves.size() == listOfMoves.size())
    {
        // Pause for 1 second to let the player see the progress bar at 100%.
        QTimer::singleShot(1000, this, [this]() {
            emit playerProgressUpdated(0, 1);  // Reset progress bar after the delay.
            QTimer::singleShot(500, this, SLOT(onStartNewGame()));  // Pause for an additional 0.5 seconds before starting the computer's turn.
        });
    }
}
