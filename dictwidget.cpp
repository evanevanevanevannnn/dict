#include "dictwidget.h"

#include <iostream>
#include <fstream>
#include <string>

Word::Word() {}

Word::Word(QString Original, QString Translation, int SinceLastTest): translation(Original), original(Translation), sinceLastTest(SinceLastTest) {}

DictWidget::DictWidget(QString FileName): fileName(FileName) {
    using namespace std;

    words.clear();

    ifstream fin(fileName.toStdString());
    string str;

    while (getline(fin, str)) {
        QStringList fields = QString::fromStdString(str).split(",", QString::SkipEmptyParts);

        Word newWord(fields.at(0), fields.at(1), fields.at(2).toInt());
        words.push_back(newWord);
    }

    fin.close();
}

void DictWidget::addWord(QString Original, QString Translation) {
    words.push_back(Word(Original, Translation, 1e6));
}

void DictWidget::updateWordsStats() {
    for (int i = 0; i < words.size(); ++i)
        words[i].sinceLastTest++;

    for (int i = 0; i < lastTested.size(); ++i)
        words[lastTested[i]].sinceLastTest = (accuracy[i] ? 0 : 1e6);
}

QVector <Word> DictWidget::testDict(int time, QVector <int> *&res) {
    lastTested.clear();
    accuracy.clear();

    QVector <Word> dict;
    for (int i = 0; i < words.size(); ++i) {
        if (words[i].sinceLastTest >= time) {
            lastTested.push_back(i);
            dict.push_back(words[i]);
        }
    }

    accuracy.resize(dict.size());

    for (int i = 0; i < 1e6; ++i) {
        int ind1 = rand() % dict.size(),
            ind2 = rand() % dict.size();

        Word tmp = dict[ind1];
        dict[ind1] = dict[ind2];
        dict[ind2] = tmp;
    }

    res = &accuracy;
    return dict;
}

DictWidget::~DictWidget() {
    std::ofstream fout(fileName.toStdString());

    for (auto word : words)
        fout << word.original.toStdString() << ","
             << word.translation.toStdString() << ","
             << word.sinceLastTest << "\n";

    fout.close();
}

void swap(int &a, int &b) {
    int t = a;
    a = b;
    b = t;
}

QVector <int> DictWidget::timePeriods() {
    QSet <int> st;
    for (int i = 0; i < words.size(); ++i)
        st.insert(words[i].sinceLastTest);

    st.insert(0);
    st.insert(1e6);

    QVector <int> res;
    for (auto i : st)
        res.push_back(i);

    for (int i = 0; i < res.size(); ++i)
        for (int j = i; j < res.size(); ++j)
            if (res[i] > res[j])
                swap(res[i], res[j]);

    return res;
}

int DictWidget::wordsTimeCount(int time) {
    int res = 0;
    for (auto word : words)
        res += (word.sinceLastTest >= time);

    return res;
}
