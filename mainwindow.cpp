#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>


///
/// \brief MainWindow::MainWindow
/// \param parent
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);

    setDisableButtons();

    simongame = new SimonGame(this);

    QObject::connect(this, SIGNAL(startNewGame()), simongame, SLOT(onStartNewGame()));
    QObject::connect(this, SIGNAL(playerClickedButton(int)), simongame, SLOT(checkPlayerButtonClicked(int)));
    QObject::connect(simongame, SIGNAL(playerProgressUpdated(int, int)), this, SLOT(updatePlayerProgressBar(int, int)));
    QObject::connect(simongame, SIGNAL(showProgressBar()), this, SLOT(displayProgressBar()));
}

///
/// \brief MainWindow::~MainWindow
///
MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief Ma!
/// thinWindow::on_startButton_clicked
///
void MainWindow::on_startButton_clicked()
{
    ui->GameOverMessage->setText("");
    ui->buttonBorder->setStyleSheet("background-color: white;");
    ui->startButton->setDisabled(true);
    emit startNewGame();
}

///
/// \brief MainWindow::setEnableButtons
///
void MainWindow::setEnableButtons()
{
    ui->redButton->setEnabled(true);
    ui->blueButton->setEnabled(true);
}

///
/// \brief MainWindow::setDisableButtons
///
void MainWindow::setDisableButtons()
{
    ui->redButton->setEnabled(false);
    ui->blueButton->setEnabled(false);
}

void MainWindow::displayFlashingColor(int color)
{
    currentColor = color;
    setDisableButtons();
    QTimer::singleShot(150, this, SLOT(flashColorButton()));
}

void MainWindow::flashColorButton()
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);

    if (currentColor == 0)
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

    QTimer::singleShot(150, this, SLOT(unflashColorButton()));
}

void MainWindow::unflashColorButton()
{
    if (currentColor == 0)
    {
        ui->redButton->setStyleSheet( QString("QPushButton {background-color: rgb(113,0,0);} QPushButton:pressed {background-color: rgb(255,0,0);}"));
    }

    else
    {

        ui->blueButton->setStyleSheet( QString("QPushButton {background-color: rgb(0,0,113);} QPushButton:pressed {background-color: rgb(0,0,255);}"));
    }
}


void MainWindow::on_redButton_clicked()
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);

    player->setSource(QUrl("qrc:/sounds/redSound.mp3"));
    audioOutput->setVolume(50);
    player->play();
    emit playerClickedButton(0);

}


void MainWindow::on_blueButton_clicked()
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);


    player->setSource(QUrl("qrc:/sounds/blueSound.mp3"));
    audioOutput->setVolume(50);
    player->play();
    emit playerClickedButton(1);

}

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

void MainWindow::displayProgressBar()
{
    ui->progressBar->setValue(0);    // Reset the progress bar to 0%.
    ui->progressBar->show();         // Show the progress bar.
}

void MainWindow::gameOver()
{
    setDisableButtons();
    updatePlayerProgressBar(0,1);
    ui->startButton->setEnabled(true);
    ui->buttonBorder->setStyleSheet("background-color: pink;");
    ui->GameOverMessage->setText("You Lost!");
}
