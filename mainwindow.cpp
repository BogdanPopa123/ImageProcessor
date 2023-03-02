#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QString>
#include <QVector>
#include <QColor>
#include <QTransform>
#include "FilterDialog.h"
#include "CropDialog.h"
#include <QRect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_4_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//                                                    "C:\\Users\\Asus\\Desktop",
//                                                    tr("Images (*.png *.xpm *.jpg)"));

    QString fileName="C:\\Users\\Asus\\Desktop\\leaves-7770035_640.jpg";

    if(!fileName.isEmpty()){

        QImage image(fileName);
        QPixmap map = QPixmap::fromImage(image);

//        int w = ui->imageLabel->width();
//        int h = ui->imageLabel->height();
//        ui->imageLabel->setPixmap(map.scaled(w, h, Qt::KeepAspectRatio));

        int imagew = image.width();
        int imageh = image.height();

        if(this->initialLabelh<0){
            this->initialLabelh = ui->imageLabel->height();
        }

        if(this->initialLabelw<0){
            this->initialLabelw = ui->imageLabel->width();
        }



        if(imagew <= ui->imageLabel->maximumWidth() && imageh <= ui->imageLabel->maximumHeight()){
            ui->imageLabel->setFixedWidth(imagew);
            ui->imageLabel->setFixedHeight(imageh);
            ui->imageLabel->setPixmap(map);
        }
        else{
            int labelw = this->initialLabelw;
            int labelh = this->initialLabelh;
            ui->imageLabel->setPixmap(map.scaled(labelw, labelh, Qt::KeepAspectRatio));
        }

        ui->dims->setText(QString::fromStdString("W: " + std::to_string(imagew) + "  H: " + std::to_string(imageh)));
        ui->initials->setText(QString::fromStdString("W: " + std::to_string(this->initialLabelw) + "  H: " + std::to_string(this->initialLabelh)));


    }
}


void MainWindow::on_pushButton_2_clicked()
{
    QPixmap avgGrayscaleMap;
    QPixmap imageMap = ui->imageLabel->pixmap();
    QImage image = imageMap.toImage();
    QImage avgGrayscaleImage(image.width(), image.height(), QImage::Format_RGB32);
    QRgb value;
    int cols = imageMap.width();
    int rows = imageMap.height();

    int r, g, b, avg, i, j;
    for (i = 0; i < rows; i++){
        for (j = 0; j < cols; j++){
            QColor currentColor(image.pixel(j, i));
            r = currentColor.red();
            g = currentColor.green();
            b = currentColor.blue();
            avg = (r+g+b)/3;
            value = qRgb(avg, avg, avg);
            avgGrayscaleImage.setPixel(j, i, value);
        }
    }

    avgGrayscaleMap = QPixmap::fromImage(avgGrayscaleImage);
    ui->imageLabel->setPixmap(avgGrayscaleMap);
}


void MainWindow::on_pushButton_3_clicked()
{
    QPixmap lumenGrayscaleMap;
    QPixmap imageMap = ui->imageLabel->pixmap();
    QImage image = imageMap.toImage();
    QImage lumenGrayscaleImage(image.width(), image.height(), QImage::Format_RGB32);
    QRgb value;
    int cols = imageMap.width();
    int rows = imageMap.height();

    int r, g, b, i, j, gray;
    for (i = 0; i < rows; i++){
        for (j = 0; j < cols; j++){
            QColor currentColor(image.pixel(j, i));
            r = currentColor.red();
            g = currentColor.green();
            b = currentColor.blue();
            gray = 0.2126*r + 0.7152*g + 0.0722*b;
            value = qRgb(gray, gray, gray);
            lumenGrayscaleImage.setPixel(j, i, value);
        }
    }

    lumenGrayscaleMap = QPixmap::fromImage(lumenGrayscaleImage);
    ui->imageLabel->setPixmap(lumenGrayscaleMap);
}


void MainWindow::on_pushButton_4_clicked()
{
    FilterDialog dialog;
    if(dialog.exec() == QDialog::Accepted){
        float rfilter = dialog.getRfilter();
        float gfilter = dialog.getGfilter();
        float bfilter = dialog.getBfilter();

        QImage image = ui->imageLabel->pixmap().toImage();
        QImage filteredImage(image.width(), image.height(), QImage::Format_RGB32);
        QRgb value;
        int cols = image.width();
        int rows = image.height();

        int i, j, r, g, b;
        for (i = 0; i < rows; i++){
            for (j = 0; j < cols; j++){
                QColor currentColor(image.pixel(j, i));
                r = currentColor.red() * rfilter;
                g = currentColor.green() * gfilter;
                b = currentColor.black() * bfilter;
                value=qRgb(r, g, b);
                filteredImage.setPixel(j, i, value);
            }
        }

        QPixmap filteredMap = QPixmap::fromImage(filteredImage);
        ui->imageLabel->setPixmap(filteredMap);
    }
}


void applyFilter(const QImage& sourceImage, QImage& targetImage, const float* kernel, const int kernelSize);

