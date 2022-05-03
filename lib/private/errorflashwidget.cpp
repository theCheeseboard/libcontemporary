#include "errorflashwidget.h"
#include "tpaintcalculator.h"
#include "ui_errorflashwidget.h"

#include "libcontemporary_global.h"
#include "tvariantanimation.h"
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QTimer>

ErrorFlashWidget::ErrorFlashWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ErrorFlashWidget) {
    ui->setupUi(this);
    ui->warningIcon->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(SC_DPI_WT(QSize(16, 16), QSize, this)));
}

ErrorFlashWidget::~ErrorFlashWidget() {
    delete ui;
}

void ErrorFlashWidget::showError(QWidget* parent, QString message) {
    if (ErrorFlashWidget* widget = parent->property("cntp-errorflashwidget").value<ErrorFlashWidget*>()) {
        widget->ui->errorMessage->setText(message);
        return;
    }

    QMargins originalMargins = parent->contentsMargins();
    int fixedHeight = parent->sizePolicy().verticalStretch() == QSizePolicy::Fixed ? parent->height() : QWIDGETSIZE_MAX;

    ErrorFlashWidget* widget = new ErrorFlashWidget(parent);
    widget->ui->errorMessage->setText(message);
    widget->setFixedSize(QSize(parent->width(), widget->sizeHint().height()));

    parent->setProperty("cntp-errorflashwidget", QVariant::fromValue(widget));

    QRect finalGeometry;
    finalGeometry.setSize(widget->size());
    finalGeometry.moveTop(parent->height() + SC_DPI_W(9, parent));
    finalGeometry.moveLeft(0);

    widget->setGeometry(finalGeometry);
    widget->setVisible(true);
    widget->raise();

    tVariantAnimation* anim = new tVariantAnimation(widget);
    anim->setStartValue(0);
    anim->setEndValue(finalGeometry.height() + SC_DPI_W(9, parent));
    anim->setDuration(250);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    connect(anim, &tVariantAnimation::valueChanged, widget, [=](QVariant value) {
        QMargins newMargins = originalMargins;
        newMargins.setBottom(newMargins.bottom() + value.toInt());
        parent->setContentsMargins(newMargins);
        parent->setFixedHeight(finalGeometry.top() + value.toInt() - SC_DPI_W(9, parent));
    });
    connect(anim, &tVariantAnimation::finished, widget, [=] {
        QMargins newMargins = originalMargins;
        newMargins.setBottom(newMargins.bottom() + finalGeometry.height() + SC_DPI_W(9, parent));
        parent->setContentsMargins(newMargins);
        parent->setFixedHeight(finalGeometry.bottom());
    });
    anim->start(tVariantAnimation::DeleteWhenStopped);

    QTimer::singleShot(5000, widget, [=] {
        tVariantAnimation* anim = new tVariantAnimation(widget);
        anim->setStartValue(finalGeometry.height() + SC_DPI_W(9, parent));
        anim->setEndValue(0);
        anim->setDuration(250);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        connect(anim, &tVariantAnimation::valueChanged, widget, [=](QVariant value) {
            QMargins newMargins = originalMargins;
            newMargins.setBottom(newMargins.bottom() + value.toInt());
            parent->setContentsMargins(newMargins);
            parent->setFixedHeight(finalGeometry.top() + value.toInt() - SC_DPI_W(9, parent));
        });
        connect(anim, &tVariantAnimation::finished, widget, [=] {
            anim->start(tVariantAnimation::DeleteWhenStopped);
            widget->deleteLater();
            parent->setFixedHeight(fixedHeight);
            parent->setContentsMargins(originalMargins);

            parent->setProperty("cntp-errorflashwidget", QVariant::fromValue(nullptr));
        });
        anim->start(tVariantAnimation::DeleteWhenStopped);
    });
}
