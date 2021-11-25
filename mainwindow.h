#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_radioButton_binario_toggled(bool checked);

    void on_radioButton_decimal_toggled(bool checked);

    void on_pushButton_calcular_clicked();

    void on_pushButton_limpar_clicked();

    void on_radioButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
