#include "View/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    try {
        this->windowController = new MainWindowController();
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }

    // For taking a photo from video at the entrance, by pressing key E, or at the exit, by pressing key Q
    connect(new QShortcut(QKeySequence(Qt::Key_E), this), &QShortcut::activated, this, &MainWindow::GetEntranceFrame);
    connect(new QShortcut(QKeySequence(Qt::Key_Q), this), &QShortcut::activated, this, &MainWindow::GetExitFrame);

    // Button for changing video source for parking lot cameras
    connect(ui->changeVideoSourceAction, SIGNAL(triggered()), this, SLOT(OnChangeVideoSourceButtonClick()));
    connect(ui->systemTurnButton, SIGNAL(clicked()), this, SLOT(OnRefreshButtonClicked()));

}

void MainWindow::GetEntranceFrame() {
    try {
        windowController->TakeEntranceFrame();
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void MainWindow::GetExitFrame() {
    try {
        windowController->TakeExitFrame();
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void MainWindow::OnChangeVideoSourceButtonClick() {
    
    // TODO: Allow user to change video sources
    qDebug() << "button press";
}

void MainWindow::OnRefreshButtonClicked()
{
    try {
        windowController->SetupCameras();
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

