#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

namespace Ui {
class Helpdialog;
}

class Helpdialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Helpdialog(QWidget *parent = 0);
    ~Helpdialog();
    
public:
   void setText(const QString& aAppName, const QString& aText);

private:
    Ui::Helpdialog *ui;
};

#endif // HELPDIALOG_H
