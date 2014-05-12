#include "mydelegat.h"

MyDelegat::MyDelegat()
{
}

QWidget * MyDelegat::createEditor( QWidget * parent, const QStyleOptionViewItem & option,
                        const QModelIndex & index ) const
{
    QLineEdit *editor = new QLineEdit(parent);

    RegExpString Exp;

    QRegExpValidator *factorValidator =
            new QRegExpValidator(QRegExp (Exp.INT() + "|" +
                                          Exp.FRACTION()+ "|" +
                                          Exp.DOUBLE()));

    editor->setValidator(factorValidator);
    return editor;
}
