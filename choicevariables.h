#ifndef CHOICEVARIABLES_H
#define CHOICEVARIABLES_H

#include <QDialog>

namespace Ui {
class ChoiceVariables;
}

class ChoiceVariables : public QDialog
{
    Q_OBJECT

private:
    QList<int> selectedValues;
    int conditionCount;

public:
    explicit ChoiceVariables(QWidget *parent = 0);
    ~ChoiceVariables();
    void setColumnCount(int columnCount);
    void setConditionCount(int condCount) { conditionCount = condCount; }

    QList<int> getSelectedValues() { return selectedValues; }

private slots:

    void on_buttonBox_accepted();

private:
    Ui::ChoiceVariables *ui;
};

#endif // CHOICEVARIABLES_H
