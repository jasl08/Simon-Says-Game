#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simongame.h"
#include <QMediaPlayer>
#include <QAudioOutput>

class SimonGame;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Represents the main window and user interface for the Simon Game.
 *
 * This class manages the interactions between the game logic (SimonGame) and the user interface,
 * including button flashes, audio playback, and progress tracking.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new MainWindow instance.
     * @param parent The parent widget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for the MainWindow.
     */
    ~MainWindow();

public slots:
    /**
     * @brief Displays the specified flashing color on the UI.
     * @param colorID The ID or index of the color to flash.
     */
    void displayFlashingColor(int);

    /**
     * @brief Displays a game over message and resets the UI to its initial state.
     */
    void gameOver();

    /**
     * @brief Enables the color buttons, allowing player input.
     */
    void setEnableButtons();

private slots:
    /**
     * @brief Triggered when the start button is clicked; begins a new game.
     */
    void on_startButton_clicked();

    /**
     * @brief Triggers a visual flash of a color button.
     */
    void flashColorButton();

    /**
     * @brief Reverts a color button to its default state after flashing.
     */
    void unflashColorButton();

    /**
     * @brief Triggered when the red color button is clicked.
     */
    void on_redButton_clicked();

    /**
     * @brief Triggered when the blue color button is clicked.
     */
    void on_blueButton_clicked();

    /**
     * @brief Updates the player's progress bar on the UI.
     * @param matchedMoves The number of correctly matched moves by the player so far.
     * @param totalMoves The total number of moves in the current sequence.
     */
    void updatePlayerProgressBar(int matchedMoves, int totalMoves);

    /**
     * @brief Displays a progress bar on the UI.
     */
    void displayProgressBar();

signals:
    /**
     * @brief Signal emitted to initiate a new game round.
     */
    void startNewGame();

    /**
     * @brief Signal emitted when a player clicks a color button.
     * @param buttonID The ID or index of the button clicked by the player.
     */
    void playerClickedButton(int);

private:
    Ui::MainWindow *ui; ///< Pointer to the UI components.
    SimonGame *simongame; ///< Pointer to the SimonGame logic controller.
    int currentColor; ///< The ID or index of the current color being flashed or interacted with.
    QMediaPlayer *player; ///< Audio player for sound effects.
    QAudioOutput *audioOutput; ///< Audio output for the game sounds.

    /**
     * @brief Disables the color buttons to prevent player input.
     */
    void setDisableButtons();
};

#endif // MAINWINDOW_H
