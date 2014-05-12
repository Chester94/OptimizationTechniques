#ifndef MYDELEGAT_H
#define MYDELEGAT_H

#include <QItemDelegate>
#include <QLineEdit>

#include <QRegExp>
#include <QRegExpValidator>

#include "regexpstring.h"

class MyDelegat : public QItemDelegate
{
public:
    MyDelegat();

    QWidget * createEditor( QWidget * parent, const QStyleOptionViewItem & option,
                            const QModelIndex & index ) const;
};

#endif // MYDELEGAT_H
