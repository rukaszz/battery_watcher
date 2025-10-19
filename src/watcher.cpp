#include "watcher.h"
#include "toast.h"
#include "notifyutil.h"
#include <QFile>
#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QSystemTrayIcon>

BatteryWatcher::BatteryWatcher(QSystemTrayIcon *tray, QObject *parent) : QObject(parent), m_tray(tray){
    m_timer.setInterval(60 * 1000); // 60秒ごと
    connect(&m_timer, &QTimer::timeout, this, &BatteryWatcher::onTimeout);
    m_timer.start();
    // first check
    checkOnce();
}

void BatteryWatcher::onTimeout(){
    checkOnce();
}

int BatteryWatcher::readCapacity() const {
    QFile f("/sys/class/power_supply/BAT1/capacity");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) return -1;
    QTextStream in(&f);
    int v = in.readLine().trimmed().toInt();
    return v;
}

QString BatteryWatcher::readStatus() const{
    QFile f("/sys/class/power_supply/BAT1/status");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) return QString();
    QTextStream in(&f);
    return in.readLine().trimmed();
}

void BatteryWatcher::notify(const QString &title, const QString &body) {
    // QSystemTrayIcon::showMessage を使う（libnotify と統合される）
    m_tray->showMessage(title, body, QSystemTrayIcon::Information, 10 * 1000);
}

void BatteryWatcher::checkOnce(){
    int cap = readCapacity();
    if(cap < 0) return;

    QString status = readStatus();
    qint64 now = QDateTime::currentMSecsSinceEpoch();

    if(cap <= m_lowThreshold && status != "Charging"){
        QString battery_low = QString("Remaining amout %1% - Please insert the charging code").arg(cap);
        showSystemNotification(m_tray, "Battery low", battery_low);

        Toast *t = new Toast(battery_low);
        t->showAtTopCenter();

        int res = showBlockingDialog(nullptr, "Battery low", "Would you like to charge it? ");
        if(res == 1){
            //snooze
        }
    }
    if(cap >= m_highThreshold && status == "Charging"){
        QString battery_high = QString("Remaining amout %1% - Please unplug the charging code").arg(cap);
        showSystemNotification(m_tray, "Battery high", battery_high);

        Toast *t = new Toast(battery_high);
        t->showAtTopCenter();

        int res = showBlockingDialog(nullptr, "Battery high", "Would you like to finish it? ");
        if(res == 1){
            //snooze
        }
    }
}
