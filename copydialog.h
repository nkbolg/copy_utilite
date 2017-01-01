#ifndef COPYDIALOG_H
#define COPYDIALOG_H

#include <QDialog>

class CopyDialog : public QDialog
{
    Q_OBJECT
public:
    CopyDialog(QWidget *parent = 0);
    ~CopyDialog();
};

#endif // COPYDIALOG_H
