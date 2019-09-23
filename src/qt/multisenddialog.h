// Copyright (c) 2017-2018 The PIVX developers
// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ReviewBase_Coin_QT_MULTISENDDIALOG_H
#define ReviewBase_Coin_QT_MULTISENDDIALOG_H

#include <QDialog>

namespace Ui
{
class MultiSendDialog;
}

class WalletModel;
class QLineEdit;
class MultiSendDialog : public QDialog
{
    Q_OBJECT
    void updateCheckBoxes();

public:
    explicit MultiSendDialog(QWidget* parent = nullptr);
    ~MultiSendDialog();
    void setModel(WalletModel* model);
    void setAddress(const QString& address);
    void setAddress(const QString& address, QLineEdit* addrEdit);
private slots:
    void on_viewButton_clicked();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_activateButton_clicked();
    void on_disableButton_clicked();
    void on_addressBookButton_clicked();

private:
    Ui::MultiSendDialog* ui;
    WalletModel* model;
};

#endif // ReviewBase_Coin_QT_MULTISENDDIALOG_H
