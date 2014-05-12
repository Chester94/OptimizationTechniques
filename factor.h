#ifndef FACTOR_H
#define FACTOR_H

#include <QTypeInfo>
#include <QString>
#include <qmath.h>

#include "QDebug"
#include "regexpstring.h"

// ����� ��� ������ � �������������� ��������� � �������.
// ������ ����������� � ���� ����� (������������, �� ����������� ����������).
// ���� �������� � ���������

// ��������� ������
// ��������� � ����������� �������� � ���� int, ���������� �����������,
// ��� ��� ������ ������������ � ������������ ���������� ���������� ������������,
// ���� �������� ��� ������� ����� ��� ������� �������.
//
// ��������� ���������
// �������� ��� ������ �� int64
//
// ��������� �������
//
//
//
// ��������� ������
// ��������� ������������ ������ ��� ������� ��������� ����������,
// �������� � ������������, ��� ��������� ��� �������.
//
// ��������� ���������
// �������� ����������� �� ������������ (�� ����� - �������)

class Factor
{
private:
    qint64 numerator; // ���������
    qint64 denominator; // �����������

    // ���������� ����� �������� ���� �����. ���������: ��� �����
    qint64 NOD(qint64 num1, qint64 num2);

    // ���������, ��� ���� ���������� ������ � ���������
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

    // ���������� ���������� �����, ���� ��� ��������.
    // ���������: ���������, �����������
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
