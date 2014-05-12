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

    void trainingNextStep();

public:
    explicit Solution(QWidget *parent = 0);
    ~Solution();

    void setParametrsArtificalBasis(int rowCount, int columnCount, Factor **values, int directionSolution);

private slots:
    void on_nextStep_clicked();

    void on_previousStep_clicked();

    void on_autoSolution_clicked();

private:
    Ui::Solution *ui;
};

#endif // SOLUTION_H
