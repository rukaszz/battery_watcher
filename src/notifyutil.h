#ifndef NOTIFYUTIL_H
#define NOTIFYUTIL_H
#include <QString>
#include <QSystemTrayIcon>

class QWidget;

// トレイの通知を出す
void showSystemNotification(QSystemTrayIcon *tray, const QString &title, const QString &body);

// モーダルウィンドウでのダイアログ(OK/スヌーズの機能)
int showBlockingDialog(QWidget *parent, const QString &title, const QString &msg);

#endif  // NOTIFYUTIL_H
