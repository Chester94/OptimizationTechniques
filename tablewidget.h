#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>

#include "mydelegat.h"
#include "factor.h"

class TableWidget : public QTableWidget
{
private:
    //QStringList VerticalHeader;
    //QStringList HorizontalHeader;

    void setZeroToAllCell();

    //QStringList createMyVerticalHeader(QString nameVar = "x");
    //QStringList createMyHorizontalHeader(QString nameVar = "x");

    //QStringList createMyVerticalHeaderForArtificalBasis(QString nameVar);

public:
    TableWidget(QWidget *parent);

    void setDimension(int row, int column);

    void setValues(int row, int column, Factor** values);
    Factor** getValues();

    void setMyVerticalHeader();
    void setMyHorizontalHeader();

    void setMyVerticalHeader(QList<int> val);
    void setMyHorizontalHeader(QList<int> val);

    //void setMyVerticalHeaderForArtificalBasis(QString nameVar);

    void addRow();
    void deleteRow();

    void addColumn();
    void deleteColumn();

    //void switchHeaderLabels(int verHeadIndex, int horHeadIndex);

    void selectSupportElemtnts(QList<int> supEl);

    void clearAllocation();
};

#endif // TABLEWIDGET_H
