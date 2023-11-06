#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>

/**
 * @brief Constructor for the MainWindow class.
 * @param parent The parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Set up the UI components from the UI file.
    ui->setupUi(this);

    // Initialize the progress bar.
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);

    // Disable the color buttons initially.
    setDisableButtons();

    // Create a new SimonGame instance and connect signals/slots.
    simongame = new SimonGame(this);
    QObject::connect(this, SIGNAL(startNewGame()), simongame, SLOT(onStartNewGame()));
    QObject::connect(this, SIGNAL(playerClickedButton(int)), simongame, SLOT(checkPlayerButtonClicked(int)));
    QObject::connect(simongame, SIGNAL(playerProgressUpdated(int, int)), this, SLOT(updatePlayerProgressBar(int, int)));
    QObject::connect(simongame, SIGNAL(showProgressBar()), this, SLOT(displayProgressBar()));
}

/**
 * @brief Destructor for the MainWindow class.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Triggered when the start button is clicked. Resets the game UI and initiates a new round.
 */
void MainWindow::on_startButton_clicked()
{
    ui->GameOverMessage->setText("");
    ui->buttonBorder->setStyleSheet("background-color: white;");
    ui->startButton->setDisabled(true);
    emit startNewGame();
}

/**
 * @brief Enables the color buttons for player input.
 */
void MainWindow::setEnableButtons()
{
    ui->redButton->setEnabled(true);
    ui->blueButton->setEnabled(true);
    ui->yellowButton->setEnabled(true);
    ui->greenButton->setEnabled(true);
}

/**
 * @brief Disables the color buttons to prevent player input.
 */
void MainWindow::setDisableButtons()
{
    ui->redButton->setDisabled(true);
    ui->blueButton->setDisabled(true);
    ui->greenButton->setDisabled(true);
    ui->yellowButton->setDisabled(true);
}

/**
 * @brief Initiates the flashing of the specified color.
 * @param color The ID of the color to flash (e.g., 0 for red).
 */
void MainWindow::displayFlashingColor(int color)
{
    currentColor = color;
    setDisableButtons();
    QTimer::singleShot(125, this, SLOT(flashColorButton()));
}

/**
 * @brief Plays the sound and visual flash for the current color.
 */
void MainWindow::flashColorButton()
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);

    // Play the corresponding sound and change the color based on the currentColor value.
    // 0 represents red and any other value represents blue.
    if (currentColor == 0)
    {
        ui->greenButton->setStyleSheet("background-color:rgb(0, 255, 0)");
    }

    else if (currentColor == 1)
    {
        ui->yellowButton->setStyleSheet("background-color:rgb(255, 255, 0)");
    }

    else if (currentColor == 2)
    {
        player->setSource(QUrl("qrc:/sounds/redSound.mp3"));
        audioOutput->setVolume(50);
        player->play();
        ui->redButton->setStyleSheet("background-color:rgb(255, 0, 0)");
    }
    else
    {
        player->setSource(QUrl("qrc:/sounds/blueSound.mp3"));
        audioOutput->setVolume(50);
        player->play();
        ui->blueButton->setStyleSheet("background-color:rgb(0, 0, 255)");
    }

    QTimer::singleShot(125, this, SLOT(unflashColorButton()));
}

/**
 * @brief Reverts the flashed color back to its default state.
 */
void MainWindow::unflashColorButton()
{
    // Based on the currentColor, revert the color button's style.
    if (currentColor == 0)
    {
        ui->greenButton->setStyleSheet( QString("QPushButton {background-color: rgb(0,113,0);} QPushButton:pressed {background-color: rgb(0,255,0);}"));
    }

    else if (currentColor == 1)
    {
        ui->yellowButton->setStyleSheet( QString("QPushButton {background-color: rgb(113,113,0);} QPushButton:pressed {background-color: rgb(255,255,0);}"));
    }

    else if (currentColor == 2)
    {
        ui->redButton->setStyleSheet( QString("QPushButton {background-color: rgb(113,0,0);} QPushButton:pressed {background-color: rgb(255,0,0);}"));
    }

    else
    {
        ui->blueButton->setStyleSheet( QString("QPushButton {background-color: rgb(0,0,113);} QPushButton:pressed {background-color: rgb(0,0,255);}"));
    }
}

/**
 * @brief Triggered when the red button is clicked. Plays the red sound and emits a signal to the game logic.
 */
void MainWindow::on_redButton_clicked()
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);

    player->setSource(QUrl("qrc:/sounds/redSound.mp3"));
    audioOutput->setVolume(50);
    player->play();
    emit playerClickedButton(2);
}

/**
 * @brief Triggered when the blue button is clicked. Plays the blue sound and emits a signal to the game logic.
 */
void MainWindow::on_blueButton_clicked()
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);


    player->setSource(QUrl("qrc:/sounds/blueSound.mp3"));
    audioOutput->setVolume(50);
    player->play();
    emit playerClickedButton(3);

}

void MainWindow::on_yellowButton_clicked()
{
    emit playerClickedButton(1);
}

void MainWindow::on_greenButton_clicked()
{
    emit playerClickedButton(0);
}
/**
 * @brief Updates the player's progress bar based on matched moves and total moves in the sequence.
 * @param matchedMoves Number of correct moves made by the player.
 * @param totalMoves Total moves in the current sequence.
 */
void MainWindow::updatePlayerProgressBar(int matchedMoves, int totalMoves)
{
    int progress = (matchedMoves * 100) / totalMoves; // Calculate percentage progress.

    if (progress > 99)
    {
        ui->buttonBorder->setStyleSheet("background-color: lightgreen;");
        setDisableButtons();
    }

    else
    {
        ui->buttonBorder->setStyleSheet("background-color: white;");
    }

    ui->progressBar->setValue(progress);
}

/**
 * @brief Displays the progress bar and resets it to the beginning.
 */
void MainWindow::displayProgressBar()
{
    ui->progressBar->setValue(0);    // Reset the progress bar to 0%.
    ui->progressBar->show();         // Show the progress bar.
}

void MainWindow::displayCurrentLevel(int currentLevel)
{

    QString level = QString::number(currentLevel);
    ui->startButton->setText(level);
}
/**
 * @brief Handles the game-over scenario by updating the UI and allowing for a new game to start.
 */
void MainWindow::gameOver()
{
    setDisableButtons();
    updatePlayerProgressBar(0,1);
    ui->startButton->setEnabled(true);
    ui->buttonBorder->setStyleSheet("background-color: pink;");
    ui->GameOverMessage->setText("You Lost!");
    ui->startButton->setText("Start");
}





