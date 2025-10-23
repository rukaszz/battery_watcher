#include "config.h"
#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QTimer>

Config::Config(QObject *parent) : QObject(parent) {
    applyDefaults();
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &Config::onFileChanged);
}

void Config::applyDefaults(){
    m_lowThreshold = 30;
    m_highThreshold = 80;
    m_checkIntervalSeconds = 60;
    m_notifyCooldownSeconds = 600;
    m_toastDurationMs = 5000;
    m_snoozeMinutes = 10;
    m_msgLow = QStringLiteral("Remaining amount %1% - Please insert the charging plug. ");
    m_msgHigh = QStringLiteral("Remaining amount %1% - Please unplug the charging plug. ");
    m_msgDialog = QStringLiteral("Remaining amount %1%, what would you like to do? ");
    m_loggingEnabled = false;
    m_loggingPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/log.csv";
}

bool Config::load(const QString &path){
    QString realPath = path;
    if(realPath.isEmpty()){
        realPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +"battery-watcher/config.json";
    }
    QFile f(realPath);
    if(!f.exists()){
        // ensure directory exists and write defaults
        QDir dir = QFileInfo(realPath).dir();
        if(!dir.exists()) dir.mkpath(".");
        save(realPath);
    }
    if(!f.open(QIODevice::ReadOnly)){
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
    m_watcher.removePaths(m_watcher.files());
    m_watcher.addPath(m_path);
    emit changed();
    return true;
}

bool Config::save(const QString &path) const{
    QString realPath = path;
    if(realPath.isEmpty()){
        realPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/battery-watcher/config.json";
    }
    QFile f(realPath);
    QDir dir = QFileInfo(realPath).dir();
    if(!dir.exists()) dir.mkpath(".");
    if(!f.open(QIODevice::WriteOnly)) return false;

    QJsonObject obj;
    obj["lowThreshold"] = m_lowThreshold;
    obj["highThreshold"] = m_highThreshold;
    obj["checkIntervalSeconds"] = m_checkIntervalSeconds;
    obj["notifyCooldownSeconds"] = m_notifyCooldownSeconds;
    obj["toaslDurationMs"] = m_toastDurationMs;
    obj["snoozeMinutes"] = m_snoozeMinutes;

    QJsonObject msgs;
    msgs["low"] = m_msgLow;
    msgs["high"] = m_msgHigh;
    msgs["dialog"] = m_msgDialog;

    obj["messages"] = msgs;

    QJsonObject logobj;
    logobj["enabled"] = m_loggingEnabled;
    logobj["path"] = m_loggingPath;
    obj["logging"] = logobj;

    QJsonDocument doc(obj);
    f.write(doc.toJson(QJsonDocument::Indented));

    return true;
}

void Config::parseJson(const QJsonObject &obj){
    // 最初はデフォルト，もし修正されていたら上書き
    applyDefaults();

    if(obj.contains("lowThreshold")) m_lowThreshold = obj["lowThreshold"].toInt(m_lowThreshold);
    if(obj.contains("highThreshold")) m_highThreshold = obj["highThreshold"].toInt(m_highThreshold);
    if(obj.contains("checkIntervalSeconds")) m_checkIntervalSeconds = obj["checkIntervalSeconds"].toInt(m_checkIntervalSeconds);
    if(obj.contains("notifyCooldownSeconds")) m_notifyCooldownSeconds = obj["notifyCooldownSeconds"].toInt(m_notifyCooldownSeconds);
    if(obj.contains("toastDurationMs")) m_toastDurationMs = obj["toastDurationMs"].toInt(m_toastDurationMs);
    if(obj.contains("snoozeMinutes")) m_snoozeMinutes = obj["snoozeMinutes"].toInt(m_snoozeMinutes);
    if(obj.contains("messages") && obj["messages"].isObject()){
        QJsonObject l = obj["logging"].toObject();
        m_loggingEnabled = l["enabled"].toBool(m_loggingEnabled);
        m_loggingPath = l["path"].toString(m_loggingPath);
    }
}

void Config::onFileChanged(const QString &path){
    // 編集時に若干の遅延を入れて調整する
    QTimer::singleShot(200, this, [this, path](){
        QFile f(path);
        if(f.exists()){
            // 変更されたものを出力する
            load(path);
        }
    });
}

// getters
int Config::lowThreshold() const {return m_lowThreshold;}
int Config::highThreshold() const {return m_highThreshold;}
int Config::checkIntervalSeconds() const {return m_checkIntervalSeconds;}
int Config::notifyCooldownSeconds() const {return m_notifyCooldownSeconds;}
int Config::toastDurationMs() const {return m_toastDurationMs;}
int Config::snoozeMinutes() const {return m_snoozeMinutes;}
QString Config::msgLow() const {return m_msgLow;}
QString Config::msgHigh() const {return m_msgHigh;}
QString Config::msgDialog() const {return m_msgDialog;}
bool Config::loggingEnable() const {return m_loggingEnabled;}
QString Config::loggingPath() const {return m_loggingPath;}
