#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Factor **values;

    //values = ui->tableInput->getValues();

    //qDebug() << values[1][2].toString();
    //ui->tableInput->setValues(4, 4, values);

    //ui->tableInput->switchHeaderLabels(1, 1);
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

    if(f)
    {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка целевой функции",
            QString("Целевая функция не содержит переменных.\nЗначение всегда равно ") +
            values[0][columnCount-1].toString());

        msgBox->setWindowIcon(QIcon("resources\\logo.png"));
        msgBox->exec();
        delete msgBox;

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
            {
                QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка условий",
                                                 QString("Невозможное условие №%1").arg(i));
                msgBox->setWindowIcon(QIcon("resources\\logo.png"));
                msgBox->exec();
                delete msgBox;
            }
            else
            {
                QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, "Ошибка условий",
                                                 QString("Нулевое условие №%1").arg(i));
                msgBox->setWindowIcon(QIcon("resources\\logo.png"));
                msgBox->exec();
                delete msgBox;
            }

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
    {
        if(values)
        {
            for( int i = 0; i < rowCount; i++ )
                delete values[i];

            delete values;
        }
    }
}

void MainWindow::on_gauss_clicked()
{
    qDebug() << "gauus method";
}

void MainWindow::on_newAction_triggered()
{
    ui->tableInput->setDimension(3, 4);

    ui->tableInput->setMyVerticalHeader();
    ui->tableInput->setMyHorizontalHeader();
}

void MainWindow::on_openAction_triggered()
{
    QString path = QFileDialog::getOpenFileName( this, tr("Open File"), "D:\\", tr("*.mo") );

    if( path != "" )
    {
        QFile input(path);
        input.open( QIODevice::ReadOnly );

        QTextStream in(&input);

        int rowCount;
        int columnCount;

        in >> rowCount >> columnCount;

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
        QFile output(path);
        output.open( QIODevice::WriteOnly );

        QTextStream out(&output);

        int rowCount = ui->tableInput->rowCount();
        int columnCount = ui->tableInput->columnCount();

        Factor **value = ui->tableInput->getValues();

        out << rowCount << " " << columnCount;

        for( int i = 0; i < rowCount; i++ )
            for( int j = 0; j < columnCount; j++)
                out << endl << value[i][j].toString();

        output.close();

        this->setWindowTitle(path);
    }
}
