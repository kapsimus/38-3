#include "widget.h"

#include <QApplication>
#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QPainter>

QImage blurImage(QImage image, int blurRadius) {
    if (image.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(image));
    auto blur = new QGraphicsBlurEffect();
    blur->setBlurRadius(blurRadius/10.0);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(image.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(), QRectF(0, 0, image.width(), image.height()));
    return result;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    QLabel picture;
    QImage image;
    QSlider blurPower(Qt::Orientation::Horizontal);
    QPushButton open;
    QVBoxLayout layout(&w);
    layout.addWidget(&picture);
    layout.addWidget(&blurPower);
    layout.addWidget(&open);
    blurPower.setMaximum(100);
    picture.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    open.setText("Open Image");
    QObject::connect(&open, &QPushButton::clicked, [&picture, &image]{
        QString imagePath = QFileDialog::getOpenFileName(nullptr, "Open Imege File", "", "Image files (*.jpg *.png)");
        image.load(imagePath);
        picture.setPixmap(QPixmap::fromImage(image).scaled(picture.width(), picture.height(), Qt::KeepAspectRatio));
    });
    QObject::connect(&blurPower, &QSlider::valueChanged, [&picture, &image, &blurPower]{
        picture.setPixmap(QPixmap::fromImage(blurImage(image, blurPower.value())).scaled(picture.width(), picture.height(), Qt::KeepAspectRatio));
    });
    w.resize(800,600);
    w.show();
    return a.exec();
}
