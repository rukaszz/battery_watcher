#ifndef CONFIG_H
#define CONFIG_H
#include <QObject>
#include <QFileSystemWatcher>
#include <QJsonObject>

class Config : public QObject{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    // 指定パスから読み込み(true=成功)
    bool load(const QString &path);
    // 書き込む
    bool save(const QString &path) const;

    QString path() const{
        return m_path;
    }

    // getters
    int lowThreshold() const;
    int highThreshold() const;
    int checkIntervalSeconds() const;
    int notifyCooldownSeconds() const;
    int toastDurationMs() const;
    int snoozeMinutes() const;
    QString msgLow() const;
    QString msgHigh() const;
    QString msgDialog() const;
    bool loggingEnable() const;
    QString loggingPath() const;

    // setters
    void setLowThreshold(int v);
    void setHighThreshold(int v);
    void setCheckIntervalSeconds(int v);
    void setNotifyCooldownSeconds(int v);
    void setToastDurationMs(int v);
    void setSnoozeMinutes(int v);
    void setMsgLow(const QString &s);
    void setMsgHigh(const QString &s);
    void setMsgDialog(const QString &s);
    void setLoggingPath(const QString &p);

// Qtオブジェクトを継承したクラスが発行するイベント
signals:
    void changed();

// signalsのイベントリスナーであり登録(connect)する
private slots:
    void onFileChanged(const QString &path);

private:
    void applyDefaults();
    void parseJson(const QJsonObject &obj);

    QString m_path;
    QFileSystemWatcher m_watcher;
    // config file values
    int m_lowThreshold;
    int m_highThreshold;
    int m_checkIntervalSeconds;
    int m_notifyCooldownSeconds;
    int m_toastDurationMs;
    int m_snoozeMinutes;
    QString m_msgLow, m_msgHigh, m_msgDialog;
    bool m_loggingEnabled;
    QString m_loggingPath;
};

#endif // CONFIG_H
