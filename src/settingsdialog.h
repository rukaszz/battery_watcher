#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include <QDialog>

class Config;
class QSpinBox;
class QLineEdit;
class QTextEdit;
class QDialogButtonBox;
class QPushButton;

class SettingsDialog : public QDialog{
    Q_OBJECT
public:
    explicit SettingsDialog(Config *cfg, QWidget *parent = nullptr);

private slots:
    void onSaveClicked();
    void onRevertClicked();
    void onConfigChanged(); // ファイルが外部で書き換えられたとき用

private:
    void loadToUi();    // Config -> Ui
    void saveFormUi();  // UI -> Config

    Config *m_cfg;

    // widgets
    QSpinBox *m_lowSpin;
    QSpinBox *m_highSpin;
    QSpinBox *m_checkIntervalSpin;
    QSpinBox *m_notifyCooldownSpin;
    QSpinBox *m_toastDurationSpin;
    QSpinBox *m_snoozeSpin;
    QLineEdit *m_logPathEdit;
    QTextEdit *m_msgLowEdit;
    QTextEdit *m_msgHighEdit;
    QTextEdit *m_msgDialogEdit;

    QDialogButtonBox *m_buttons;
    QPushButton *m_saveBtn;
    QPushButton *m_revertBtn;
};

#endif // SETTINGSDIALOG_H
