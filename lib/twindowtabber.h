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

    /// Add a button to the tab bar
    /// \param button The button to add to the tab bar
    void addButton(tWindowTabberButton* button);

    /// Remove a button from the tab bar
    /// \note The button will be deleted
    /// \param button The button to remove from the tab bar
    void removeButton(tWindowTabberButton* button);

    /// Set the current button for the tab bar
    /// \param button The button to make current
    void setCurrent(tWindowTabberButton* button);

    /// Set whether to show the New Tab button
    /// \param showNewTabButton True to show the New Tab button
    void setShowNewTabButton(bool showNewTabButton);

    signals:
        void newTabRequested();

private:
    tWindowTabberPrivate* d;
    virtual bool eventFilter(QObject *watched, QEvent *event);
};

#endif//LIBCONTEMPORARY_TWINDOWTABBER_H
