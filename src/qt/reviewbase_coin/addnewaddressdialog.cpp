// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/reviewbase_coin/addnewaddressdialog.h"
#include "qt/reviewbase_coin/forms/ui_addnewaddressdialog.h"

AddNewAddressDialog::AddNewAddressDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNewAddressDialog)
{
    ui->setupUi(this);

    // Stylesheet
    this->setStyleSheet(parent->styleSheet());

    // Container

    ui->frameContainer->setProperty("cssClass", "container-dialog");

    // Title

    ui->labelTitle->setText("New Address");
    ui->labelTitle->setProperty("cssClass", "text-title-dialog");

    // Buttons

    ui->btnCancel->setProperty("cssClass", "btn-dialog-cancel");
    ui->btnSave->setText("SAVE");
    ui->btnSave->setProperty("cssClass", "btn-primary");
}

AddNewAddressDialog::~AddNewAddressDialog()
{
    delete ui;
}
