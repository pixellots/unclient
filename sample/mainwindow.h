#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    int executeCommand(const QString& commandMode);

private slots:
    void on_actionCheck_for_updates_triggered();
    void on_actionCheck_for_messages_triggered();
    void on_actionLaunch_Manager_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
