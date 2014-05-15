#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_newAction_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkValue(int rowCount, int columnCount, Factor **values)
{
    bool f = true;
    for( int i = 0; i < columnCount-1; i++ )
        if( values[0][i] != 0 )
            f = false;

    if( f )
    {
        MessengeBoxFunctionWithoutValues(values[0][columnCount-1]);

        return false;
    }


    for( int i = 1; i < rowCount; i++ )
    {
        bool f = true;

        for( int j = 0; j < columnCount - 1; j++ )
            if( values[i][j] != 0 )
                f = false;

        if( f )
        {
            if( values[i][columnCount-1] != 0 )
                MessengeBoxImpossibleCondition(i);
            else
                MessengeBoxZeroCondition(i);

            return false;
        }
    }

    return true;
}

int MainWindow::directionSolution()
{
    if( ui->min->isChecked())
        return MIN;
    else
        return MAX;
}

void MainWindow::MessengeBoxFunctionWithoutValues(Factor val)
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка целевой функции",
        QString("Целевая функция не содержит переменных.\nЗначение всегда равно ") +
        val.toString());

    msgBox->setWindowIcon(QIcon("resources\\logo.png"));
    msgBox->exec();
    delete msgBox;
}

void MainWindow::MessengeBoxImpossibleCondition(int num)
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка условий",
                                     QString("Невозможное условие №%1").arg(num));
    msgBox->setWindowIcon(QIcon("resources\\logo.png"));
    msgBox->exec();
    delete msgBox;
}

void MainWindow::MessengeBoxZeroCondition(int num)
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка условий",
                                     QString("Нулевое условие №%1").arg(num));
    msgBox->setWindowIcon(QIcon("resources\\logo.png"));
    msgBox->exec();
    delete msgBox;
}

void MainWindow::MessengeBoxErrorGaussCountValues()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка введенных данных",
        QString("Количество главных переменных должно быть равным %1").arg(ui->tableInput->rowCount()-1) );
    msgBox->setWindowIcon(QIcon("resources\\logo.png"));
    msgBox->exec();
    delete msgBox;
}

bool MainWindow::checkGaussValueCount(QList<int> selVal)
{
    int sum = 0;
    for( int i = 0; i < selVal.length(); i++ )
        sum += selVal[i];

    if( sum == ui->tableInput->rowCount() - 1 )
        return true;
    else
    {
        MessengeBoxErrorGaussCountValues();
        return false;
    }
}

QString MainWindow::checkPathToSaveFile(QString path)
{
    if(path.length() < 3)
        return path;

    if( path.mid(path.length()-3, 3) != ".mo" )
        path += ".mo";

    return path;
}

void MainWindow::on_addRow_clicked()
{
    ui->tableInput->addRow();
}

void MainWindow::on_deleteRow_clicked()
{
    ui->tableInput->deleteRow();
}

void MainWindow::on_addColumn_clicked()
{
    ui->tableInput->addColumn();
}

void MainWindow::on_deleteColumn_clicked()
{
    ui->tableInput->deleteColumn();
}

void MainWindow::on_artificialBasis_clicked()
{
    int rowCount = ui->tableInput->rowCount();
    int colunmCount = ui->tableInput->columnCount();

    Factor **values = ui->tableInput->getValues();

    if( checkValue( rowCount, colunmCount, values ) )
    {
        Solution *sol = new Solution();
        sol->setParametrsArtificalBasis(rowCount, colunmCount, values, directionSolution());
        sol->show();
    }
    else
        if(values)
        {
            for( int i = 0; i < rowCount; i++ )
                delete values[i];

            delete values;
        }
}

void MainWindow::on_gauss_clicked()
{
    ChoiceVariables *dialGauss = new ChoiceVariables();
    dialGauss->setColumnCount(ui->tableInput->columnCount()-1);
    dialGauss->setConditionCount(ui->tableInput->rowCount()-1);

    if( dialGauss->exec() == QDialog::Accepted )
    {
        QList<int> selectedValues = dialGauss->getSelectedValues();

        int rowCount = ui->tableInput->rowCount();
        int colunmCount = ui->tableInput->columnCount();

        Factor **values = ui->tableInput->getValues();

        if( checkGaussValueCount(selectedValues) && checkValue( rowCount, colunmCount, values ) )
        {
            Solution *sol = new Solution();
            sol->show();
            sol->setParametrsGauss(rowCount, colunmCount, values, directionSolution(), selectedValues);
        }
        else
            if(values)
            {
                for( int i = 0; i < rowCount; i++ )
                    delete values[i];

                delete values;
            }
    }

    delete dialGauss;
}

void MainWindow::on_newAction_triggered()
{
    ui->tableInput->setDimension(3, 4);

    ui->tableInput->setMyVerticalHeader();
    ui->tableInput->setMyHorizontalHeader();

    ui->min->setChecked(true);

    this->setWindowTitle("<null>");
}

void MainWindow::on_openAction_triggered()
{
    QString path = QFileDialog::getOpenFileName( this, tr("Open File"), "D:\\", tr("*.mo") );

    if( path != "" )
    {
        QFile input(path);
        input.open( QIODevice::ReadOnly );

        QTextStream in(&input);

        int directionSolution;
        int rowCount;
        int columnCount;

        in >> directionSolution >> rowCount >> columnCount;

        ui->min->setChecked(!directionSolution);
        ui->max->setChecked(directionSolution);

        Factor **values = new Factor* [rowCount];
        for(int i = 0; i < rowCount; i++)
            values[i] = new Factor [columnCount];

        QString tmp;

        for( int i = 0; i < rowCount; i++ )
            for( int j = 0; j < columnCount; j++ )
            {
                in >> tmp;
                values[i][j] = Factor(tmp);
            }

        ui->tableInput->setValues(rowCount, columnCount, values);
        ui->tableInput->setMyVerticalHeader();
        ui->tableInput->setMyHorizontalHeader();

        if(values)
        {
            for( int i = 0; i < rowCount; i++ )
                delete values[i];

            delete values;
        }

        this->setWindowTitle(path);
    }
}

void MainWindow::on_saveAction_triggered()
{
    QString path = QFileDialog::getSaveFileName( this, tr("Create File"), "D:\\", tr("*.mo") );

    if( path != "" )
    {
        path = checkPathToSaveFile(path);
        QFile output(path);
        output.open( QIODevice::WriteOnly );

        QTextStream out(&output);

        int directionSolution = ui->max->isChecked();
        int rowCount = ui->tableInput->rowCount();
        int columnCount = ui->tableInput->columnCount();

        Factor **value = ui->tableInput->getValues();

        out << directionSolution << " " << rowCount << " " << columnCount;

        for( int i = 0; i < rowCount; i++ )
            for( int j = 0; j < columnCount; j++)
                out << endl << value[i][j].toString();

        output.close();

        this->setWindowTitle(path);
    }
}
