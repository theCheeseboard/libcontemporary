#ifndef TST_TRANGE_H
#define TST_TRANGE_H

#include <QObject>

class TRange : public QObject {
        Q_OBJECT
    public:
        TRange();
        ~TRange();

    private slots:
        void trange_canIterate();
        void trange_canMap();
        void trange_canMapToDifferentType();
        void trange_canFilter();
        void trange_canCompose();
        void trange_canReturn();
};

#endif // TST_TRANGE_H