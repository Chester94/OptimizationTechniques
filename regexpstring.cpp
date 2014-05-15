#include "regexpstring.h"

RegExpString::RegExpString()
{
    Int = "\\-{,1}[1-9][0-9]{,4}|0";
    Fraction = "(\\-{,1}[1-9][0-9]{,4}|0)/[1-9][0-9]{,4}";
    Double = "(\\-{,1}([1-9][0-9]{,4}|0))\\.[0-9]{1,5}";
}
