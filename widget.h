#ifndef WIDGET_H
#define WIDGET_H

#include "newwordwidget.h"
#include "testwidget.h"
#include "dictwidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    TestWidget *testWidget;
    NewWordWidget *newWordWidget;
    DictWidget *dictWidget;

    QPushButton *addWord, *test;
    QLabel *dictStats;
    QSlider *timeSlider;

    QVector <int> timePeriods;

private slots:
    void Test();
    void AddWord();

    void updateSlider();
    void sliderChange();

    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
};
#endif // WIDGET_H
