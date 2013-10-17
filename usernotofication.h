#ifndef USERNOTOFICATION_H
#define USERNOTOFICATION_H

#include <QDialog>
#include <QUrl>

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

public slots:
    void openLink(const QUrl& aUrl);
    void updateSelectedUpdate();
    void detailsClicked();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::UserNotofication *ui;
};

#endif // USERNOTOFICATION_H
