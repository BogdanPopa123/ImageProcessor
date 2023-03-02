#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = nullptr);
    ~FilterDialog();

    float getRfilter() const;
    float getGfilter() const;
    float getBfilter() const;

signals:
    void filterValues(float r, float g, float b);

private slots:
    void onOkButtonClicked();
    void onCancelButtonClicked();

private:
    Ui::FilterDialog *ui;
};

#endif // FILTERDIALOG_H
