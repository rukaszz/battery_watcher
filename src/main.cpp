#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include "watcher.h"

int main(int argc, char **argv){
    QApplication app(argc, argv);
    // トレイアイコンが使えるか確認
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qFatal("System tray is not available");
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);
    // trayの定義
    QSystemTrayIcon tray;
    tray.setIcon(QIcon::fromTheme("battery"));
    tray.setToolTip("Battery Watcher");

    // メニューの作成
    QMenu menu;
    QAction quitAction("Quit");
    QObject::connect(&quitAction, &QAction::triggered, &app, &QApplication::quit);
    menu.addAction(&quitAction);
    tray.setContextMenu(&menu);
    tray.show();

    BatteryWatcher watcher(&tray);

    return app.exec();
}
