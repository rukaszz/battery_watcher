#ifndef TOAST_H
#define TOAST_H
#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>

class Toast : public QWidget{
    Q_OBJECT
public:
    explicit Toast(const QString &message, QWidget *rarent = nullptr, int durationMs = 5000);
    void showAtTopCenter();

private:
    QLabel *m_label;
    QPropertyAnimation *m_anim;
    int m_durationMs;
};

#endif // TOAST_H
