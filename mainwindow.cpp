#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./downloader.h"
#include "./transformurls.h"
#include "./deletefiles.h"
#include <QDebug>
#include <QDialog>
#include <QTextStream>
#include <QStringListModel>
#include <QFile>
#include <QTextStream>
#include <QPushButton>
#include <QTextEdit>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

const QString LISTS_SOURCE("lists.txt");


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnGetHosts_clicked()
{
    launchVeryLongTask();
}

void MainWindow::showListsUrls(){


    QFile input(LISTS_SOURCE);
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QDialog dialog;

        // Setup dialog layout
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        QLabel *label = new QLabel("Not found (in showListsUrls) "+LISTS_SOURCE+"!", &dialog);
        layout->addWidget(label);
        dialog.setLayout(layout);


        dialog.exec(); // Show dialog modally
        return;
    }


    QTextStream in(&input);

    QStringList lines;
    while (!in.atEnd()) {
        QString line = in.readLine();
        lines << line;
    }

    input.close();
    QStringListModel* model = new QStringListModel( lines, this);
    ui->listView->setModel(model);
    this->getHostsByItems(this->ui->listView->model());
}

void MainWindow::getHostsByItems(QAbstractItemModel* model){
    if (!model) {
        QDialog dialog;

        // Setup dialog layout
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        QLabel *label = new QLabel("No model! (getHostsByItems)", &dialog);
        layout->addWidget(label);
        dialog.setLayout(layout);


        dialog.exec(); // Show dialog modally
        return ;
    }

    this->ui->progressBar->setMinimum(0);
    this->ui->progressBar->setMaximum( model->rowCount());
    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex index = model->index(row, 0);
        QString itemUrl = model->data(index).toString();


        QString fileNamePrefix = "";

        fileNamePrefix.append(QString::number(row));

        this->ui->pteLog->appendPlainText("run for " + itemUrl + " as " + fileNamePrefix);

        QFuture<void> future = QtConcurrent::run(
            longTaskWithOneUrl,
            itemUrl, fileNamePrefix );
        QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
        // Set the future to the watcher
        watcher->setFuture(future);

        connect(watcher, &QFutureWatcher<void>::finished, this,
                [this, watcher, row]() {
            bool result = watcher->isFinished();
            if (!result) {
                this->ui->pteLog->appendPlainText("something wrong in FutureWatcher");
                return;
            }
            // Update ui
            QString MODIFIED_FILE = "";
            MODIFIED_FILE.append(QString::number(row));
            MODIFIED_FILE.append(QString(".mdf"));
            this->ui->pteLog->appendPlainText("processed "+MODIFIED_FILE);
            this->updateTextView(MODIFIED_FILE);

            deleteFileByName(MODIFIED_FILE);
            this->ui->progressBar->setValue(row);
            watcher->deleteLater(); // Clean up

        });



    }
}


void MainWindow::launchVeryLongTask(){

    QFuture<QString> future = QtConcurrent::run(this->veryLongTask);
    QFutureWatcher<QString> *watcher = new QFutureWatcher<QString>(this);
    watcher->setFuture(future);
    connect(watcher, &QFutureWatcher<QString>::finished, this, [this, watcher]() {
        QString result = watcher->result();
        this->ui->frmResults->setToolTip(result);
        // Update ui
        this->showListsUrls();
        watcher->deleteLater(); // Clean up
    });

    watcher->setFuture(future);

}
QString MainWindow::veryLongTask(){

    const QString URL_SOURCE("https://v.firebog.net/hosts/lists.php?type=tick");
    downloadFile(URL_SOURCE, LISTS_SOURCE);
    // Read strings from file
    QFile file(LISTS_SOURCE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QDialog dialog;

        // Setup dialog layout
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        QLabel *label = new QLabel("Error with lists source file", &dialog);
        layout->addWidget(label);
        dialog.setLayout(layout);


        dialog.exec(); // Show dialog modally
        return "Error with source files";
    }

    QTextStream textStream(&file);
    QStringList lines;
    while (!textStream.atEnd()) {
        QString line = textStream.readLine();
        lines.append(line);
    }
    file.close();


    return "Success loaded";
}

void MainWindow::updateTextView(const QString& fileName){
    this->ui->lblHostsText->setText(fileName);
    // Read strings from file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QDialog dialog;

        // Setup dialog layout
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        QLabel *label = new QLabel("Error " + fileName, &dialog);
        layout->addWidget(label);
        dialog.setLayout(layout);


        dialog.exec(); // Show dialog modally
        return;
    }

    QTextStream textStream(&file);
    QString lines = "";
    while (!textStream.atEnd()) {
        QString line = textStream.readLine();
        if (line.contains("127.0.0.1")||line.contains("0.0.0.0")){
            lines.append(line);
        };
        if (!line.contains("127.0.0.1")&&!line.contains("0.0.0.0")){
            QString modifyStr("0.0.0.0 ");
            modifyStr.append(line);
            lines.append(modifyStr);
        }
        lines.append("\n\r");
    }
    file.close();
    this->ui->textEdit->appendPlainText(lines);



}

void MainWindow::longTaskWithOneUrl(const QString& itemUrl, const QString& fileNamePrefix){
    QString TEMP_FILE(fileNamePrefix);
    TEMP_FILE.append(".tmp");
    QString MODIFIED_FILE(fileNamePrefix);
    MODIFIED_FILE.append(".mdf");
    downloadFile(itemUrl, TEMP_FILE);
    transformUrls(TEMP_FILE, MODIFIED_FILE);
    deleteFileByName(TEMP_FILE);
}
