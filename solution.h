#ifndef SOLUTION_H
#define SOLUTION_H

#include <QWidget>
#include <QMessageBox>

#include "symplextable.h"

enum DirectionSolution {MIN, MAX};

namespace Ui {
class Solution;
}

class Solution : public QWidget
{
    Q_OBJECT

private:
    Factor *func;
    SymplexTable symTable;

    QList<SymplexTable> listSymTable;
    QList<int> supElements;

    int direction;

    bool allVeryBad;

    void trainingNextStep();
    void pathSelection();

    void CheckFunctionDirection(int columnCount);
    void CheckCondition(int rowCount, int columnCount, Factor **values);

    void MessageBoxRealSolution();
    void MessageBoxInfSolution();
    void MessengeBoxError();
    void MessengeBoxGaussImpossible();
    void MessengeBoxGaussSolutionDontPermissible();

    void setFunc(int columnCount, Factor **values);
    Factor** setCondition(int rowCount, int columnCount, Factor **values);

    void deleteValues(Factor **values, int rowCount);

    Factor** gauss(int rowCount, int columnCount, Factor** val, QList<int> selectedValues);

    int searchIndexMatchedString(int begin, Factor** values, int rowCount);

    void swapRow(int that, int where, Factor **values, int columnCount);
    void multiplyPowByFactor(int row, Factor** values, Factor mul, int columnCount);
    void subtractionRow(int first, int second, Factor **values, int columnCount);

    Factor **transformationAfterGauss(int rowCount, int columnCount, Factor **val);

    QString createNewWindowTitle();

public:
    explicit Solution(QWidget *parent = 0);
    ~Solution();

    void setParametrsArtificalBasis(int rowCount, int columnCount, Factor **values, int directionSolution);
    void setParametrsGauss(int rowCount, int columnCount, Factor **values,
                           int directionSolution, QList<int> selectedValues);

private slots:
    void on_nextStep_clicked();

    void on_previousStep_clicked();

    void on_autoSolution_clicked();

private:
    Ui::Solution *ui;
};

#endif // SOLUTION_H
