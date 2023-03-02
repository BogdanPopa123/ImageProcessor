#include "cropdialog.h"
#include "ui_cropdialog.h"

CropDialog::CropDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CropDialog)
{
    ui->setupUi(this);

    connect(ui->okButton, &QPushButton::clicked, this, &CropDialog::onOkButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &CropDialog::onCancelButtonClicked);
}

CropDialog::~CropDialog()
{
    delete ui;
}

void CropDialog::onCancelButtonClicked()
{
    reject();
}


void CropDialog::onOkButtonClicked()
{
    int cx = ui->initialX->value();
    int cy = ui->initialY->value();
    int width = ui->cropWidth->value();
    int height = ui->cropHeight->value();
    emit croppingValues(cx, cy, width, height);
    accept();
}

int CropDialog::getInitialX() const{
    return ui->initialX->value();
}

int CropDialog::getInitialY() const{
    return ui->initialY->value();
}

int CropDialog::getCropWidth() const{
    return ui->cropWidth->value();
}

int CropDialog::getCropHeight() const{
    return ui->cropHeight->value();
}

