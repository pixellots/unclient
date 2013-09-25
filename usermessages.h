#ifndef USERMESSAGES_H
#define USERMESSAGES_H

#include <QDialog>
#include <QList>
#include <QUrl>

#include "message.h"
#include "updatenode_service.h"

namespace Ui {
class UserMessages;
}

class UserMessages : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserMessages(QWidget *parent = 0);
    ~UserMessages();
    
    void init(UpdateNode::Service* aService);

public slots:
    void serviceDone();
    void onLeft();
    void onRight();
    void onRead();
    void openLink(const QUrl&);

private:
    void showMessage();

private:
    Ui::UserMessages *ui;
    UpdateNode::Service* m_pService;
    QList<UpdateNode::Message> m_listMessages;

    int m_iCurrentIndex;
};

#endif // USERMESSAGES_H
