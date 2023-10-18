//
// Created by victor on 28/3/22.
//

#include "tstandardlabel.h"

struct tStandardLabelPrivate {
        tStandardLabel::StandardLabelType type = tStandardLabel::Normal;

        QString text;
        tStandardLabel::ElideMode elideMode = tStandardLabel::ElideRight;
};

tStandardLabel::tStandardLabel(QWidget* parent) :
    QLabel(parent) {
    d = new tStandardLabelPrivate();
}

tStandardLabel::~tStandardLabel() {
    delete d;
}

tStandardLabel::StandardLabelType tStandardLabel::type() {
    return d->type;
}

void tStandardLabel::setType(tStandardLabel::StandardLabelType type) {
    d->type = type;

    QFont font = QApplication::font();
    switch (type) {
        case Subtitle:
            {
                font.setBold(true);
                font.setCapitalization(QFont::AllUppercase);
                break;
            }
        default:
            break;
    }

    this->setFont(font);
}

void tStandardLabel::setText(QString text) {
    d->text = text;
    this->updateText();
}

QString tStandardLabel::text() {
    return d->text;
}

void tStandardLabel::setElideMode(ElideMode elideMode) {
    d->elideMode = elideMode;
}

tStandardLabel::ElideMode tStandardLabel::elideMode() {
    return d->elideMode;
}

void tStandardLabel::updateText() {
    int availableWidth = this->width();
    auto text = d->text;
    switch (d->elideMode) {
        case ElideRight:
            text = this->fontMetrics().elidedText(d->text, Qt::ElideRight, availableWidth);
            break;
        case ElideCenter:
            text = this->fontMetrics().elidedText(d->text, Qt::ElideMiddle, availableWidth);
            break;
        case ElideLeft:
            text = this->fontMetrics().elidedText(d->text, Qt::ElideLeft, availableWidth);
            break;
        case NoElide:
            break;
    }
    QLabel::setText(text);
}

void tStandardLabel::resizeEvent(QResizeEvent* event) {
    this->updateText();
}
