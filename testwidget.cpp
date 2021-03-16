#include "testwidget.h"

#include <iostream>
#include <fstream>

TestWidget::TestWidget(DictWidget *Dict, QWidget *parent)
    :QWidget(parent), dict(Dict) {

    finish = new QLabel(" that's all for now:) ", this);
    finish->setHidden(true);
    finish->setAlignment(Qt::AlignCenter);

    original = new QLabel(this);
    translation = new LineEdit(this);
    hint = new QLabel(this);

    help = new QPushButton("help", this);

    original->setAlignment(Qt::AlignCenter);
    hint->setAlignment(Qt::AlignCenter);

    QSizePolicy policy;
    policy.setVerticalPolicy(QSizePolicy::Ignored);
    policy.setHorizontalPolicy(QSizePolicy::Ignored);

    original->setSizePolicy(policy);
    translation->setSizePolicy(policy);
    hint->setSizePolicy(policy);
    help->setSizePolicy(policy);

    QGridLayout *lay = new QGridLayout(this);

    lay->addWidget(new QWidget(this), 0, 0, 2, 1);
    lay->addWidget(new QWidget(this), 5, 4, 2, 1);

    lay->addWidget(hint, 1, 1, 1, 3);
    lay->addWidget(original, 2, 1, 1, 3);
    lay->addWidget(translation, 3, 1, 1, 3);
    lay->addWidget(finish, 3, 1, 1, 3);
    lay->addWidget(help, 4, 1, 1, 3);

    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);

    original->setPalette(palette);
    original->setAutoFillBackground(true);

    finish->setPalette(palette);
    finish->setAutoFillBackground(true);

    hint->setPalette(palette);
    hint->setAutoFillBackground(true);

    connect(translation, &LineEdit::nextWord, this, &TestWidget::nextWord);
    connect(translation, &LineEdit::showWord, this, &TestWidget::showWord);
    connect(translation, &LineEdit::returnPressed, this, &TestWidget::submitWord);

    connect(help, &QPushButton::pressed, this, &TestWidget::showHint);
}

void TestWidget::showWord() {
    original->setVisible(true);
    translation->setVisible(true);
    help->setVisible(true);

    finish->setHidden(true);
    hint->setHidden(true);

    original->setText(words[index].original);
    translation->setText("");
    this->update();
}

void TestWidget::submitWord() {
    if (translation->isActive())
        return;

    QString Word = translation->text();
    translation->highlightAsnwer(Word == words[index].translation);
}

void TestWidget::nextWord() {
    index++;

    if (index == words.size())
        stop();
    else
        showWord();
}

void TestWidget::showHint() {
    hint->setText(words[index].translation);
    hint->setVisible(true);

    (*accuracy)[index] = false;

    translation->setFocus();
}

void TestWidget::stop() {
    original->setHidden(true);
    translation->setHidden(true);
    help->setHidden(true);
    hint->setHidden(true);

    finish->setVisible(true);

    index--;

    dict->updateWordsStats();
    emit testFinished();
}

void TestWidget::test(int time) {
    words = dict->testDict(time, accuracy);
    index = 0;

    for (int i = 0; i < words.size(); ++i)
        (*accuracy)[i] = true;

    translation->setFocus();

    if (words.size())
        showWord();
    else
        stop();
}

void TestWidget::resizeEvent(QResizeEvent *event) {
    QFont font = original->font();
    font.setPixelSize(original->height() * 0.65);

    original->setFont(font);
    translation->setFont(font);
    finish->setFont(font);
    hint->setFont(font);

    font.setPixelSize(original->height() * 0.5);
    help->setFont(font);

    this->update();
}
