#ifndef WATCHER_H
#define WATCHER_H
#include "config.h"
#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>

class BatteryWatcher : public QObject{
    Q_OBJECT
public:
    // 暗黙のコンストラクタの呼び出しの禁止
    explicit BatteryWatcher(QSystemTrayIcon *tray, Config *cfg, QObject *parent = nullptr);
    void checkOnce();

private slots:
    void onTimeout();
    void onConfigChanged();

private:
    QSystemTrayIcon *m_tray;
    QTimer m_timer;
    Config *m_cfg;
    int m_lowThreshold = 35;
    int m_highThreshold = 80;
    int readCapacity() const;
    QString readStatus() const;
    qint64 m_lastLowNotify = 0;
    qint64 m_lastHighNotify = 0;
    qint64 m_notifyCooldownMs = 10 * 60 * 1000; // 10分
    void notify(const QString &title, const QString &body);
};

#endif // WATCHER_H
