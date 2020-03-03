#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define CHAR_LOWEST 33
#define CHAR_HIGHEST 126

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
#include <QStatusBar>

#include <fstream>
#include <random>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_radioButton_new_toggled(bool checked);

    void on_radioButton_read_toggled(bool checked);

    void on_radioButton_generate_toggled(bool checked);

    void on_listWidget_read_fileList_itemClicked(QListWidgetItem *item);

    void on_pushButton_read_browseDir_clicked();

    void on_lineEdit_read_masterPassword_editingFinished();

    void on_pushButton_generate_storePassword_clicked();

    void on_lineEdit_read_currentDir_editingFinished();

    void on_pushButton_new_storePassword_clicked();

    void on_pushButton_new_browseFile_clicked();

    void on_lineEdit_new_passwordToStore_editingFinished();

    void on_lineEdit_new_passwordToStoreConfirm_editingFinished();

    void on_lineEdit_new_masterPassword_editingFinished();

    void on_lineEdit_new_masterPasswordConfirm_editingFinished();

    void on_lineEdit_new_filename_editingFinished();

    void on_pushButton_generate_generatePassword_clicked();

private:
    Ui::MainWindow *ui;
    void listPasswordFiles(const QString &dir);
};

int numberOfNonAlphanumeric(std::string &s);
#endif // MAINWINDOW_H
