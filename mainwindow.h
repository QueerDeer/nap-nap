#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSettings>
#include "popup.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();   //Старт таймера с отсечками по "min for work"
    void ariseNapnap();             //Показ виджета
    void setTrayIconActions();
    void showTrayIcon();
    void status_update();           //min of work left

    void on_lineEdit_textChanged(const QString &arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

protected:
    void changeEvent(QEvent*);

private:
    Ui::MainWindow *ui;
    QSettings settings;
    PopUp *popUp;
    QTimer *workTimer;          // Таймер, по которому будет вызываться виджет
    QTimer *statusTimer;        // Таймер для min left в статусбаре
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QAction *timeShow;
    QSystemTrayIcon *trayIcon;
    QLabel* timeLeft;
};

#endif // MAINWINDOW_H
