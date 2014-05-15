#include "symplextable.h"

SymplexTable::SymplexTable()
{
    table = NULL;
}

SymplexTable::~SymplexTable()
{
    deleteTable();
}

SymplexTable::SymplexTable(const SymplexTable &tmp)
{
    VerticalHeader = tmp.VerticalHeader;
    HorizontalHeader = tmp.HorizontalHeader;

    rowCount = tmp.rowCount;
    columnCount = tmp.columnCount;

    table = new Factor* [rowCount];
    for( int i = 0; i < rowCount; i++ )
        table[i] = new Factor [columnCount];

    for( int i = 0; i < rowCount; i++ )
        for( int j = 0; j < columnCount; j++ )
            table[i][j] = tmp.table[i][j];
}

void SymplexTable::switchHeaderLabels(int verHeadIndex, int horHeadIndex)
{
    int ver = VerticalHeader[verHeadIndex];
    int hor = HorizontalHeader[horHeadIndex];

    VerticalHeader[verHeadIndex] = hor;
    HorizontalHeader[horHeadIndex] = ver;
}

void SymplexTable::deleteColumn(int columnIndex)
{
    Factor **tmp;

    tmp = new Factor* [rowCount];
    for( int i = 0; i < rowCount; i++ )
        tmp[i] = new Factor [columnCount-1];

    for( int i = 0; i < rowCount; i++ )
        for( int j = 0, k = 0; j < columnCount; j++ )
            if( j != columnIndex )
                tmp[i][k++] = table[i][j];

    deleteTable();

    table = tmp;

    columnCount--;

    HorizontalHeader.removeAt(columnIndex);
}

void SymplexTable::deleteTable()
{
    if(table)
    {
        for( int i = 0; i < rowCount; i++ )
            delete table[i];

        delete table;
    }
}

void SymplexTable::setValues(Factor **values, int row, int column, int _realValueCount)
{
    if(table)
        delete table;

    table = values;

    rowCount = row;
    columnCount = column;

    realValueCount = _realValueCount;
}

void SymplexTable::createVerticalHeader()
{
    VerticalHeader.clear();
    for( int i = 0; i < rowCount-1; i++ )
        VerticalHeader << columnCount+i;
}

void SymplexTable::createHorizontalHeader()
{
    HorizontalHeader.clear();
    for( int i = 0; i < columnCount-1; i++ )
        HorizontalHeader << i+1;
}

void SymplexTable::createVerticalHeader(QList<int> selectedValues)
{
    VerticalHeader.clear();
    for( int i = 0; i < selectedValues.length(); i++ )
        if( selectedValues[i] == 1 )
            VerticalHeader << i+1;
}

void SymplexTable::createHorizontalHeader(QList<int> selectedValues)
{
    HorizontalHeader.clear();
    for( int i = 0; i < selectedValues.length(); i++ )
        if( selectedValues[i] == 0 )
            HorizontalHeader << i+1;
}

int SymplexTable::isSolution()
{
    bool realSol = true;

    for(int i = 0; i < columnCount-1; i++)
        if(table[rowCount-1][i] < 0)
        {
            realSol = false;
            break;
        }

    if(realSol)
        return REAL_SOLUTION;

    for(int i = 0; i < columnCount-1; i++)
    {
        if(table[rowCount-1][i] < 0)
        {
            bool infSol = true;
            for(int j = 0; j < rowCount-1; j++)
                if(table[j][i] > 0)
                {
                    infSol = false;
                    break;
                }

            if(infSol)
                return INF_SOLUTION;
        }
    }

    return NOT_SOLUTION;
}

bool SymplexTable::isEndOfArtificalBasis()
{
    for(QList<int>::iterator i = VerticalHeader.begin(); i != VerticalHeader.end(); i++)
        if( *i > realValueCount )
            return false;

    return true;
}

bool SymplexTable::checkPermissibleSolution()
{
    for( int i = 0; i < rowCount-1; i++ )
        if( table[i][columnCount-1] < 0 )
            return false;

    return true;
}

