#ifndef GCLED_H
#define GCLED_H

#include <QObject>
#include <QLabel>


typedef enum{
    Close = 0x00,
    Open = 0x01
}eState;

class GCLed : public QLabel
{
    Q_OBJECT
public:
    GCLed(QWidget *parent = nullptr);

    void setState(eState state);


    eState state() const;

protected:

    void paintEvent(QPaintEvent *);

private:
    eState mState;
    QLinearGradient linearGradient;
};

#endif // GCLED_H
