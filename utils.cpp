#include "mainwindow.h"
#include "./ui_mainwindow.h"

void MainWindow::listPasswordFiles(const QString &dir) {
    QDir directory{dir};
    ui->lineEdit_read_currentDir->setText(directory.absolutePath());
    directory.setNameFilters(QStringList{"*.pwd"});
    auto fileList = directory.entryList();
    ui->listWidget_read_fileList->clear();
    if (fileList.size() == 0)
        ui->listWidget_read_fileList->addItem("<no passwords>");
    else
        ui->listWidget_read_fileList->addItems(fileList);
}

int numberOfNonAlphanumeric(std::string &s) {
    int num = 0;
    for (char c : s)
        num += !std::isalnum(c);
    return num;
}
