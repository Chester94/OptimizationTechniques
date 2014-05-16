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
    QString applicationName;
    QString path;

    bool checkValue(int rowCount, int columnCount, Factor **values);
    int directionSolution();

    void MessengeBoxFunctionWithoutValues(Factor val);
    void MessengeBoxImpossibleCondition(int num);
    void MessengeBoxZeroCondition(int num);
    void MessengeBoxErrorGaussCountValues();
    void MessengeBoxErrorPath();

    bool checkGaussValueCount(QList<int> selVal);

    bool checkPathToOpenFile(QString pathToFile);
    QString checkPathToSaveFile(QString &pathToFile);

    void openObjective(QString path);
    void saveObjective(QString &path);

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

    void on_saveAsAction_triggered();

    void on_exit_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
