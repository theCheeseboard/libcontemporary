#include "androidplatformicongenerator.h"

#include <QDir>
#include <QImage>
#include <QPainter>
#include <QSize>
#include <QSvgRenderer>

AndroidPlatformIconGenerator::AndroidPlatformIconGenerator(QObject* parent) :
    PlatformIconGenerator{parent} {
}

void AndroidPlatformIconGenerator::renderPng(QString output, QSizeF size) {
    QImage image(size.toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter imagePainter(&image);

    QSvgRenderer renderer(inputSvg().toUtf8());
    renderer.render(&imagePainter, QRect(QPoint(0, 0), size.toSize()));
    imagePainter.end();

    image.save(output);
}

void AndroidPlatformIconGenerator::generateIcon() {
    QDir androidDir(outputFile());
    QDir resDir(androidDir.absoluteFilePath("res"));

    QSizeF baseline{48, 48};
    renderPng(resDir.absoluteFilePath("drawable-ldpi/icon.png"), baseline * 0.75);
    renderPng(resDir.absoluteFilePath("drawable-mdpi/icon.png"), baseline * 1);
    renderPng(resDir.absoluteFilePath("drawable-hdpi/icon.png"), baseline * 1.5);
    renderPng(resDir.absoluteFilePath("drawable-xhdpi/icon.png"), baseline * 2);
    renderPng(resDir.absoluteFilePath("drawable-xxhdpi/icon.png"), baseline * 3);
    renderPng(resDir.absoluteFilePath("drawable-xxxhdpi/icon.png"), baseline * 4);
}