QList<int> SymplexTable::searchSupportElements()
{
    QList<int> elements;

    for(int i = 0; i < columnCount-1; i++)
    {
        QList<int> curSupEl;
        curSupEl.clear();

        if(table[rowCount-1][i] < 0)
            for(int j = 0; j < rowCount-1; j++)
                if( table[j][i] > 0 )
                    if( curSupEl.isEmpty() )
                    {
                        curSupEl.append(j);
                        curSupEl.append(i);
                    }
                    else
                        if( table[j][columnCount-1] / table[j][i] ==
                                table[curSupEl[0]][columnCount-1] / table[curSupEl[0]][curSupEl[1]] )
                        {
                            curSupEl.append(j);
                            curSupEl.append(i);
                        }
                        else
                            if( table[j][columnCount-1] / table[j][i] <
                                    table[curSupEl[0]][columnCount-1] / table[curSupEl[0]][curSupEl[1]] )
                            {
                                curSupEl.clear();

                                curSupEl.append(j);
                                curSupEl.append(i);
                            }

        elements.append(curSupEl);
    }

    return elements;
}

QList<int> SymplexTable::searchSupportElementsForArtificalBasis()
{
    QList<int> elements = searchSupportElements();

    if( elements.length() == 0 )
        for( int i = 0; i < rowCount-1; i++ )
            if( VerticalHeader[i] > realValueCount )
                for( int j = 0; j < columnCount-1; j++ )
                    if( table[i][j] != 0 && table[i][columnCount-1] == 0)
                    {
                        elements.append(i);
                        elements.append(j);
                    }

    int i;
    for(i = 0; i < rowCount-1; i++ )
        if( VerticalHeader[i] > realValueCount )
            break;

    for( int j = 0; j < elements.length(); j+=2 )
        if(elements[j] == i)
        {
            elements.move(j, 0);
            elements.move(j+1, 1);

            break;
        }

    return elements;
}

void SymplexTable::nextStep(int rowIndex, int columnIndex)
{
    for(int i = 0; i < columnCount; i++)
        if( i != columnIndex )
            table[rowIndex][i] = table[rowIndex][i] / table[rowIndex][columnIndex];

    for( int i = 0; i < rowCount; i++ )
        if( i != rowIndex )
        {
            Factor tmp = table[i][columnIndex];

            for( int j = 0; j < columnCount; j++ )
                if( j != columnIndex )
                    table[i][j] = table[i][j] - tmp * table[rowIndex][j];
        }

    for(int i = 0; i < rowCount; i++)
    {
        if( i != rowIndex )
            table[i][columnIndex] = table[i][columnIndex] / -table[rowIndex][columnIndex];
    }

    table[rowIndex][columnIndex] = table[rowIndex][columnIndex] ^ -1;

    switchHeaderLabels(rowIndex, columnIndex);

    if( HorizontalHeader[columnIndex] > realValueCount )
        deleteColumn(columnIndex);
}

bool SymplexTable::zeroLastRow()
{
    for( int i = 0; i < columnCount; i++ )
        if(table[rowCount-1][i] != 0)
            return false;

    return true;
}

void SymplexTable::computeLastRow(Factor *func)
{
    for( int i = 0; i < columnCount; i++ )
    {
        if( i != columnCount-1 )
            table[rowCount-1][i] = func[ HorizontalHeader[i] - 1 ];

        for( int j = 0; j < rowCount-1; j++ )
            if( i != columnCount-1 )
                table[rowCount-1][i] = table[rowCount-1][i] -
                        table[j][i] * func[ VerticalHeader[j] - 1 ];
            else
                table[rowCount-1][i] = table[rowCount-1][i] +
                        table[j][i] * func[ VerticalHeader[j] - 1 ];
    }

    table[rowCount-1][columnCount-1] = -table[rowCount-1][columnCount-1];
}

void SymplexTable::operator = (const SymplexTable &tmp)
{
    deleteTable();

    VerticalHeader = tmp.VerticalHeader;
    HorizontalHeader = tmp.HorizontalHeader;

    rowCount = tmp.rowCount;
    columnCount = tmp.columnCount;

    table = new Factor* [rowCount];
    for( int i = 0; i < rowCount; i++ )
        table[i] = new Factor [columnCount];

    for( int i = 0; i < rowCount; i++ )
        for( int j = 0; j < columnCount; j++ )
            table[i][j] = tmp.table[i][j];
}

QString SymplexTable::getPointSolution()
{
    QList<Factor> pointSolution;

    for( int i = 0; i < realValueCount; i++ )
        pointSolution.append(Factor(0));

    for( int i = 0; i < VerticalHeader.length(); i++ )
        pointSolution[VerticalHeader[i]-1] = table[i][columnCount - 1];

    QString sol = "( ";
    int i;

    for( i = 0; i < pointSolution.length()-1; i++ )
        sol += pointSolution[i].toString() + ", ";

    sol += pointSolution[i].toString() + " ) ";

    return sol;
}
