#ifndef USERNOTOFICATION_H
#define USERNOTOFICATION_H

#include <QDialog>

namespace Ui {
    class UserNotofication;
}

class UserNotofication : public QDialog {
    Q_OBJECT
public:
    UserNotofication(QWidget *parent = 0);
    ~UserNotofication();

public:
    void updateView();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::UserNotofication *ui;

    int m_iNewUpdates;
    int m_iNewMessages;
};

#endif // USERNOTOFICATION_H
