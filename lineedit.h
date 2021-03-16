#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QtWidgets>

class LineEdit : public QLineEdit {
    Q_OBJECT
public:
    LineEdit(QWidget *parent = nullptr);

    void highlightAsnwer(int mode);

    void setText(QString string);
    QString text();
    bool isActive();

private:
    QTimer *timer;
    int Hue, Mode;

private slots:
    void changeText();
    void changeSelection();
    void changeCursorPosition();

    void rightAnswer();
    void wrongAnswer();

    void resizeEvent(QResizeEvent *event);

signals:
    void nextWord();
    void showWord();
};

#endif // LINEEDIT_H
