#include <QMessageBox>
#include <QClipboard>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDateTime>
#include <QColorDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qrcodegen.hpp"

using namespace qrcodegen;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Изначально кнопки неактивны
    ui->pushButton_copy->setEnabled(false);
    ui->pushButton_save->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}


QImage MainWindow::get_graphicsView_result_scene_image() {
    // получаем текущую сцену
    QGraphicsScene *scene = ui->graphicsView_result->scene();

    // создаем изображение нужного размера
    QRectF sceneRect = scene->sceneRect();
    QImage image(sceneRect.size().toSize(), QImage::Format_ARGB32);

    // рисуем сцену в изображение
    QPainter painter(&image);
    scene->render(&painter);
    painter.end();

    // возвращаем изображение
    return image;
}


void MainWindow::on_pushButton_generate_clicked() {
    // устанавливаем сцену
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView_result->setScene(scene);

    // входные данные
    QString inputData = ui->plainTextEdit_data->toPlainText();

    if (inputData.isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Введите текст для кодирования");
        return;
    }

    // выбираем уровень коррекции ошибок
    QrCode::Ecc ecc = QrCode::Ecc::HIGH;

    if (ui->radioButton_ecc_L->isChecked()) {
        ecc = QrCode::Ecc::LOW;
    } else if (ui->radioButton_ecc_M->isChecked()) {
        ecc = QrCode::Ecc::MEDIUM;
    } else if (ui->radioButton_ecc_H->isChecked()) {
        ecc = QrCode::Ecc::HIGH;
    } else if (ui->radioButton_ecc_Q->isChecked()) {
        ecc = QrCode::Ecc::QUARTILE;
    }

    // создаем QR код
    QrCode qr = QrCode::encodeText(inputData.toStdString().c_str(), ecc);

    // получаем размеры холста
    QRect viewportRect = ui->graphicsView_result->viewport()->rect();
    QSizeF boundingRectSize = ui->graphicsView_result->mapToScene(viewportRect).boundingRect().size();

    // размер QR кода в модулях
    int qrSize = qr.getSize();

    // считаем масштаб по каждой оси
    double scaleX = boundingRectSize.width() / qrSize;
    double scaleY = boundingRectSize.height() / qrSize;

    // выбираем масштаб, чтобы все влезло
    int scale = std::floor(std::min(scaleX, scaleY));

    // рисуем модули
    for (int i = 0; i < qrSize; i++) {
        for (int j = 0; j < qrSize; j++) {
            bool currentModule = qr.getModule(i, j);

            QColor color = currentModule ? moduleColor : backgroundColor;


            scene->addRect(i*scale, j*scale, scale, scale, Qt::NoPen, QBrush(color));
        }
    }

    // включаем кнопки копирования и сохранения
    ui->pushButton_copy->setEnabled(true);
    ui->pushButton_save->setEnabled(true);
}


void MainWindow::on_pushButton_copy_clicked() {
    // преобразуем содержимое холста в изображение
    QImage image = get_graphicsView_result_scene_image();

    // копируем изображение в буфер
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(image);

    // сообщение об успехе
    QMessageBox::information(this, "Успех", "Изображение скопировано в буфер обмена");
}


void MainWindow::on_pushButton_save_clicked() {
    // уникальное имя файла с датой и временем
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString defaultFileName = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                              + QString("/qr_%1.png").arg(timestamp);

    // получаем путь
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Сохранить файл",
        defaultFileName,
        "Image (*.png)"
        );

    if (fileName.isEmpty()) {
        return;
    }

    // преобразуем содержимое холста в изображение
    QImage image = get_graphicsView_result_scene_image();

    // сохраняем
    image.save(fileName);
}

void MainWindow::on_chooseModuleColor_clicked()
{
    QColor color = QColorDialog::getColor(moduleColor, this, "Выберите цвет модуля");
    if (color.isValid()) {
        moduleColor = color;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QColor color = QColorDialog::getColor(moduleColor, this, "Выберите цвет фона");
    if (color.isValid()) {
        backgroundColor = color;
    }
}

void MainWindow::on_plainTextEdit_data_blockCountChanged(int newBlockCount)
{

}

