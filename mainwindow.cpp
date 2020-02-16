#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_radioButton_new_toggled(bool checked) {
    if (checked) {
        ui->stackedWidget->setCurrentIndex(0);
        ui->lineEdit_new_filename->setFocus();
    }
}

void MainWindow::on_lineEdit_new_filename_editingFinished() {
    if (!ui->lineEdit_new_filename->text().endsWith(".pwd"))
        ui->lineEdit_new_filename->setText(ui->lineEdit_new_filename->text() + ".pwd");
    if (QDir::isRelativePath(ui->lineEdit_new_filename->text()))
        ui->lineEdit_new_filename->setText(QDir::currentPath() + "/" + ui->lineEdit_new_filename->text());
}

void MainWindow::on_pushButton_new_browseFile_clicked() {
    ui->lineEdit_new_filename->setText(QFileDialog::getSaveFileName(this, "Choose password file", QDir::currentPath(), "Password files (*.pwd)"));
}

void MainWindow::on_pushButton_new_storePassword_clicked() {
    if (ui->lineEdit_new_filename->text() == "") {
        ui->label_message->setText("Empty filename!");
        return;
    }
    if (ui->lineEdit_new_passwordToStore->text() == "" ||
        ui->lineEdit_new_passwordToStoreConfirm->text() == "" ||
        ui->lineEdit_new_masterPassword->text() == "" ||
        ui->lineEdit_new_masterPasswordConfirm->text() == "") {
        ui->label_message->setText("Empty password field!");
        return;
    }
    if (ui->lineEdit_new_passwordToStore->text() != ui->lineEdit_new_passwordToStoreConfirm->text() ||
        ui->lineEdit_new_masterPassword->text() != ui->lineEdit_new_masterPasswordConfirm->text()) {
        ui->label_message->setText("Passwords don't match!");
        return;
    }
    std::ofstream passwordFile{ui->lineEdit_new_filename->text().toStdString()};
    if (!passwordFile.is_open()) {
        ui->label_message->setText("Could not write to file!");
        return;
    }
    std::string masterPassword{ui->lineEdit_new_masterPassword->text().toStdString()};
    std::string passwordToStore{ui->lineEdit_new_passwordToStore->text().toStdString()};
    while (masterPassword.length() < passwordToStore.length())
        masterPassword += masterPassword;
    for (int i = 0; i < passwordToStore.length(); ++i)
        passwordToStore[i] = passwordToStore[i] + masterPassword[i] - CHAR_LOWEST > CHAR_HIGHEST ?
                             passwordToStore[i] + masterPassword[i] - CHAR_HIGHEST - 1 :
                             passwordToStore[i] + masterPassword[i] - CHAR_LOWEST;
    passwordFile << passwordToStore;
    ui->label_message->setText("Password successully saved.");
}

void MainWindow::on_lineEdit_new_passwordToStore_editingFinished() {
    ui->label_new_messagePasswordToStore->setText(
                ui->lineEdit_new_passwordToStore->text() != "" &&
                ui->lineEdit_new_passwordToStoreConfirm->text() != "" &&
                ui->lineEdit_new_passwordToStore->text() != ui->lineEdit_new_passwordToStoreConfirm->text() ?
                "Passwords don't match!" : "");
}

void MainWindow::on_lineEdit_new_passwordToStoreConfirm_editingFinished() {
    MainWindow::on_lineEdit_new_passwordToStore_editingFinished();
}

void MainWindow::on_lineEdit_new_masterPassword_editingFinished() {
    ui->label_new_messageMasterPassword->setText(
                ui->lineEdit_new_masterPassword->text() != "" &&
                ui->lineEdit_new_masterPasswordConfirm->text() != "" &&
                ui->lineEdit_new_masterPassword->text() != ui->lineEdit_new_masterPasswordConfirm->text() ?
                "Passwords don't match!" : "");
}

void MainWindow::on_lineEdit_new_masterPasswordConfirm_editingFinished() {
    MainWindow::on_lineEdit_new_masterPassword_editingFinished();
}

void MainWindow::on_radioButton_read_toggled(bool checked) {
    if (checked) {
        ui->stackedWidget->setCurrentIndex(1);
        if (ui->lineEdit_read_currentDir->text() == "")
            listPasswordFiles(QDir::currentPath());
    }
}

void MainWindow::on_lineEdit_read_currentDir_editingFinished() {
    listPasswordFiles(ui->lineEdit_read_currentDir->text());
}

void MainWindow::on_pushButton_read_browseDir_clicked() {
    listPasswordFiles(QFileDialog::getExistingDirectory(this, "Open password directory", ui->lineEdit_read_currentDir->text()));
}

void MainWindow::on_listWidget_read_fileList_itemClicked(QListWidgetItem *item) {
    ui->lineEdit_read_masterPassword->setEnabled(item->text() != "<no passwords>");
}

void MainWindow::on_lineEdit_read_masterPassword_editingFinished() {
    std::ifstream passwordFile{(ui->lineEdit_read_currentDir->text() + "/" + ui->listWidget_read_fileList->currentItem()->text()).toStdString()};
    if (!passwordFile.is_open()) {
        ui->label_message->setText("Could not read from file!");
        return;
    }
    std::string codedPassword{std::istreambuf_iterator<char>{passwordFile}, std::istreambuf_iterator<char>{}};
    std::string masterPassword{ui->lineEdit_read_masterPassword->text().toStdString()};
    while (masterPassword.length() < codedPassword.length())
        masterPassword += masterPassword;
    for (int i = 0; i < codedPassword.length(); ++i)
        codedPassword[i] = codedPassword[i] - masterPassword[i] + CHAR_LOWEST < CHAR_LOWEST ?
                           codedPassword[i] - masterPassword[i] + CHAR_HIGHEST + 1 :
                           codedPassword[i] - masterPassword[i] + CHAR_LOWEST;
    ui->lineEdit_read_decodedPassword->setText(QString::fromStdString(codedPassword));
}

void MainWindow::on_radioButton_generate_toggled(bool checked) {
    if (checked) {
        ui->stackedWidget->setCurrentIndex(2);
        ui->lineEdit_generate_length->setFocus();
    }
}

void MainWindow::on_lineEdit_generate_length_editingFinished() {
    ui->pushButton_generate_storePassword->setEnabled(true);
    int n = ui->lineEdit_generate_length->text().toInt();
    std::mt19937 engine{std::random_device{}()};
    std::uniform_int_distribution<int> dist{CHAR_LOWEST, CHAR_HIGHEST};
    std::string password;
    do {
        password = "";
        for (int i = 0; i < n; ++i)
            password += (char) dist(engine);
    } while (numberOfNonAlphanumeric(password) > 2);
    ui->lineEdit_generate_generatedPassword->setText(QString::fromStdString(password));
}

void MainWindow::on_pushButton_generate_generatePassword_clicked() {
    MainWindow::on_lineEdit_generate_length_editingFinished();
}

void MainWindow::on_pushButton_generate_storePassword_clicked() {
    ui->radioButton_new->toggle();
    ui->lineEdit_new_filename->setText("");
    ui->lineEdit_new_passwordToStore->setText(ui->lineEdit_generate_generatedPassword->text());
    ui->lineEdit_new_passwordToStoreConfirm->setText(ui->lineEdit_generate_generatedPassword->text());
    ui->lineEdit_new_masterPassword->setText("");
    ui->lineEdit_new_masterPasswordConfirm->setText("");
}