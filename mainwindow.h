#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_playButton_pressed();

    void on_actionOpen_triggered();

    void on_forwardButton_pressed();

    void onMediaPositionChanged();
    void onMediaDurationChanged();
    void onVideoAvailableChanged();
    void on_backwardButton_pressed();

    void on_volumeSlider_valueChanged(int value);

    void on_actionQuit_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QMediaPlayer player;
};
#endif // MAINWINDOW_H
