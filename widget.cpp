#include "widget.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLine>
#include <QPushButton>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>
#include <QFileDialog>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout;

    leSrc = new QLineEdit;
    leDst = new QLineEdit;

    QPushButton *btnSrc = new QPushButton("Browse");
    connect(btnSrc, &QPushButton::clicked, [this]{
        QString file = QFileDialog::getExistingDirectory(this);
        this->leSrc->setText(file);
    });
    QPushButton *btnDst = new QPushButton("Browse");
    connect(btnDst, &QPushButton::clicked, [this]{
        QString file = QFileDialog::getExistingDirectory(this);
        this->leDst->setText(file);
    });

    QHBoxLayout *laySrc = new QHBoxLayout;
    laySrc->addWidget(leSrc);
    laySrc->addWidget(btnSrc);

    QHBoxLayout *layDst = new QHBoxLayout;
    layDst->addWidget(leDst);
    layDst->addWidget(btnDst);

    formLayout->addRow("Source folder:", laySrc);
    formLayout->addRow("Destination folder:", layDst);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *btn = new QPushButton("Copy");
    btn->setFixedSize(btn->sizeHint());
    connect(btn, &QPushButton::clicked, this, &Widget::beginCopy);
    btnLayout->addWidget(btn, 0, Qt::AlignRight);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    setFixedHeight(mainLayout->minimumSize().height());
}

Widget::~Widget()
{

}

bool check(const QString &path)
{
    QFileInfo fi(path);
    return fi.isDir();
}

void Widget::beginCopy()
{
    QString srcDir = leSrc->text();
    QString dstDir = leDst->text();
    if (check(srcDir) == false ||
            check(dstDir) == false) {
        QMessageBox::critical(this, "Error!", "Error in input params");
        return;
    }
    QProgressDialog *progressDialog = new QProgressDialog("Preparing copy...", "Cancel", 0, 100, this);
    progressDialog->setFixedSize(progressDialog->sizeHint());
    progressDialog->setModal(true);

    connect(progressDialog, &QProgressDialog::finished, this, &Widget::endCopy);
    connect(progressDialog, &QProgressDialog::canceled, this, &Widget::copyCanceled);

    workingThread = make_unique<::thread>([ptr = this, srcDir = std::move(srcDir), dstDir = std::move(dstDir)]{
        //проверить, что src не пустое,
        //Если нет - строим мап относительных путей до файлов с хешами для обеих папок, тут же считаем суммарный размер
        //Находим то, чего нет в dst (for elem in src: check if is in dst)
        //???

});

    progressDialog->show();
}

void Widget::endCopy()
{
    QProgressDialog *dialog = qobject_cast<QProgressDialog*>(sender());
    qDebug () << "ended";
    dialog->deleteLater();
    workingThread->join();
    workingThread.reset(nullptr);
}

void Widget::copyCanceled()
{
    QProgressDialog *dialog = qobject_cast<QProgressDialog*>(sender());
    qDebug () << "canceled";
    dialog->deleteLater();
    workingThread->join();
    workingThread.reset(nullptr);
}

