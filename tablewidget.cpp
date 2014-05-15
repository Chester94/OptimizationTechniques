#include "tablewidget.h"

TableWidget::TableWidget(QWidget *parent)
{
    setParent(parent);
    setItemDelegate(new MyDelegat());

    setDimension(3, 4);

    setMyVerticalHeader();
    setMyHorizontalHeader();
}

void TableWidget::setDimension(int row, int column)
{
    setColumnCount(column);
    setRowCount(row);

    setZeroToAllCell();
}

void TableWidget::setValues(int row, int column, Factor **values)
{
    /*setColumnCount(column);
    setRowCount(row);

    setZeroToAllCell();*/
    setDimension(row, column);

    for( int i = 0; i < row; i++ )
        for( int j = 0; j < column; j++ )
            item(i, j)->setText( values[i][j].toString() );
}

Factor** TableWidget::getValues()
{
    Factor **values;

    values = new Factor* [rowCount()];
    for(int i = 0; i < rowCount(); i++)
        values[i] = new Factor [columnCount()];

    for( int i = 0; i < rowCount(); i++ )
        for( int j = 0; j < columnCount(); j++ )
            values[i][j] = Factor(item(i, j)->text());

    return values;
}

void TableWidget::setZeroToAllCell()
{
    for(int i = 0; i < rowCount(); i++)
        for(int j = 0; j < columnCount(); j++)
        {
            model()->setData( model()->index(i, j), QVariant(0) );
            item(i, j)->setText("0");
        }
}

void TableWidget::setMyVerticalHeader()
{
    QStringList verHeader;

    verHeader << "f(x)";
    for(int i = 1; i < rowCount(); i++)
        verHeader << QString("c%1").arg(i);

    setVerticalHeaderLabels( verHeader );
}

void TableWidget::setMyHorizontalHeader()
{
    QStringList horHeader;

    for(int i = 0; i < columnCount()-1; i++)
        horHeader << QString("x%1").arg(i+1);
    horHeader << "const";

    setHorizontalHeaderLabels( horHeader );
}

void TableWidget::setMyVerticalHeader(QList<int> val)
{
    QStringList verHeader;

    for(int i = 0; i < val.length(); i++)
        verHeader << QString("x%1").arg(val[i]);
    verHeader << "*";

    setVerticalHeaderLabels( verHeader );
}

void TableWidget::setMyHorizontalHeader(QList<int> val)
{
    QStringList horHeader;

    for(int i = 0; i < val.length(); i++)
        horHeader << QString("x%1").arg(val[i]);
    horHeader << "const";

    setHorizontalHeaderLabels( horHeader );
}

void TableWidget::addRow()
{
    if(rowCount() > 16)
        return;

    if( rowCount() + 1 > columnCount() - 1 )
        return;

    setRowCount(rowCount() + 1);
    setZeroToAllCell();
    setMyVerticalHeader();
}

void TableWidget::deleteRow()
{
    if(rowCount() < 3)
        return;

    setRowCount(rowCount() - 1);
    setZeroToAllCell();
    setMyVerticalHeader();
}

void TableWidget::addColumn()
{
    if(columnCount() > 16)
        return;

    setColumnCount(columnCount() + 1);
    setZeroToAllCell();
    setMyHorizontalHeader();
}

void TableWidget::deleteColumn()
{
    if(columnCount() < 4)
        return;

    if( rowCount() >= columnCount() - 1 )
        deleteRow();

    setColumnCount(columnCount() - 1);
    setZeroToAllCell();
    setMyHorizontalHeader();
}

void TableWidget::selectSupportElemtnts(QList<int> supEl)
{
    clearAllocation();

    if(supEl.length() == 0)
        return;

    for( int i = 0; i < supEl.length(); i+=2)
    {
        item(supEl[i], supEl[i+1])->setFlags(item(supEl[i], supEl[i+1])->flags() |
                                   Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        item(supEl[i], supEl[i+1])->setBackgroundColor(Qt::green);
    }

    item(supEl[0], supEl[1])->setSelected(true);
}

void TableWidget::clearAllocation()
{
    for(int i = 0; i < rowCount(); i++)
        for(int j = 0; j < columnCount(); j++)
        {
            item(i, j)->setFlags(Qt::ItemIsEnabled);
            item(i, j)->setBackgroundColor(Qt::white);
        }

    clearSelection();
}

void TableWidget::toGaussMethod(int column)
{
    setDimension(1, column);

    for( int i = 0; i < column; i++ )
    {
        item(0, i)->setCheckState(Qt::Unchecked);
        item(0, i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item(0, i)->setText("");
        setColumnWidth(i, 40);
    }

    setRowHeight(0, 47);
}

QList<int> TableWidget::selectedValues()
{
    QList<int> selVal;

    for( int i = 0; i < columnCount(); i++ )
        if(item(0, i)->checkState() == Qt::Checked)
            selVal.append(1);
        else
            selVal.append(0);

    return selVal;
}
