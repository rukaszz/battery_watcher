#include "config.h"
#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

Config::Config(QObject *parent) : QObject(parent) {
    applyDefaults();
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &Config::onFileChanged);
}

void Config::applyDefault(){
    m_lowThreshold = 30;
    m_highThreshold = 80;
    m_checkIntervalSeconds = 60;
    m_toastDurationMs = 5000;
    m_snoozeMinutes = 10;
    m_msgLow = QStringLiteral("Remaining amount %1% - Please insert the charging plug. ");
    m_msgHigh = QStringLiteral("Remaining amount %1% - Please unplug the charging plug. ");
    m_msgDialog = QStringLiteral("Remaining amount %1%, what would you like to do? ");
    m_loggingEnabled = false;
    m_loggingPath = QStandardPaths::writableLocation(QStandPaths::DataLocation) + "./log.csv";
}

bool Config::load(const QString &path){
    QString realPath = path;
    if(realPath.isEmpty()){
        realPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +"battery-watcher/config.json"
    }
    QFile f(realPath);
    if(!f.exists()){
        // ensure directory exists and write defaults
        QDir = QFileInfo(realPath).dir();
        if(!dir.exists()) dir.mkpath(".");
        save(realPath);
    }
    if(!f.open(QIDevice::ReadOnly)){
        m_path = realPath;
        return false;
    }
    QByteArray data = f.readAll();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if(err.error != QJsonParseError::NoError || !doc.isObject()){
        m_path = realPath;
        return false;
    }
    parseJson(doc.object());
    m_path = realPath;
    // watch file for hot reload
    m_watcher.removePath(m_watcher.files());
    m_watcher.addPath(m_path);
    emit changed();
    return true;
}
