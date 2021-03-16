#ifndef NEWWORDWIDGET_H
#define NEWWORDWIDGET_H

#include <QtWidgets>

#include "dictwidget.h"
#include "lineedit.h"

class NewWordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewWordWidget(DictWidget *Dict, QWidget *parent = nullptr);

    void clear();
    void setLineEditFocus();

private:
    DictWidget *dict;
    LineEdit *original, *translation;
    QPushButton *submit;

    QTimer *timer;

    void resizeEvent(QResizeEvent *event);

private slots:
    void submitWord();
    void format();

signals:
    void wordAdded();

};

#endif // NEWWORDWIDGET_H
