#include "watcher.h"
#include "config.h"
#include "settingsdialog.h"
#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

int main(int argc, char **argv){
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
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
    QAction *settingsAct = menu.addAction("Settings...");
    QAction *quitAct = menu.addAction("Quit");

    tray.setContextMenu(&menu);
    tray.show();

    // config
    Config config;
    // default path: ~/.config/battery-watcher/config.json
    config.load("~/battery_watcher/config.json");

    // watcher
    BatteryWatcher watcher(&tray, &config);
    
    // settings dialog
    SettingsDialog settings(&config);
    QObject::connect(settingsAct, &QAction::triggered, [&settings](){
        settings.exec();    // modal; if you want to show modaless, settings.show()
    });
    // quit process
    QObject::connect(quitAct, &QAction::triggered, &app, &QApplication::quit);
    
    return app.exec();
}
