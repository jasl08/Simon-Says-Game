#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simongame.h"

class SimonGame;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void displayFlashingColor(int);
    void gameOver();
    void setEnableButtons();

private slots:
    void on_startButton_clicked();
    void flashColorButton();
    void unflashColorButton();

    void on_redButton_clicked();
    void on_blueButton_clicked();

    void updatePlayerProgressBar(int matchedMoves, int totalMoves);
    void displayProgressBar();

signals:
    void startNewGame();
    void playerClickedButton(int);

private:
    Ui::MainWindow *ui;
    void setDisableButtons();
    SimonGame *simongame;
    int currentColor;
};
#endif // MAINWINDOW_H
