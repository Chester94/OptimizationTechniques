#include "choicevariables.h"
#include "ui_choicevariables.h"

ChoiceVariables::ChoiceVariables(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoiceVariables)
{
    ui->setupUi(this);

    selectedValues.clear();
    conditionCount = 0;

    this->setWindowTitle("Выбор главных переменных");
}

ChoiceVariables::~ChoiceVariables()
{
    delete ui;
}

void ChoiceVariables::setColumnCount(int columnCount)
{
    ui->tableView->toGaussMethod(columnCount);

    QList<int> HorHeader;
    for( int i = 0; i < columnCount; i++ )
        HorHeader << i+1;

    QStringList VerHeader;
    VerHeader << "*";

    ui->tableView->setMyHorizontalHeader(HorHeader);
    ui->tableView->setVerticalHeaderLabels(VerHeader);
}

void ChoiceVariables::on_buttonBox_accepted()
{
    selectedValues = ui->tableView->selectedValues();

    /*int sum = 0;
    for( int i = 0; i < selectedValues.length(); i++ )
        sum += selectedValues[i];

    if( sum = conditionCount )
        accept();*/
}
