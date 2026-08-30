#pragma once

#include <QWidget>
#include <QTimer>
#include <QVector>

class EqualizerWidget : public QWidget {
    Q_OBJECT

public:
    EqualizerWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTimer *timer;
    QVector<int> barHeights;

    void updateBars();
};
