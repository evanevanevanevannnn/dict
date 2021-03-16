#include "newwordwidget.h"

NewWordWidget::NewWordWidget(DictWidget *Dict, QWidget *parent)
    : QWidget(parent), dict(Dict) {

    timer = new QTimer(this);

    original = new LineEdit(this);
    translation = new LineEdit(this);

    submit = new QPushButton("submit", this);

    QSizePolicy policy;
    policy.setVerticalPolicy(QSizePolicy::Ignored);
    policy.setHorizontalPolicy(QSizePolicy::Ignored);

    original->setSizePolicy(policy);
    translation->setSizePolicy(policy);
    submit->setSizePolicy(policy);

    QGridLayout *lay = new QGridLayout(this);

    lay->addWidget(new QWidget(this), 0, 0, 2, 1);
    lay->addWidget(new QWidget(this), 5, 4, 2, 1);

    lay->addWidget(original, 2, 1, 1, 3);
    lay->addWidget(translation, 3, 1, 1, 3);
    lay->addWidget(submit, 4, 1, 1, 3);

    connect(original, &LineEdit::returnPressed, [this]{this->translation->setFocus();});
    connect(translation, &LineEdit::returnPressed, this, &NewWordWidget::submitWord);

    connect(submit, &QPushButton::pressed, this, &NewWordWidget::submitWord);
    connect(timer, &QTimer::timeout, this, &NewWordWidget::format);
}

void NewWordWidget::clear() {
    original->setText("");
    translation->setText("");
}

void NewWordWidget::setLineEditFocus() {
    original->setFocus();
}

void NewWordWidget::submitWord() {
    QString Original = original->text(),
            Translation = translation->text();

    original->setReadOnly(true);
    translation->setReadOnly(true);

    QPalette palette;
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Base, Qt::green);

    original->setPalette(palette);
    translation->setPalette(palette);

    timer->start(500);

    dict->addWord(Original, Translation);
    emit wordAdded();
}

void NewWordWidget::format() {
    original->setText("");
    translation->setText("");

    original->setReadOnly(false);
    translation->setReadOnly(false);

    QPalette palette;
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Base, Qt::white);

    original->setPalette(palette);
    translation->setPalette(palette);

    timer->stop();

    original->setFocus();
}

void NewWordWidget::resizeEvent(QResizeEvent *event) {
    QFont font = original->font();
    font.setPixelSize(original->height() * 0.65);

    original->setFont(font);
//    translation->setFont(font);

    font.setPixelSize(original->height() * 0.5);
    submit->setFont(font);
}
