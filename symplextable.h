#ifndef SYMPLEXTABLE_H
#define SYMPLEXTABLE_H

#include "factor.h"

enum TypeSolution {REAL_SOLUTION, INF_SOLUTION, NOT_SOLUTION};

class SymplexTable
{
private:
    Factor **table;

    int rowCount;
    int columnCount;

    QList<int> VerticalHeader;
    QList<int> HorizontalHeader;

    int realValueCount;

    void switchHeaderLabels(int verHeadIndex, int horHeadIndex);

    void deleteColumn(int columnIndex);

    void deleteTable();

public:
    SymplexTable();
    ~SymplexTable();

    SymplexTable(const SymplexTable &tmp);

    void setValues(Factor **values, int row, int column, int _realValueCount);
    Factor** getValues() { return table; }

    int getRowCount() {return rowCount;}
    int getColumnCount() {return columnCount;}

    QList<int> getVerticalHeader() { return VerticalHeader; }
    QList<int> getHorizontalHeader() { return HorizontalHeader; }

    void setVerticalHeader(QList<int> verHeader) { VerticalHeader = verHeader; }
    void setHorizontalHeader(QList<int> horHeader) { HorizontalHeader = horHeader; }

    void createVerticalHeader();
    void createHorizontalHeader();

    void createVerticalHeader(QList<int> selectedValues);
    void createHorizontalHeader(QList<int> selectedValues);

    int isSolution();
    bool isEndOfArtificalBasis();

    bool checkPermissibleSolution();

    QList<int> searchSupportElements();
    QList<int> searchSupportElementsForArtificalBasis();

    void nextStep(int rowIndex, int columnIndex);

    bool zeroLastRow();
    void computeLastRow(Factor *func);

    void operator = (const SymplexTable &tmp);

    Factor getSolution(Factor *func) { return func[realValueCount] - table[rowCount-1][columnCount-1]; }
    QString getPointSolution();

    Factor* operator [] (int n) { return table[n]; }
};

#endif // SYMPLEXTABLE_H
