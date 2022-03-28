//
// Created by victor on 28/3/22.
//

#include "tstandardlabel.h"

struct tStandardLabelPrivate {
    tStandardLabel::StandardLabelType type = tStandardLabel::Normal;
};

tStandardLabel::tStandardLabel(QWidget* parent) : QLabel(parent) {
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
        case Subtitle: {
            font.setBold(true);
            font.setCapitalization(QFont::SmallCaps);
            break;
        }
        default:
            break;
    }

    this->setFont(font);
}
