#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QString veryLongTask();
    static void longTaskWithOneUrl(const QString& itemUrl, const QString& fileNamePrefix);
    void showListsUrls();

    void updateTextView(const QString& fileName);
    void getHostsByItems(QAbstractItemModel* model);
    void launchVeryLongTask();

private slots:
    void on_btnGetHosts_clicked();







private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
