#ifndef ANDROIDPLATFORMICONGENERATOR_H
#define ANDROIDPLATFORMICONGENERATOR_H

#include "platformicongenerator.h"

class AndroidPlatformIconGenerator : public PlatformIconGenerator {
        Q_OBJECT
    public:
        explicit AndroidPlatformIconGenerator(QObject* parent = nullptr);

    signals:

    private:
        void renderPng(QString output, QSizeF size);

        // PlatformIconGenerator interface
    public:
        void generateIcon();
};

#endif // ANDROIDPLATFORMICONGENERATOR_H
