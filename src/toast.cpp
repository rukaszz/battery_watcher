#include "toast.h"
#include <QHBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>

Toast::Toast(const QString &message, QWidget *parent, int durationMs) : QWidget(parent), m_durationMs(durationMs){
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating); // フォーカスは取らないようにする
    m_label = new QLabel(message);
    m_label->setStyleSheet(
        "QLabel { background-color: rgba(40, 40, 40, 230); color: white; padding: 12px; "
        "border-radius: 8px; font-size: 14px; }"
    );
    auto layout = new QHBoxLayout(this);
    layout->addWidget(m_label);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    adjustSize();

    m_anim = new QPropertyAnimation(this, "pos", this);
    m_anim->setDuration(300);
}

void Toast::showAtTopCenter(){
    // 画面トップの計算
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect geo = screen->availableGeometry();
    int w = width();
    int x = geo.x() + (geo.width() - w) / 2;
    int y_show = geo.y() + 20;  // 表示位置(上から20px)
    int y_hide = y_show - height() - 10;    // 移動先(隠れる位置へ)

    move(x, y_hide);
    show();

    // スライドダウンして表示する
    m_anim->stop();
    m_anim->setStartValue(QPoint(x, y_hide));
    m_anim->setEndValue(QPoint(x, y_show));
    m_anim->start();

    // 自動的に消す
    QTimer::singleShot(m_durationMs, this, [this, x, y_hide](){
        // スライドアップして消える
        m_anim->stop();
        m_anim->setStartValue(pos());
        m_anim->setEndValue(QPoint(x, y_hide));
        m_anim->start();
        connect(m_anim, &QPropertyAnimation::finished, this, &Toast::close);
    });
}
