#include "regexpstring.h"

RegExpString::RegExpString()
{
    //Int = "\\-{,1}[1-9][0-9]{,4}|0";

    QString NotZeroInt = "(\\-{,1}[1-9][0-9]{,4})";
    QString DecimalPart = "([0-9]{1,5})";

    /*Int = "(" + NotZeroInt + "|[0]{1,1}" + ")";
    Fraction = Int + "/" + NotZeroInt;
    Double = Int + "\\." + DecimalPart;*/

    Int = "\\-{,1}[1-9][0-9]{,4}|0";
    Fraction = "(\\-{,1}[1-9][0-9]{,4}|0)/[1-9][0-9]{,4}";
    Double = "(\\-{,1}([1-9][0-9]{,4}|0))\\.[0-9]{1,5}";
}
