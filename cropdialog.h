#ifndef CROPDIALOG_H
#define CROPDIALOG_H

#include <QDialog>

namespace Ui {
class CropDialog;
}

class CropDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CropDialog(QWidget *parent = nullptr);
    ~CropDialog();

    int getInitialX() const;
    int getInitialY() const;
    int getCropWidth() const;
    int getCropHeight() const;

signals:
    void croppingValues(int cx, int cy, int width, int height);

private slots:
    void onCancelButtonClicked();

    void onOkButtonClicked();

private:
    Ui::CropDialog *ui;
};

#endif // CROPDIALOG_H
