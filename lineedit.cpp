#include "lineedit.h"

LineEdit::LineEdit(QWidget *parent) {
    this->setParent(parent);
    setText("");

    timer = new QTimer(this);

    connect(this, &LineEdit::textChanged, this, &LineEdit::changeText);
    connect(this, &LineEdit::selectionChanged, this, &LineEdit::changeSelection);
    connect(this, &LineEdit::cursorPositionChanged, this, &LineEdit::changeCursorPosition);

    connect(timer, &QTimer::timeout, [this] { Mode ? rightAnswer() : wrongAnswer(); });
}

bool LineEdit::isActive() {
    return timer->isActive();
}

void LineEdit::highlightAsnwer(int mode) {

    QPalette palette;
    palette.setColor(QPalette::Base, mode ? Qt::green : Qt::red);

    Mode = mode;
    Hue = 100;

    this->setReadOnly(true);
    this->setPalette(palette);

    timer->start(250);
}

void LineEdit::wrongAnswer() {
    QPalette palette;
    palette.setColor(QPalette::Base, QColor(255, 255 - Hue * 255 / 100, 255 - Hue * 255 / 100));

    this->setPalette(palette);

    timer->start(2);

    if (!Hue--) {
        timer->stop();
        this->setReadOnly(false);
        emit showWord();
    }
}

void LineEdit::rightAnswer() {
    QPalette palette;
    palette.setColor(QPalette::Base, Qt::white);

    this->setPalette(palette);

    timer->stop();
    this->setReadOnly(false);
    emit nextWord();
}

void LineEdit::setText(QString string) {
    this->QLineEdit::setText(" " + string.replace('_', ' '));
}

QString LineEdit::text() {
    QString text = this->QLineEdit::text().remove(0, 1);

    int ind = text.size();
    while (ind > 0 && text[--ind] == ' ');

    ++ind;
    return text.remove(ind, text.size() - ind);
}

void LineEdit::changeText() {
    QString text = this->QLineEdit::text();

    if (text.size() == 0 || text[0] != ' ')
        setText(text);

    this->update();
}

void LineEdit::changeSelection() {
    if (selectionStart() == 0)
        setSelection(1, selectionLength() - 1);
}

void LineEdit::changeCursorPosition() {
    if (cursorPosition() == 0)
        setCursorPosition(1);
}

void LineEdit::resizeEvent(QResizeEvent *event) {
    QFont font = this->font();
    font.setPixelSize(this->height() * 0.65);
    this->setFont(font);

    this->update();
}
