#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "sara_service.h"

namespace Ui
{
    class DialogUpdate;
}

class Dialog : public QDialog
{
    Q_OBJECT
    public:
        Dialog(QWidget *parent = 0);
        ~Dialog();

        void init(Sara::Service* aService);

    protected:
        void changeEvent(QEvent *e);

    public slots:
        void serviceDone();
        void refresh();

    private:
        Ui::DialogUpdate* m_pUI;
        Sara::Service* m_pService;
};

#endif // DIALOG_H
