#ifndef REGEXPSTRING_H
#define REGEXPSTRING_H

#include <QString>

class RegExpString
{
private:
    QString Int;
    QString Fraction;
    QString Double;

public:
    RegExpString();

    QString INT() { return Int; }
    QString FRACTION() { return Fraction; }
    QString DOUBLE() { return Double; }
};

#endif // REGEXPSTRING_H
