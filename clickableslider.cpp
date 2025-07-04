#include "ClickableSlider.h"

ClickableSlider::ClickableSlider(QWidget *parent) : QSlider(parent)
{
}

void ClickableSlider::mousePressEvent(QMouseEvent *event)
{
    if (orientation() == Qt::Horizontal) {
        int value = minimum() + ((maximum()-minimum()) * event->x()) / width();
        setValue(value);
        emit sliderClicked(value);
    }
    QSlider::mousePressEvent(event);
}
