#ifndef WIDGET_H
#define WIDGET_H

#include <thread>
#include <memory>

#include <QWidget>

class QLineEdit;


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void beginCopy();
    void endCopy();
    void copyCanceled();
signals:
    void threadFinished();
private:
    QLineEdit *leSrc;
    QLineEdit *leDst;
    std::unique_ptr<std::thread> workingThread;
};

#endif // WIDGET_H
