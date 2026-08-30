#include "equalizerwidget.h"
#include <QPainter>
#include <cstdlib>
#include <ctime>

EqualizerWidget::EqualizerWidget(QWidget *parent)
    : QWidget(parent), barHeights(20, 0) // 20 نوار
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EqualizerWidget::updateBars);
    timer->start(100); // هر 100 میلی‌ثانیه نوارها عوض می‌شن
}

void EqualizerWidget::updateBars() {
    for (int i = 0; i < barHeights.size(); ++i) {
        barHeights[i] = rand() % height(); // ارتفاع تصادفی بین 0 تا ارتفاع ویجت
    }
    update(); // ری‌پینت کردن
}

void EqualizerWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int barWidth = width() / barHeights.size();

    for (int i = 0; i < barHeights.size(); ++i) {
        int x = i * barWidth;
        int y = height() - barHeights[i];
        QRect rect(x + 2, y, barWidth - 4, barHeights[i]);

        QColor color = QColor(3, 149, 85); // سبز
        painter.setBrush(color);
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect);
    }
}
