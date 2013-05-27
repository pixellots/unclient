#ifndef USERMESSAGES_H
#define USERMESSAGES_H

#include <QDialog>
#include "sara_service.h"

namespace Ui {
class UserMessages;
}

class UserMessages : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserMessages(QWidget *parent = 0);
    ~UserMessages();
    
    void init(Sara::Service* aService);

public slots:
    void serviceDone();

private:
    Ui::UserMessages *ui;
    Sara::Service* m_pService;
};

#endif // USERMESSAGES_H
