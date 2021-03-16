#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include "dictwidget.h"
#include "lineedit.h"

class TestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestWidget(DictWidget *Dict, QWidget *parent = nullptr);

private:
    QLabel *original, *finish, *hint;
    LineEdit *translation;
    QPushButton *help;

    DictWidget *dict;
    QVector <Word> words;
    QVector <int> *accuracy;
    int index;

    void stop();
    void resizeEvent(QResizeEvent *event);

private slots:
    void submitWord();

    void showWord();
    void nextWord();

public slots:
    void test(int time);
    void showHint();

signals:
    void testFinished();
};

#endif // TESTWIDGET_H
