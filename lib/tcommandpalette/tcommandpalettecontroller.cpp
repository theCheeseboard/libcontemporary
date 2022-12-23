#include "tcommandpalettecontroller.h"

#include "tcommandpaletteactionscope.h"
#include "tcommandpalettewindow.h"
#include <QKeyEvent>
#include <QPointer>
#include <QShortcut>

struct tCommandPaletteControllerPrivate {
        QWidget* parent;
        QPointer<tCommandPaletteWindow> activeWindow;
        QAction* commandPaletteAction;

        QList<tCommandPaletteScope*> scopes;

        bool awaitingNextShift = false;
};

tCommandPaletteController::tCommandPaletteController(QWidget* parent) :
    QObject{parent} {
    d = new tCommandPaletteControllerPrivate();
    d->parent = parent;

    d->commandPaletteAction = new QAction();
    d->commandPaletteAction->setText(tr("Command Palette"));
    d->commandPaletteAction->setShortcut(QKeySequence(Qt::Key_Shift, Qt::Key_Shift));
    connect(d->commandPaletteAction, &QAction::triggered, this, &tCommandPaletteController::activate);
}

tCommandPaletteController::~tCommandPaletteController() {
    delete d;
}

tCommandPaletteController* tCommandPaletteController::defaultController(QWidget* parent, tCommandPaletteActionScope** actionScope) {
    auto controller = new tCommandPaletteController(parent);
    controller->installShortcut();
    *actionScope = new tCommandPaletteActionScope(controller, controller->commandPaletteAction());
    controller->addScope(*actionScope);
    return controller;
}

void tCommandPaletteController::addScope(tCommandPaletteScope* scope) {
    d->scopes.append(scope);
}

QList<tCommandPaletteScope*> tCommandPaletteController::scopes() {
    return d->scopes;
}

void tCommandPaletteController::activate() {
    if (!d->activeWindow) {
        d->activeWindow = new tCommandPaletteWindow(this, d->parent);
    }

    d->activeWindow->show();

    QRect geometry;
    geometry.setSize(SC_DPI_WT(QSize(600, 800), QSize, d->parent));
    geometry.moveCenter(d->parent->geometry().center());
    d->activeWindow->setGeometry(geometry);
}

void tCommandPaletteController::installShortcut() {
    //    auto shortcut = new QShortcut(QKeySequence(Qt::Key_Shift, Qt::Key_Shift), d->parent);
    //    connect(shortcut, &QShortcut::activatedAmbiguously, this, &tCommandPaletteController::activate);
    qApp->installEventFilter(this);
}

QAction* tCommandPaletteController::commandPaletteAction() {
    return d->commandPaletteAction;
}

bool tCommandPaletteController::eventFilter(QObject* watched, QEvent* event) {
    if (watched == d->parent && event->type() == QEvent::KeyPress) {
        auto ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Shift) {
            if (d->awaitingNextShift) {
                d->awaitingNextShift = false;
                this->activate();
            } else {
                d->awaitingNextShift = true;
            }
        } else {
            d->awaitingNextShift = false;
        }
    } else if (event->type() == QEvent::KeyPress) {
        auto ke = static_cast<QKeyEvent*>(event);
        if (ke->key() != Qt::Key_Shift) {
            d->awaitingNextShift = false;
        }
    }
    return false;
}
