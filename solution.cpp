#include "solution.h"
#include "ui_solution.h"

Solution::Solution(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Solution)
{
    ui->setupUi(this);

    ui->previousStep->setEnabled(false);

    listSymTable.clear();

    allVeryBad = false;
}

Solution::~Solution()
{
    delete ui;
}

void Solution::trainingNextStep()
{
    ui->tableView->setMyVerticalHeader(symTable.getVerticalHeader());
    ui->tableView->setMyHorizontalHeader(symTable.getHorizontalHeader());

    ui->tableView->setValues(symTable.getRowCount(), symTable.getColumnCount(),
                             symTable.getValues());

    pathSelection();
}

void Solution::pathSelection()
{
    if( symTable.isEndOfArtificalBasis() )
    {
        if( symTable.zeroLastRow() )
        {
            symTable.computeLastRow(func);

            ui->tableView->setValues(symTable.getRowCount(), symTable.getColumnCount(),
                                     symTable.getValues());
        }

        ui->tableView->clearAllocation();

        this->setWindowTitle(createNewWindowTitle());

        switch (symTable.isSolution())
        {
        case REAL_SOLUTION :
            ui->nextStep->setEnabled(false);
            MessageBoxRealSolution();
            break;

        case INF_SOLUTION :
            this->setWindowTitle("Ребро в бесконечность");
            ui->nextStep->setEnabled(false);
            MessageBoxInfSolution();
            break;

        case NOT_SOLUTION :
            ui->nextStep->setEnabled(true);
            supElements = symTable.searchSupportElements();
            ui->tableView->selectSupportElemtnts(supElements);
            break;
        }
    }
    else
    {
        supElements = symTable.searchSupportElementsForArtificalBasis();
        ui->tableView->selectSupportElemtnts(supElements);

        if(supElements.length() == 0)
        {
            MessengeBoxError();
            this->close();
        }

        this->setWindowTitle("Искусственный базис");
    }
}

void Solution::CheckFunctionDirection(int columnCount)
{
    if( direction == MAX )
        for( int i = 0; i < columnCount; i++ )
            func[i] = -func[i];
}

void Solution::CheckCondition(int rowCount, int columnCount, Factor** values)
{
    for( int i = 0; i < rowCount-1; i++ )
        if(values[i][columnCount-1] < 0)
            for( int j = 0; j < columnCount; j++ )
                values[i][j] = -values[i][j];
}

void Solution::MessageBoxRealSolution()
{
    Factor sol = symTable.getSolution(func);

    if( direction == MAX )
        sol = -sol;

    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information, "Решение найдено",
                             "Решение достигается в точке: " +
                             symTable.getPointSolution() + "\nЗначение функции: " +
                             sol.toString());

    msgBox->setWindowIcon(QIcon("resources\\logo.png"));
    msgBox->exec();
    delete msgBox;
}

void Solution::MessageBoxInfSolution()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, "Решение найдено",
                                          "Существует ребро в бесконечность");
    msgBox->setWindowIcon(QIcon("resources\\logo.png"));
    msgBox->exec();
    delete msgBox;
}

void Solution::MessengeBoxError()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка условий",
                                          "Проверьте правильность ввода данных");
    msgBox->setWindowIcon(QIcon("resources\\logo.png"));
    msgBox->exec();
    delete msgBox;
    allVeryBad = true;
}

void Solution::MessengeBoxGaussImpossible()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка условий",
                                          "Метод гаусса невозможен");
    msgBox->setWindowIcon(QIcon("resources\\logo.png"));
    msgBox->exec();
    delete msgBox;
    allVeryBad = true;
}

void Solution::MessengeBoxGaussSolutionDontPermissible()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка выбора главных переменных",
                                          "Данные переменные в базисе дают недопустимое решение");
    msgBox->setWindowIcon(QIcon("resources\\logo.png"));
    msgBox->exec();
    delete msgBox;
    allVeryBad = true;
}

void Solution::setFunc(int columnCount, Factor **values)
{
    func = new Factor [columnCount];

    for( int i = 0; i < columnCount; i++ )
        func[i] = values[0][i];

    CheckFunctionDirection(columnCount);
}

