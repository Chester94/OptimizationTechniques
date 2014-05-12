#include "solution.h"
#include "ui_solution.h"

Solution::Solution(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Solution)
{
    ui->setupUi(this);

    ui->previousStep->setEnabled(false);

    listSymTable.clear();
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

    if( symTable.isEndOfArtificalBasis() )
    {
        if( symTable.zeroLastRow() )
        {
            symTable.computeLastRow(func);

            ui->tableView->setValues(symTable.getRowCount(), symTable.getColumnCount(),
                                     symTable.getValues());
        }

        ui->tableView->clearAllocation();

        QMessageBox *msgBox;

        switch (symTable.isSolution())
        {
        case REAL_SOLUTION :
            ui->nextStep->setEnabled(false);

            msgBox = new QMessageBox(QMessageBox::Information, "Решение найдено",
                                     "Решение достигается в точке: " +
                                     symTable.getPointSolution() + "\nЗначение функции: " +
                                     symTable.getSolution(func).toString());
            msgBox->setWindowIcon(QIcon("resources\\logo.png"));
            msgBox->exec();
            delete msgBox;

            break;

        case INF_SOLUTION :
            ui->nextStep->setEnabled(false);

            msgBox = new QMessageBox(QMessageBox::Warning, "Решение найдено",
                                                  QString("Существует ребро в бесконечность"));
            msgBox->setWindowIcon(QIcon("resources\\logo.png"));
            msgBox->exec();
            delete msgBox;

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
            QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка условий",
                                                  QString("Несовместная задача"));
            msgBox->setWindowIcon(QIcon("resources\\logo.png"));
            msgBox->exec();
            delete msgBox;

            this->close();
        }
    }
}

void Solution::setParametrsArtificalBasis(int rowCount, int columnCount, Factor **values, int directionSolution)
{
    this->setWindowTitle("Искусственный базис");

    QList<int> VerticalHeader;
    QList<int> HorizontalHeader;

    for( int i = 0; i < columnCount-1; i++ )
        HorizontalHeader << i+1;

    for( int i = 0; i < rowCount-1; i++ )
        VerticalHeader << columnCount+i;

    func = new Factor [columnCount];

    for( int i = 0; i < columnCount; i++ )
        func[i] = values[0][i];

    Factor **new_values = new Factor* [rowCount];

    for( int i = 0; i < rowCount; i++ )
        new_values[i] = new Factor [columnCount];

    for( int i = 0; i < rowCount-1; i++ )
        for( int j = 0; j < columnCount; j++ )
            new_values[i][j] = values[i+1][j];

    for( int i = 0; i < columnCount; i++ )
    {
        for( int j = 0; j < rowCount-1; j++ )
            new_values[rowCount-1][i] = new_values[rowCount-1][i] + new_values[j][i];

        new_values[rowCount-1][i] = -new_values[rowCount-1][i];
    }

    symTable.setValues(new_values, rowCount, columnCount, columnCount-1);
    symTable.setVerticalHeader(VerticalHeader);
    symTable.setHorizontalHeader(HorizontalHeader);

    ui->tableView->setDimension(rowCount, columnCount);

    ui->tableView->setMyVerticalHeader(symTable.getVerticalHeader());
    ui->tableView->setMyHorizontalHeader(symTable.getHorizontalHeader());

    ui->tableView->setValues(symTable.getRowCount(), symTable.getColumnCount(),
                              symTable.getValues());

    supElements = symTable.searchSupportElementsForArtificalBasis();
    ui->tableView->selectSupportElemtnts(supElements);

    if(values)
    {
        for( int i = 0; i < rowCount; i++ )
            delete values[i];

        delete values;
    }

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
