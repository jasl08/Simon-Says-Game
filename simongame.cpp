#include "simongame.h"
#include "mainwindow.h"
#include <QTimer>
#include <QRandomGenerator>

// Constructor for SimonGame.
// Sets up the game and initializes the connections between signals and slots.
SimonGame::SimonGame(MainWindow* _mainWindow)
{
    mainWindow = _mainWindow;
    current_level = 1;
    playerInput = false;
    generatingMoves = true;

    // Connects game signals to mainWindow slots.
    QObject::connect(this, SIGNAL(detectedFlashingColor(int)), mainWindow, SLOT(displayFlashingColor(int)));
    QObject::connect(this, SIGNAL(activateButtons()), mainWindow, SLOT(setEnableButtons()));
    QObject::connect(this, SIGNAL(playerLoses()), mainWindow, SLOT(gameOver()));
    QObject::connect(this, SIGNAL(updateCurrentLevel(int)), mainWindow, SLOT(displayCurrentLevel(int)));
}

// Begins a new game by clearing player input and setting the conditions to generate moves.
void SimonGame::onStartNewGame()
{
    playerInput = false;
    generatingMoves = true;
    playerInputMoves.clear();
    emit showProgressBar();
    selectNextColor();
}

// Function to select the next color for the game sequence
void SimonGame::selectNextColor()
{
    int selectColor = QRandomGenerator::global()->bounded(4); // Randomly select between four colors (0, 1, 2, 3)
    listOfMoves.push_back(selectColor);
    flashColorButtons(); // Flash the sequence of colors
}

// Function to flash the color buttons in the game sequence
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

        // Activate player input after flashing the sequence
        QTimer::singleShot(totalDelay, this, [this] {
            emit activateButtons();
        });

        generatingMoves = false;
        playerInput = true;
        emit updateCurrentLevel(current_level);
        current_level += 1;
    }
}

// Function to check the player's button input against the game sequence
void SimonGame::checkPlayerButtonClicked(int color)
{
    gameOver = false;

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

    // Handle game over or move to next level if player's input is correct
    if (gameOver)
    {
        current_level = 1;
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