Factor** Solution::setCondition(int rowCount, int columnCount, Factor **values)
{
    Factor **new_values = new Factor* [rowCount];

    for( int i = 0; i < rowCount; i++ )
        new_values[i] = new Factor [columnCount];

    for( int i = 0; i < rowCount-1; i++ )
        for( int j = 0; j < columnCount; j++ )
            new_values[i][j] = values[i+1][j];

    CheckCondition(rowCount, columnCount, new_values);

    for( int i = 0; i < columnCount; i++ )
    {
        for( int j = 0; j < rowCount-1; j++ )
            new_values[rowCount-1][i] = new_values[rowCount-1][i] + new_values[j][i];

        new_values[rowCount-1][i] = -new_values[rowCount-1][i];
    }

    return new_values;
}

void Solution::deleteValues(Factor **values, int rowCount)
{
    if(values)
    {
        for( int i = 0; i < rowCount; i++ )
            delete values[i];

        delete values;
    }
}

Factor** Solution::gauss(int rowCount, int columnCount, Factor **val, QList<int> selectedValues)
{
// ***********************************************************************
    Factor **values;

    values = new Factor* [rowCount];
    for(int i = 0; i < rowCount; i++)
        values[i] = new Factor [columnCount];

    for( int i = 0; i < rowCount; i++ )
        for( int j = 0; j < columnCount; j++ )
            values[i][j] = Factor(0);

    int j = 0;

    for( int k = 0; k < selectedValues.length(); k++ )
        if( selectedValues[k] == 1 )
        {
            for( int i = 0; i < rowCount-1; i++ )
                values[i][j] = val[i+1][k];
            j++;
        }

    for( int k = 0; k < selectedValues.length(); k++ )
        if( selectedValues[k] == 0 )
        {
            for( int i = 0; i < rowCount-1; i++ )
                values[i][j] = val[i+1][k];
            j++;
        }

    for( int i = 0; i < rowCount-1; i++ )
        values[i][columnCount-1] = val[i+1][columnCount-1];
// ***********************************************************************


// ***********************************************************************
    for( int i = 0; i < rowCount-1; i++ )
    {
        int n = searchIndexMatchedString(i, values, rowCount - 1);
        if( n != -1 )
        {
            swapRow(n, i, values, columnCount);
            multiplyPowByFactor(i, values, values[i][i]^-1, columnCount);

            for( int j = i+1; j < rowCount - 1; j++ )
            {
                Factor tmp = values[j][i];
                if( tmp != 0 )
                {
                    multiplyPowByFactor(i, values, tmp, columnCount);
                    subtractionRow(j, i, values, columnCount);
                    multiplyPowByFactor(i, values, tmp^-1, columnCount);
                }
            }
        }
        else
        {
            MessengeBoxGaussImpossible();
            this->close();
        }
    }
// ***********************************************************************

// ***********************************************************************
    for( int  i = rowCount-2; i > 0; i-- )
        for( int j = i-1; j >= 0; j-- )
        {
            Factor tmp = values[j][i];
            if( tmp != 0 )
            {
                multiplyPowByFactor(i, values, tmp, columnCount);
                subtractionRow(j, i, values, columnCount);
                multiplyPowByFactor(i, values, tmp^-1, columnCount);
            }
        }
// ***********************************************************************

    return values;
}

int Solution::searchIndexMatchedString(int begin, Factor **values, int rowCount)
{
    for( int i = begin; i < rowCount; i++ )
        if( values[i][begin] != 0 )
            return i;

    return -1;
}

void Solution::swapRow(int that, int where, Factor **values, int columnCount)
{
    Factor tmp;

    for( int i = 0; i < columnCount; i++ )
    {
        tmp = values[where][i];
        values[where][i] = values[that][i];
        values[that][i] = tmp;
    }
}

void Solution::multiplyPowByFactor(int row, Factor **values, Factor mul, int columnCount)
{
    for( int i = 0; i < columnCount; i++ )
        values[row][i] = values[row][i] * mul;
}

