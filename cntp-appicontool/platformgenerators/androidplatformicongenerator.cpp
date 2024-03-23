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

    QMap<QString, qreal> icons = {
        {"drawable-ldpi/icon.png",    0.75},
        {"drawable-mdpi/icon.png",    1   },
        {"drawable-hdpi/icon.png",    1.5 },
        {"drawable-xhdpi/icon.png",   2   },
        {"drawable-xxhdpi/icon.png",  3   },
        {"drawable-xxxhdpi/icon.png", 4   }
    };

    QSizeF baseline{48, 48};

    for (auto i = icons.constBegin(); i != icons.constEnd(); ++i) {
        QFileInfo fileInfo(resDir.absoluteFilePath(i.key()));

        if (!fileInfo.dir().exists()) {
            QDir::root().mkpath(fileInfo.dir().absolutePath());
        }

        renderPng(fileInfo.filePath(), baseline * i.value());
    }
}
