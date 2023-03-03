#include "gcled.h"
#include <QPainter>

GCLed::GCLed(QWidget *parent)
    : QLabel(parent)
    , mState(eState::Close)
    , linearGradient(0,0,0,1)
{
    linearGradient.setColorAt(0.0, QColor("#E1E1E1"));
    linearGradient.setColorAt(0.4, QColor("#DDDDDD"));
    linearGradient.setColorAt(0.5, QColor("#D8D8D8"));
    linearGradient.setColorAt(1.0, QColor("#D3D3D3"));
}

void GCLed::setState(eState state)
{
    mState = state;
}

void GCLed::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::white);
    painter.setBrush(linearGradient);
    int w = this->width();
    int h = this->height();
    int wdiv4 = w >> 2;
    int hdiv4 = h >> 2;

    painter.drawEllipse(wdiv4,hdiv4, w - wdiv4*2, h - hdiv4*2);

    if(mState == eState::Open)
        painter.setBrush(Qt::green);
    else if(mState == eState::Close)
        painter.setBrush(Qt::gray);
    painter.drawEllipse(wdiv4+1, hdiv4+1, w - (wdiv4+1)*2, h - (hdiv4+1)*2);
    painter.end();
    update();
}

eState GCLed::state() const
{
    return mState;
}