void Solution::subtractionRow(int first, int second, Factor **values, int columnCount)
{
    for( int i = 0; i < columnCount; i++ )
        values[first][i] = values[first][i] - values[second][i];
}

Factor** Solution::transformationAfterGauss(int rowCount, int columnCount, Factor **val)
{
    Factor **values;

    values = new Factor* [rowCount];
    for(int i = 0; i < rowCount; i++)
        values[i] = new Factor [columnCount-rowCount+1];

    for( int i = 0; i < rowCount-1; i++ )
        for( int j = 0; j < columnCount-rowCount+1; j++ )
            values[i][j] = val[i][j+rowCount-1];

    deleteValues(val, rowCount);

    return values;
}

QString Solution::createNewWindowTitle()
{
    Factor sol = symTable.getSolution(func);

    if( direction == MAX )
        sol = -sol;

    return "Текущая точка " + symTable.getPointSolution() +
            ", значение функции = " + sol.toString();
}

void Solution::setParametrsArtificalBasis(int rowCount, int columnCount, Factor **values, int directionSolution)
{
    this->setWindowTitle("Искусственный базис");

    direction = directionSolution;

    setFunc(columnCount, values);

    symTable.setValues(setCondition(rowCount, columnCount, values),
                       rowCount, columnCount, columnCount-1);

    symTable.createVerticalHeader();
    symTable.createHorizontalHeader();

    ui->tableView->setDimension(rowCount, columnCount);

    ui->tableView->setMyVerticalHeader(symTable.getVerticalHeader());
    ui->tableView->setMyHorizontalHeader(symTable.getHorizontalHeader());

    ui->tableView->setValues(symTable.getRowCount(), symTable.getColumnCount(),
                              symTable.getValues());

    if(allVeryBad)
        return;

    pathSelection();

    deleteValues(values, rowCount);

    listSymTable.append(symTable);
}

void Solution::setParametrsGauss(int rowCount, int columnCount, Factor **values,
                                 int directionSolution, QList<int> selectedValues)
{
    this->setWindowTitle("Гаусс");

    direction = directionSolution;

    setFunc(columnCount, values);

    symTable.setValues(transformationAfterGauss(rowCount, columnCount,
                                                gauss(rowCount, columnCount, values, selectedValues)),
                       rowCount, columnCount-rowCount+1, columnCount-1);
    symTable.createVerticalHeader(selectedValues);
    symTable.createHorizontalHeader(selectedValues);

    symTable.computeLastRow(func);

    ui->tableView->setMyVerticalHeader(symTable.getVerticalHeader());
    ui->tableView->setMyHorizontalHeader(symTable.getHorizontalHeader());

    ui->tableView->setValues(symTable.getRowCount(), symTable.getColumnCount(),
                              symTable.getValues());

    if(allVeryBad)
        return;

    if( !symTable.checkPermissibleSolution() )
    {
        MessengeBoxGaussSolutionDontPermissible();
        this->close();
        return;
    }

    pathSelection();

    deleteValues(values, rowCount);

    listSymTable.append(symTable);
}

void Solution::on_nextStep_clicked()
{
    int rowIndex, columnIndex;

    if( ui->tableView->selectedItems().length() == 0 )
    {
        rowIndex = supElements[0];
        columnIndex = supElements[1];
    }
    else
    {
        rowIndex = ui->tableView->selectedItems().at(0)->row();
        columnIndex = ui->tableView->selectedItems().at(0)->column();
    }

    symTable.nextStep(rowIndex, columnIndex);

    trainingNextStep();

    ui->previousStep->setEnabled(true);

    listSymTable.append(symTable);
}

void Solution::on_previousStep_clicked()
{
    ui->nextStep->setEnabled(true);
    listSymTable.removeLast();

    symTable = SymplexTable(listSymTable.last());

    trainingNextStep();

    if(listSymTable.length() == 1)
        ui->previousStep->setEnabled(false);
}

void Solution::on_autoSolution_clicked()
{
    while(symTable.isSolution() == NOT_SOLUTION)
        on_nextStep_clicked();
}
