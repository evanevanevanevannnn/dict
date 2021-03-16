#ifndef DICTWIDGET_H
#define DICTWIDGET_H

#include <QtWidgets>

struct Word {
    QString original, translation;
    int sinceLastTest;

    Word();
    Word(QString Original, QString Translation, int SinceLastTest);
};

class DictWidget
{
public:
    explicit DictWidget(QString FileName);
    ~DictWidget();

    void addWord(QString Original, QString Translation);
    QVector <Word> testDict(int time, QVector <int> *&res);

    void updateWordsStats();

    QVector <int> timePeriods();
    int wordsTimeCount(int time);

private:
    QVector <Word> words;
    QString fileName;

    QVector <int> lastTested, accuracy;
};

#endif // DICTWIDGET_H
