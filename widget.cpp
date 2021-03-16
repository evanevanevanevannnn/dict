#include "widget.h"

#include <iostream>

Widget::Widget(QWidget *parent)
    : QWidget(parent) {

    QColor lightBlue(184, 217, 240),
           classicBlue(15, 76, 129);

    dictWidget = new DictWidget("C:\\Users\\sukho\\Documents\\qt\\dict\\words.txt");

    QGridLayout *lay = new QGridLayout(this);

    QSizePolicy policy;
    policy.setVerticalPolicy(QSizePolicy::Ignored);
    policy.setHorizontalPolicy(QSizePolicy::Ignored);

    testWidget = new TestWidget(dictWidget);
    newWordWidget = new NewWordWidget(dictWidget);

    testWidget->setHidden(true);

    testWidget->setSizePolicy(policy);
    newWordWidget->setSizePolicy(policy);

    //спэйсинг
    lay->addWidget(new QWidget(this), 0, 0);
    lay->addWidget(new QWidget(this), 0, 17);
    lay->addWidget(new QWidget(this), 0, 22);
    lay->addWidget(new QWidget(this), 16, 0);

    lay->addWidget(testWidget, 1, 1, 15, 16);
    lay->addWidget(newWordWidget, 1, 1, 15, 16);

    addWord = new QPushButton("add word");
    test = new QPushButton("test");

    addWord->setSizePolicy(policy);
    test->setSizePolicy(policy);

    QWidget *boxWidget = new QWidget(this),
            *boxWidget2 = new QWidget(this);

    QPalette palette;
    palette.setColor(QPalette::Window, classicBlue);

    boxWidget->setPalette(palette);
    boxWidget->setAutoFillBackground(true);

    newWordWidget->setPalette(palette);
    newWordWidget->setAutoFillBackground(true);

    testWidget->setPalette(palette);
    testWidget->setAutoFillBackground(true);

    palette.setColor(QPalette::Window, lightBlue);

    this->setPalette(palette);
    this->setAutoFillBackground(true);

    QVBoxLayout *boxLay = new QVBoxLayout(boxWidget);

    boxWidget->setSizePolicy(policy);
    boxWidget2->setPalette(palette);
    boxWidget2->setAutoFillBackground(true);

    QVBoxLayout *box2Lay = new QVBoxLayout(boxWidget2);

    dictStats = new QLabel(" tm: \n am: ");
    timeSlider = new QSlider(Qt::Horizontal);

    dictStats->setSizePolicy(policy);

    palette.setColor(QPalette::Window, Qt::white);

    dictStats->setPalette(palette);
    dictStats->setAutoFillBackground(true);

    timeSlider->setMinimum(0);
    timeSlider->setMaximum(timePeriods.size() - 1);

    box2Lay->addWidget(dictStats, 5);
    box2Lay->addWidget(timeSlider, 2);
    box2Lay->addWidget(new QWidget(this), 1);
    box2Lay->addWidget(test, 5);

    boxLay->addWidget(addWord, 5);
    boxLay->addWidget(new QWidget(this), 1);
    boxLay->addWidget(boxWidget2, 15);

    lay->addWidget(boxWidget, 1, 18, 10, 4);

    connect(addWord, &QPushButton::clicked, this, &Widget::AddWord);
    connect(test, &QPushButton::clicked, this, &Widget::Test);

    connect(timeSlider, &QSlider::valueChanged, this, &Widget::sliderChange);

    connect(testWidget, &TestWidget::testFinished, this, &Widget::updateSlider);
    connect(newWordWidget, &NewWordWidget::wordAdded, this, &Widget::updateSlider);

    updateSlider();

    this->show();
    this->resize(1100, 600);
}

void Widget::Test() {
    newWordWidget->setHidden(true);
    testWidget->setVisible(true);

    int time = timePeriods[timeSlider->value()];
    testWidget->test(time);
}

void Widget::AddWord() {
    testWidget->setHidden(true);
    newWordWidget->setVisible(true);

    newWordWidget->clear();
    newWordWidget->setLineEditFocus();
}

void Widget::updateSlider() {
    timePeriods = dictWidget->timePeriods();

    timeSlider->setMinimum(0);
    timeSlider->setMaximum(timePeriods.size() - 1);

    timeSlider->setValue(0);

    sliderChange();
}

void Widget::sliderChange() {
    QString text = dictStats->text();
    int timePeriod = timePeriods[timeSlider->value()];

    for (int i = 0; i < text.size(); ++i) {
        if (text[i] == ':') {
            for (int j = i; j < text.size(); ++j) {
                if (text[j] == '\n') {
                    text = text.left(i + 1) + (timePeriod == 1e6 ? QString("new") : QString::number(timePeriod)) + text.right(text.size() - j);
                    break;
                }
            }
            break;
        }
    }

    for (int i = text.size() - 1; i >= 0; --i) {
        if (text[i] == ':') {
            text = text.left(i + 1) + QString::number(dictWidget->wordsTimeCount(timePeriods[timeSlider->value()]));
            break;
        }
    }

    dictStats->setText(text);
}

void Widget::resizeEvent(QResizeEvent *event) {
    QFont font = addWord->font();
    font.setPixelSize(addWord->height() * 0.5);

    addWord->setFont(font);
    test->setFont(font);
    timeSlider->setFont(font);

    font.setPixelSize(dictStats->height() * 0.3);
    dictStats->setFont(font);

    this->update();
}

void Widget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == 'S' and event->modifiers() == Qt::CTRL and testWidget->isVisible())
        testWidget->showHint();
    else if (event->key() == 'T' and event->modifiers() == Qt::CTRL)
        AddWord();
}

Widget::~Widget() {
    delete dictWidget;
}

