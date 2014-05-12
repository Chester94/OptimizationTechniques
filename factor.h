#ifndef FACTOR_H
#define FACTOR_H

#include <QTypeInfo>
#include <QString>
#include <qmath.h>

#include "QDebug"
#include "regexpstring.h"

// Класс для работы с коэффициентами уравнения и условий.
// Хранит коэффициент в виде дроби (несократимой, не обязательно правильной).
// Знак хранится в числителе

// ВОЗМОЖНАЯ ОШИБКА
// Числитель и знаменатель хранятся в типе int, существует вероятность,
// что при работе конструктора с вещественным параметром произойдет переполнение,
// если параметр был слишком точен или слишком большим.
//
// ВОЗМОЖНОЕ УЛУЧШЕНИЕ
// Изменить тип данных на int64
//
// УЛУЧШЕНИЕ СДЕЛАНО
//
//
//
// ВОЗМОЖНАЯ ОШИБКА
// Ожидается некорректная работа при больших значениях числителей,
// возможно и знаменателей, при умножении или делении.
//
// ВОЗМОЖНОЕ УЛУЧШЕНИЕ
// Наложить ограничения на коэффициенты (по факту - костыль)

class Factor
{
private:
    qint64 numerator; // числитель
    qint64 denominator; // знаменатель

    // наибольший общий делитель двух чисел. аргументы: два числа
    qint64 NOD(qint64 num1, qint64 num2);

    // Проверяет, что знак содержится именно в числителе
    void CheckSign();

public:
    Factor();
    Factor(qint64 _numerator, qint64 _denominator);
    Factor(qint64 number);
    Factor(QString str);

    qint64 getNumerator() {return numerator;}
    qint64 getDenominator() {return denominator;}
    double getNumberInDecemalForm() {return (double)(numerator) / denominator;}

    void setNumerator(qint64 _numerator) {numerator = _numerator;}
    void setDenominator(qint64 _denominator) {denominator = _denominator;}

    // производит сокращение дроби, если это возможно.
    // аргументы: числитель, знаменатель
    void Reducing();

    QString toString();

    Factor operator - ();

    Factor operator + (Factor tmp);
    Factor operator - (Factor tmp);
    Factor operator * (Factor tmp);
    Factor operator / (Factor tmp);

    bool operator == (Factor tmp);
    bool operator != (Factor tmp);

    bool operator == (int num);
    bool operator != (int num);

    bool operator > (int num);
    bool operator >= (int num);

    bool operator < (int num);
    bool operator <= (int num);

    bool operator < (Factor tmp);
    bool operator <= (Factor tmp);

    Factor operator ^ (int power);
};

#endif // FACTOR_H
