#include "factor.h"

Factor::Factor()
{
    numerator = 0;
    denominator = 1;
}

Factor::Factor(qint64 _numerator, qint64 _denominator)
{
    if( !_denominator )
        return;

    numerator = _numerator;
    denominator = _denominator;

    Reducing();
}

Factor::Factor(qint64 number)
{
    numerator = number;
    denominator = 1; 
}

Factor::Factor(QString str)
{
    /*QString rate = "";
    int i = 0;

    if( str[i] == '-' )
        rate.append(str[i++]);

    while( str[i].isDigit() )
        rate.append(str[i++]);

    numerator = rate.toInt();
    denominator = 1;

    if( str[i++] == '/' )
    {
        rate.clear();

        while( str[i].isDigit() )
            rate.append(str[i++]);

        denominator = rate.toInt();
    }

    Reducing();*/

    /*for( int i = 0; i < 5; i++ )
    {
        number *= 10;
        denominator *= 10;
    }

    numerator = (qint64)number;*/

    RegExpString Exp;

    QRegExp Int(Exp.INT());
    QRegExp Fraction(Exp.FRACTION());
    QRegExp Double(Exp.DOUBLE());

    if(Int.exactMatch(str))
    {
        numerator = str.toInt();
        denominator = 1;
    }

    if(Fraction.exactMatch(str))
    {
        QString tmp = str.mid(0, str.indexOf("/"));
        numerator = tmp.toInt();

        tmp = str.mid(str.indexOf("/") + 1, str.length() - ( str.indexOf("/") + 1 ) );
        denominator = tmp.toInt();
    }

    if(Double.exactMatch(str))
    {
        double num = str.toDouble();

        for( int i = 0; i < 5; i++ )
        {
            num *= 10;
            denominator *= 10;
        }

        numerator = (qint64)num;
    }

    Reducing();
}

qint64 Factor::NOD(qint64 num1, qint64 num2)
{
    num1 = num1 > 0 ? num1 : -num1;
    num2 = num2 > 0 ? num2 : -num2;

    while(num1 != num2)
        if(num1 > num2)
            num1 = num1 - num2;
        else
            num2 = num2 - num1;

    return num1;
}

void Factor::Reducing()
{
    if(numerator % denominator)
    {
        qint64 nod = NOD(numerator, denominator);

        numerator = numerator / nod;
        denominator = denominator / nod;
    }
    else
    {
        numerator = numerator / denominator;
        denominator = 1;
    }

    CheckSign();
}

void Factor::CheckSign()
{
    int signN = 1, signD = 1;

    if(numerator < 0)
    {
        numerator = -numerator;
        signN = -1;
    }

    if(denominator < 0)
    {
        denominator = -denominator;
        signD = -1;
    }

    int sign = signN * signD;

    numerator *= sign;
}

QString Factor::toString()
{
    if(denominator != 1)
        return QString("%1/%2").arg(numerator).arg(denominator);
    else
        return QString("%1").arg(numerator);
}

Factor Factor::operator - ()
{
    return Factor(-numerator, denominator);
}

Factor Factor::operator + (Factor tmp)
{
    qint64 numer1 = this->numerator;
    qint64 denom1 = this->denominator;

    qint64 numer2 = tmp.numerator;
    qint64 denom2 = tmp.denominator;

    qint64 denomN = denom1 * ( denom2 / NOD(denom1, denom2) );

    qint64 numerN = numer1 * ( denomN / denom1 ) +
                     numer2 * ( denomN / denom2 );

    return Factor(numerN, denomN);
}

Factor Factor::operator - (Factor tmp)
{
    qint64 numer1 = this->numerator;
    qint64 denom1 = this->denominator;

    qint64 numer2 = tmp.numerator;
    qint64 denom2 = tmp.denominator;

    qint64 denomN = denom1 * ( denom2 / NOD(denom1, denom2) );

    qint64 numerN = numer1 * ( denomN / denom1 ) -
                     numer2 * ( denomN / denom2 );

    return Factor(numerN, denomN);
}

Factor Factor::operator * (Factor tmp)
{
    return Factor(this->numerator * tmp.numerator,
                  this->denominator * tmp.denominator);
}

Factor Factor::operator / (Factor tmp)
{
    return Factor(this->numerator * tmp.denominator,
                  this->denominator * tmp.numerator);
}

bool Factor::operator == (Factor tmp)
{
    if( this->numerator != tmp.numerator )
        return false;

    if( this->denominator != tmp.denominator )
        return false;

    return true;
}

bool Factor::operator != (Factor tmp)
{
    return !( (*this) == tmp );
}

bool Factor::operator == (int num)
{
    return numerator == num * denominator;
}

bool Factor::operator != (int num)
{
    return numerator != num * denominator;
}

bool Factor::operator > (int num)
{
    return numerator > num * denominator;
}

bool Factor::operator >= (int num)
{
    return numerator >= num * denominator;
}

bool Factor::operator < (int num)
{
    return numerator < num * denominator;
}

bool Factor::operator <= (int num)
{
    return numerator <= num * denominator;
}

bool Factor::operator < (Factor tmp)
{
    return ((*this) - tmp) < 0;
}

bool Factor::operator <= (Factor tmp)
{
    return ((*this) - tmp) <= 0;
}

Factor Factor::operator ^(int power)
{
    if(power == 0)
        return Factor(1);

    qint64 new_numerator = numerator;
    qint64 new_denominator = denominator;

    if(power < 0)
    {
        new_numerator = denominator;
        new_denominator = numerator;

        power = -power;
    }

    for(int i = 1; i < power; i++)
    {
        new_numerator *= new_numerator;
        new_denominator *= new_denominator;
    }

    return Factor(new_numerator, new_denominator);
}
