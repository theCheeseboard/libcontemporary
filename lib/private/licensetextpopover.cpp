#include "licensetextpopover.h"
#include "ui_licensetextpopover.h"

#include <QFile>

LicenseTextPopover::LicenseTextPopover(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::LicenseTextPopover) {
    ui->setupUi(this);

    Q_INIT_RESOURCE(libcontemporary_licenses);

    QFile file(":/the-libs/licenses/gpl3.html");
    file.open(QFile::ReadOnly);
    ui->textBrowser->setHtml(file.readAll());
}

LicenseTextPopover::~LicenseTextPopover() {
    delete ui;
}

void LicenseTextPopover::on_titleLabel_backButtonClicked() {
    emit done();
}
