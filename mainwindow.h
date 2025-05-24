#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_pushButton_generate_clicked();

    void on_pushButton_copy_clicked();

    void on_pushButton_save_clicked();

    void on_chooseModuleColor_clicked();

    void on_pushButton_clicked();

    void on_plainTextEdit_data_blockCountChanged(int newBlockCount);

private:
    Ui::MainWindow *ui;
    QColor moduleColor = Qt::black;  // цвет модулей по умолчанию
    QColor backgroundColor = Qt::white;  // цвет фона

    QImage get_graphicsView_result_scene_image();
};
#endif // MAINWINDOW_H
