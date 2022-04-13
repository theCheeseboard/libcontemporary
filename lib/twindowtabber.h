//
// Created by Victor Tran on 13/4/2022.
//

#ifndef LIBCONTEMPORARY_TWINDOWTABBER_H
#define LIBCONTEMPORARY_TWINDOWTABBER_H

#include <QWidget>
#include "twindowtabberbutton.h"
#include "libcontemporary_global.h"

struct tWindowTabberPrivate;
class LIBCONTEMPORARY_EXPORT tWindowTabber : public QWidget {
    Q_OBJECT

public:
    explicit tWindowTabber(QWidget* parent = nullptr);
    virtual ~tWindowTabber();

    void addButton(tWindowTabberButton* button);
    void setCurrent(tWindowTabberButton* button);
    virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    tWindowTabberPrivate* d;
};

#endif//LIBCONTEMPORARY_TWINDOWTABBER_H
