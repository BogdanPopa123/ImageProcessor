#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);

    connect(ui->okButton, &QPushButton::clicked, this, &FilterDialog::onOkButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &FilterDialog::onCancelButtonClicked);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::onOkButtonClicked()
{
    float r = ui->redSpinBox->value();
    float g = ui->greenSpinBox->value();
    float b = ui->blueSpinBox->value();
    emit filterValues(r, g, b);
    accept();
}


void FilterDialog::onCancelButtonClicked()
{
    reject();
}

float FilterDialog::getRfilter() const{
    return ui->redSpinBox->value();
}

float FilterDialog::getGfilter() const{
    return ui->greenSpinBox->value();
}

float FilterDialog::getBfilter() const{
    return ui->blueSpinBox->value();
}

