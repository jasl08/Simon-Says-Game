#ifndef SIMONGAME_H
#define SIMONGAME_H

#include "mainwindow.h"
#include <QObject>
#include <stdlib.h>
#include <time.h>

class MainWindow;

/**
 * @class SimonGame
 * @brief The main game logic controller for the Simon Game.
 *
 * This class manages the game's logic, sequence generation, player input verification, and
 * user interface updates through signals and slots mechanism.
 */
class SimonGame : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new SimonGame instance.
     * @param mainWindow A pointer to the main window object for UI updates.
     */
    SimonGame(MainWindow*);

public slots:
    /**
     * @brief Starts a new game by resetting relevant variables and generating a new sequence.
     */
    void onStartNewGame();

    /**
     * @brief Checks if the player's button press matches the expected sequence.
     * @param buttonID The ID or index of the button clicked by the player.
     */
    void checkPlayerButtonClicked(int);

signals:
    /**
     * @brief Signal emitted to indicate a color should flash on the UI.
     * @param colorID The ID or index of the color to flash.
     */
    void detectedFlashingColor(int);

    /**
     * @brief Signal emitted to activate or enable buttons on the UI.
     */
    void activateButtons();

    /**
     * @brief Signal emitted when the player fails to match the sequence.
     */
    void playerLoses();

    /**
     * @brief Signal to update the player's progress on the UI.
     * @param matchedMoves The number of correctly matched moves by the player so far.
     * @param totalMoves The total number of moves in the current sequence.
     */
    void playerProgressUpdated(int matchedMoves, int totalMoves);

    /**
     * @brief Signal to indicate a progress bar should be displayed on the UI.
     */
    void showProgressBar();

    /**
     * @brief Changes the display of the current level.
     */
    void updateCurrentLevel(int);

private:
    MainWindow *mainWindow; ///< Pointer to the main window object for UI updates.

    std::vector<int> listOfMoves; ///< List of generated moves representing the sequence the player needs to match.

    std::vector<int> playerInputMoves; ///< List of moves entered by the player.

    int current_level; ///< Current game level, influencing the speed and complexity of the game.

    /**
     * @brief Randomly selects the next color in the sequence.
     */
    void selectNextColor();

    /**
     * @brief Handles the flashing of color buttons based on the current game level and sequence.
     */
    void flashColorButtons();

    bool gameOver; ///< Flag indicating if the current game round is over.

    bool playerInput; ///< Flag indicating if the game is waiting for the player's input.

    bool generatingMoves; /// <Flag indicating if the game is in the process of generating a sequence of moves.

};

#endif // SIMON_H
