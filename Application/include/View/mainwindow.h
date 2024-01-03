#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QShortcut>

#include <Model/Utils/Interfaces/InterfaceVideoListener.h>
#include <Controller/MainWindowController.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    const QString buttonStyleSheet = "QPushButton {	border: 2px solid gray; border-radius: 10px; padding: 0 8px; background: lightgray;	} QPushButton:hover { color: white }";

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:

    void closeEvent(QCloseEvent* event);

private slots:

    // TODO: Accept different keys in case of multiple entrances/exits

    void GetEntranceFrame(); // when key E is pressed.
    void GetExitFrame(); // when key Q is pressed.

    void OnChangeVideoSourceButtonClick();
    void OnRefreshButtonClicked();

private:
    Ui::MainWindow *ui;

    MainWindowController* windowController;
};
#endif // MAINWINDOW_H