void MainWindow::on_pushButton_5_clicked()
{
    QImage image = ui->imageLabel->pixmap().toImage();
    QImage resultImage(image.size(), QImage::Format_ARGB32);
    int kernelSize = 3;
    float kernel[kernelSize * kernelSize]={1/16.0, 2/16.0, 1/16.0,
                                           2/16.0, 4/16.0, 2/16.0,
                                           1/16.0, 2/16.0, 1/16.0};
    applyFilter(image, resultImage, kernel, kernelSize);
    ui->imageLabel->setPixmap(QPixmap::fromImage(resultImage));
}


void MainWindow::on_pushButton_6_clicked()
{
    QImage image = ui->imageLabel->pixmap().toImage();
    QImage resultImage(image.size(), QImage::Format_ARGB32);
    int kernelSize = 3;
    float kernel[kernelSize * kernelSize]={0, -1, 0,
                                           -1, 4, -1,
                                           0, -1, 0};
    applyFilter(image, resultImage, kernel, kernelSize);
    ui->imageLabel->setPixmap(QPixmap::fromImage(resultImage));
}



void applyFilter(const QImage& sourceImage, QImage& targetImage, const float* kernel, const int kernelSize){
    // Compute half the kernel size (assuming an odd kernel size)
    const int radius = kernelSize / 2;

    // Traverse each pixel in the image
    for (int y = radius; y < sourceImage.height() - radius; ++y) {
        for (int x = radius; x < sourceImage.width() - radius; ++x) {
            // Compute the weighted sum of the surrounding pixels
            float sumR = 0, sumG = 0, sumB = 0;
            for (int j = -radius; j <= radius; ++j) {
                for (int i = -radius; i <= radius; ++i) {
                    QRgb pixel = sourceImage.pixel(x + i, y + j);
                    float weight = kernel[(j + radius) * kernelSize + (i + radius)];
                    sumR += qRed(pixel) * weight;
                    sumG += qGreen(pixel) * weight;
                    sumB += qBlue(pixel) * weight;
                }
            }

            // Normalize the sum and set the result pixel
            int red = static_cast<int>(sumR + 0.5);
            int green = static_cast<int>(sumG + 0.5);
            int blue = static_cast<int>(sumB + 0.5);
            red = qBound(0, red, 255);
            green = qBound(0, green, 255);
            blue = qBound(0, blue, 255);
            targetImage.setPixel(x, y, qRgb(red, green, blue));
        }
    }
}






void MainWindow::on_pushButton_7_clicked()
{
    QImage image = ui->imageLabel->pixmap().toImage();
    QImage processedImage(image.width(), image.height(), QImage::Format_RGB32);
    QRgb value1, value2;
    int cols = image.width();
    int rows = image.height();
    int i, j, r1, g1, b1, r2, g2, b2;
    for (i = 0; i < rows; i++){
        for (j = 0; j <= cols/2; j++){
            QColor color1(image.pixel(j, i));
            QColor color2(image.pixel(cols-1-j,i));

            r1=color1.red();
            g1=color1.green();
            b1=color1.blue();
            r2=color2.red();
            g2=color2.green();
            b2=color2.blue();


            value1 = qRgb(r1, g1, b1);
            processedImage.setPixel(cols-1-j, i, value1);
            value2 = qRgb(r2, g2, b2);
            processedImage.setPixel(j, i, value2);
        }
    }
    ui->imageLabel->setPixmap(QPixmap::fromImage(processedImage));
}


void MainWindow::on_pushButton_8_clicked()
{
    QImage image = ui->imageLabel->pixmap().toImage();
    QImage processedImage(image.width(), image.height(), QImage::Format_RGB32);
    QRgb value1, value2;
    int cols = image.width();
    int rows = image.height();
    int i, j, r1, g1, b1, r2, g2, b2;
    for (i = 0; i < cols; i++){
        for (j = 0; j <= rows/2; j++){
            QColor color1(image.pixel(i, j));
            QColor color2(image.pixel(i, rows-1-j));

            r1=color1.red();
            g1=color1.green();
            b1=color1.blue();
            r2=color2.red();
            g2=color2.green();
            b2=color2.blue();


            value1 = qRgb(r1, g1, b1);
            processedImage.setPixel(i ,rows-1-j, value1);
            value2 = qRgb(r2, g2, b2);
            processedImage.setPixel(i, j, value2);
        }
    }
    ui->imageLabel->setPixmap(QPixmap::fromImage(processedImage));
}


void MainWindow::on_pushButton_9_clicked()
{
    QImage image = ui->imageLabel->pixmap().toImage();
    QImage resultImage = image.transformed(QTransform().rotate(90), Qt::FastTransformation);
    ui->imageLabel->setPixmap(QPixmap::fromImage(resultImage));

}


void MainWindow::on_pushButton_10_clicked()
{
    CropDialog dialog;
    if(dialog.exec() == QDialog::Accepted){

        int cx = dialog.getInitialX();
        int cy = dialog.getInitialY();
        int cropWidth = dialog.getCropWidth();
        int cropHeight = dialog.getCropHeight();

        QImage image = ui->imageLabel->pixmap().toImage();
        QRect cropRect(cx, cy, cropWidth, cropHeight);
        QImage croppedImage = image.copy(cropRect);

        QPixmap croppedMap = QPixmap::fromImage(croppedImage);
        ui->imageLabel->setPixmap(croppedMap);
    }
}

