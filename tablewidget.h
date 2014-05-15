#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>

#include "mydelegat.h"
#include "factor.h"
#include "symplextable.h"

class TableWidget : public QTableWidget
{
private:
    void setZeroToAllCell();

public:
    TableWidget(QWidget *parent);

    void setDimension(int row, int column);

    void setValues(int row, int column, Factor** values);

    Factor** getValues();

    void setMyVerticalHeader();
    void setMyHorizontalHeader();

    void setMyVerticalHeader(QList<int> val);
    void setMyHorizontalHeader(QList<int> val);

    void addRow();
    void deleteRow();

    void addColumn();
    void deleteColumn();

    void selectSupportElemtnts(QList<int> supEl);

    void clearAllocation();

    void toGaussMethod(int column);
    QList<int> selectedValues();
};

#endif // TABLEWIDGET_H
