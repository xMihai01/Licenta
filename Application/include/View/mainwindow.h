#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    const QString buttonStyleSheet = "QPushButton {	border: 2px solid gray; border-radius: 10px; padding: 0 8px; background: lightgray;	} QPushButton:hover { color: white }";

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    //VideoCamera* m_videoCamera;

    //HSVComponents* m_HSVComponents;
    //DetectionGUIComponents* m_detectionGUIComponents;

   /* std::shared_ptr<InterfaceVideoListener> m_originalVideoListener;
    std::shared_ptr<InterfaceVideoListener> m_convertedVideoListener;*/
};
#endif // MAINWINDOW_H
