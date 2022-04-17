#include "tapplication.h"
#include "tmessageboxbackend.h"
#include <QMessageBox>
#include <QCheckBox>

struct tMessageBoxBackendPrivate {
    QMessageBox box;
    QHash<QPushButton*, tMessageBoxButton*> buttonStorage;
};

tMessageBoxBackend::tMessageBoxBackend(QObject *parent) : QObject(parent), d(new tMessageBoxBackendPrivate) {}

tMessageBoxBackend::~tMessageBoxBackend() = default;

void tMessageBoxBackend::init(QMessageBox::Icon style,
                              const QIcon &icon,
                              const QString &titleText,
                              const QString &messageText,
                              const QString &informativeText,
                              const QString &detailedText,
                              const QString &checkboxText,
                              const QHash<tMessageBoxButton *, tMessageBoxButtonInfo *> &buttonMap) {
    if (icon.isNull()) {
        d->box.setIcon(style);
    } else {
        d->box.setIconPixmap(icon.pixmap(SC_DPI_WT(QSize(64, 64), QSize, qobject_cast<QWidget *>(parent()))));
    }

    d->box.setWindowTitle(titleText);
    d->box.setText(messageText);
    d->box.setInformativeText(informativeText);
    d->box.setDetailedText(detailedText);
    if (!checkboxText.isEmpty()) {
        auto checkbox = new QCheckBox();
        d->box.setCheckBox(checkbox);
        checkbox->setText(checkboxText);
    }

    for (auto it = buttonMap.constKeyValueBegin(); it != buttonMap.constKeyValueEnd(); it++) {
        auto tButton = it->first;
        auto info = it->second;
        QPushButton *pushButton;
        if (info->label.isEmpty()) {
            pushButton = d->box.addButton(info->buttonType);
        } else {
            pushButton = d->box.addButton(info->label, info->buttonStyle);
        }

        connect(pushButton, &QPushButton::clicked, this, [=]() {
            emit tButton->buttonPressed(d->box.checkBox() ? d->box.checkBox()->isChecked() : false);
        });

        d->buttonStorage.insert(pushButton, tButton);
    }
}

void tMessageBoxBackend::open(QWidget *parent) {
    d->box.setParent(parent);
    d->box.setWindowFlags(Qt::Dialog);
    connect(&d->box, &QMessageBox::finished, this, &tMessageBoxBackend::canBeDestroyed);
    d->box.open();
}
