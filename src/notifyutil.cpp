#include "notifyutil.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProcess>

void showSystemNotification(QSystemTrayIcon *tray, const QString &title, const QString &body){
    if(tray && tray->isVisible()){
        tray->showMessage(title, body, QSystemTrayIcon::Information, 10000);
    } else {
        // フォールバック：notify-send
        QProcess::execute("notify-send", QStringList() << "-u" << "critical" << title << body);
    }
}

int showBlockingDialog(QWidget *parent, const QString &title, const QString &msg){
    QDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setWindowFlags(dlg.windowFlags() | Qt::WindowStaysOnTopHint);
    dlg.setModal(true);
    dlg.setWindowModality(Qt::ApplicationModal);

    auto layout = new QVBoxLayout(&dlg);
    auto label = new QLabel(msg);
    layout->addWidget(label);
    auto btnOk = new QPushButton("OK");
    auto btnSnooze = new QPushButton("Snooze 10 min");
    auto btnLayout = new QHBoxLayout();

    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnSnooze);
    layout->addLayout(btnLayout);

    QObject::connect(btnOk, &QPushButton::clicked, &dlg, &QDialog::accept);
    QObject::connect(btnSnooze, &QPushButton::clicked, [&dlg](){dlg.done(1);});
    
    int res = dlg.exec();
    return res; // 0 is OK, 1 is Snooze
}
