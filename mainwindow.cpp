#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("NapNap");

    //Учет сохраненного пользовательского выбора
    ui->spinBox->setValue(settings.value("time/work", 45).toInt());
    ui->spinBox_2->setValue(settings.value("time/rest", 15).toInt());
    ui->lineEdit->setText(settings.value("text/popup", "Nap-nap, it's time to take a pause, dear").toString());

    this -> setTrayIconActions();                              //Отображаем в трее
    this -> showTrayIcon();

    popUp = new PopUp();
    workTimer = new QTimer();
    statusTimer = new QTimer();
    timeLeft = new QLabel(this);
    ui->statusBar->addWidget(timeLeft); //для частого обновления оставшегося времени

    connect(workTimer, &QTimer::timeout, this, &MainWindow::ariseNapnap);   //Подключаем вызов виджета к таймеру работы
    connect(statusTimer, &QTimer::timeout, this, &MainWindow::status_update);   //Подключаем вызов виджета к таймеру работы
    connect(popUp, &PopUp::imHidden, this, &MainWindow::on_pushButton_clicked); //Подключаем перезапуск таймера работы к сокрытию виджета
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    workTimer->start(ui->spinBox->text().toInt()*60000);
    statusTimer->start(1000);
}

void MainWindow::ariseNapnap()
{
    statusTimer->stop();
    workTimer->stop();
    popUp->setPopupDuration(ui->spinBox_2->text().toInt());
    popUp->setPopupText(ui->lineEdit->text());
    popUp->show();
    statusTimer->start(1000);
}

void MainWindow::status_update()
{
    if(workTimer->isActive())
    {
        timeLeft->setText(QString::number((workTimer->remainingTime()+60000)/60000) + " minutes of work left");
        timeShow->setText(timeLeft->text());
    }
    else
    {
        timeLeft->setText(QString::number((popUp->getrestTimer()->remainingTime()+60000)/60000) + " minutes of rest left");
        timeShow->setText(timeLeft->text());
    }
}

void MainWindow::showTrayIcon()
{
    // Создаём экземпляр класса и задаём его свойства
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage(":/images/wolf-paw.png");
    trayIcon->setIcon(trayImage);
    trayIcon->setContextMenu(trayIconMenu);

    // Выводим значок
    trayIcon->show();
}


void MainWindow::setTrayIconActions()
{
    // Настройки actions
    timeShow = new QAction("", this);
    restoreAction = new QAction("Expand", this);
    minimizeAction = new QAction("Hide", this);
    quitAction = new QAction("Exit", this);

    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Настройки менюшки трея
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(timeShow);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
}

void MainWindow::changeEvent(QEvent *event)     //Перехватчик событий сворачивания трея
{
    QMainWindow::changeEvent(event);
    if(event->type() == QEvent::WindowStateChange)
    {
        if(isMinimized())
        {
            this->hide();
        }
    }
}

//Далее - автоматическое сохранение пользовательских изменений

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    settings.setValue("text/popup", arg1);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    settings.setValue("time/work", arg1);
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    settings.setValue("time/rest", arg1);
}
