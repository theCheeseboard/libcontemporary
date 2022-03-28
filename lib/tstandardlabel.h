//
// Created by victor on 28/3/22.
//

#ifndef LIBCONTEMPORARY_TSTANDARDLABEL_H
#define LIBCONTEMPORARY_TSTANDARDLABEL_H

#include "libcontemporary_global.h"
#include <QLabel>

struct tStandardLabelPrivate;
class LIBCONTEMPORARY_EXPORT tStandardLabel : public QLabel {
    Q_OBJECT

public:
    explicit tStandardLabel(QWidget* parent = nullptr);
    ~tStandardLabel();

    enum StandardLabelType {
        Normal,
        Subtitle
    };

    StandardLabelType type();
    void setType(StandardLabelType type);

private:
    tStandardLabelPrivate* d;
};


#endif //LIBCONTEMPORARY_TSTANDARDLABEL_H
