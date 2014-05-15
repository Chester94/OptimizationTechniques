#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "solution.h"
#include "choicevariables.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    bool checkValue(int rowCount, int columnCount, Factor **values);
    int directionSolution();

    void MessengeBoxFunctionWithoutValues(Factor val);
    void MessengeBoxImpossibleCondition(int num);
    void MessengeBoxZeroCondition(int num);
    void MessengeBoxErrorGaussCountValues();

    bool checkGaussValueCount(QList<int> selVal);

    QString checkPathToSaveFile(QString path);

private slots:
    void on_addRow_clicked();

    void on_deleteRow_clicked();

    void on_addColumn_clicked();

    void on_deleteColumn_clicked();

    void on_saveAction_triggered();

    void on_newAction_triggered();

    void on_openAction_triggered();

    void on_artificialBasis_clicked();

    void on_gauss_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
