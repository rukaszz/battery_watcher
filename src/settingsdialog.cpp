#include "settingsdialog.h"
#include "config.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>

SettingsDialog::SettingsDialog(Config *cfg, QWidget *parent) : QDialog(parent), m_cfg(cfg){
    setWindowTitle(tr("Battery watcher Setting"));
    setModal(true);
    resize(500, 400);

    auto mainLayout = new QVBoxLayout(this);
    auto form = new QFormLayout();

    // set Spinbox
    m_lowSpin = new QSpinBox();
    m_lowSpin->setRange(0, 100);

    m_highSpin = new QSpinBox();
    m_highSpin->setRange(0, 100);

    m_checkIntervalSpin = new QSpinBox();
    m_checkIntervalSpin->setRange(1, 3000);

    m_notifyCooldownSpin = new QSpinBox();
    m_notifyCooldownSpin->setRange(0, 86400);

    m_toastDurationSpin = new QSpinBox();
    m_toastDurationSpin->setRange(100, 60000);

    m_snoozeSpin = new QSpinBox();
    m_snoozeSpin->setRange(1, 1440);

    // set regarding text processing
    m_logPathEdit = new QLineEdit();
    QPushButton *chooseLogBtn = new QPushButton(tr("Bwowse..."));
    connect(chooseLogBtn, &QPushButton::clicked, this, [this](){
        QString p = QFileDialog::getSaveFileName(this, tr("Select log file"), m_logPathEdit->text());
        if(!p.isEmpty()) m_logPathEdit->setText(p);
    });
    
    auto logH = new QHBoxLayout();
    logH->addWidget(m_logPathEdit);
    logH->addWidget(chooseLogBtn);

    m_msgLowEdit = new QTextEdit();
    m_msgHighEdit = new QTextEdit();
    m_msgDialogEdit = new QTextEdit();

    form->addRow(tr("Low threshold (%)"), m_lowSpin);
    form->addRow(tr("high threshold (%)"), m_highSpin);
    form->addRow(tr("Check interval (s)"), m_checkIntervalSpin);
    form->addRow(tr("Notify cooldown (s)"), m_notifyCooldownSpin);
    form->addRow(tr("Toast duration (ms)"), m_notifyCooldownSpin);
    form->addRow(tr("snooze (min)"), m_snoozeSpin);
    form->addRow(tr("Log path"), logH);
    form->addRow(new QLabel(tr("Message for low (use %l for value)")));
    form->addRow(m_msgLowEdit);
    form->addRow(new QLabel(tr("Message for high (use %l for value)")));
    form->addRow(m_msgHighEdit);
    form->addRow(new QLabel(tr("Dialog message (use %l)")));
    form->addRow(m_msgDialogEdit);

    mainLayout->addLayout(form);

    m_buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    m_saveBtn = m_buttons->button(QDialogButtonBox::Save);
    m_revertBtn = new QPushButton(tr("Revert"));
    m_buttons->addButton(m_revertBtn, QDialogButtonBox::ResetRole);

    connect(m_buttons, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);
    connect(m_revertBtn, &QPushButton::clicked, this, &SettingsDialog::onRevertClicked);

    mainLayout->addWidget(m_buttons);
    m_snoozeSpin = new QSpinBox();
    m_snoozeSpin->setRange(1, 1440);


    // load initial valuees
    loadToUi();

    // hot-reload when underlying config file changes
    connect(m_cfg, &Config::changed, this, &SettingsDialog::onConfigChanged);
}

void SettingsDialog::loadToUi(){
    if(!m_cfg) return;

    m_lowSpin->setValue(m_cfg->lowThreshold());
    m_highSpin->setValue(m_cfg->highThreshold());
    m_checkIntervalSpin->setValue(m_cfg->checkIntervalSeconds());
    m_notifyCooldownSpin->setValue(m_cfg->notifyCooldownSeconds());
    m_toastDurationSpin->setValue(m_cfg->toastDurationMs());
    m_snoozeSpin->setValue(m_cfg->snoozeMinutes());
    m_logPathEdit->setText(m_cfg->loggingPath());
    m_msgLowEdit->setPlainText(m_cfg->msgLow());
    m_msgHighEdit->setPlainText(m_cfg->msgHigh());
    m_msgDialogEdit->setPlainText(m_cfg->msgDialog());
}

void SettingsDialog::saveFormUi(){
    // update Config object's members (we assume Config provides setters OR we can build a JSON and use save())
    // If your Config class doesn't have setters, we can construct a JSON and call save(path).
    // Here we assume simple setters exist; if not, I'll show alternate.
    // Let's use save() via temporary JSON by calling Config::save() after updating its internal state.
    // For simplicity, we'll add setter-like methods; if not present, adapt accordingly.
    // If you implemented setters (recommended), use them:

    m_cfg->setLowThreshold(m_lowSpin->value());
    m_cfg->setHighThreshold(m_highSpin->value());
    m_cfg->setCheckIntervalSeconds(m_checkIntervalSpin->value());
    m_cfg->setNotifyCooldownSeconds(m_notifyCooldownSpin->value());
    m_cfg->setToastDurationMs(m_toastDurationSpin->value());
    m_cfg->setSnoozeMinutes(m_snoozeSpin->value());
    m_cfg->setLoggingPath(m_logPathEdit->text());
    m_cfg->setMsgLow(m_msgLowEdit->toPlainText());
    m_cfg->setMsgHigh(m_msgHighEdit->toPlainText());
    m_cfg->setMsgDialog(m_msgDialogEdit->toPlainText());

    // presist
    if(!m_cfg->save(m_cfg->path())){
        QMessageBox::warning(this, tr("Save failed"), tr("Failed to write configuration file. "));
    } else {
        // notify that config changed(save may changed inside, but call anyway if not)
        emit m_cfg->changed();
    }

}

void SettingsDialog::onSaveClicked(){
    saveFormUi();
    accept();
}

void SettingsDialog::onRevertClicked(){
    loadToUi();
}

void SettingsDialog::onConfigChanged(){
    // if the config changed on disk while tge dialog is open, ask the user
    int r = QMessageBox::warning(this, tr("Config changed"), 
                                tr("Configuration file changed on disk, Reload values?"), 
                                QMessageBox::Yes | QMessageBox::No
            );
    if(r == QMessageBox::Yes) loadToUi();
}
