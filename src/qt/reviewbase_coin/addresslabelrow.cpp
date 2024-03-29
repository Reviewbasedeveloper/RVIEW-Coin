// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/reviewbase_coin/addresslabelrow.h"
#include "qt/reviewbase_coin/forms/ui_addresslabelrow.h"
#include "QFile"

AddressLabelRow::AddressLabelRow(bool isLightTheme, bool isHover , QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddressLabelRow)
{
    ui->setupUi(this);

    ui->lblAddress->setProperty("cssClass", "text-list-body1");
    ui->lblLabel->setProperty("cssClass", "text-list-title1");

    updateState(isLightTheme, isHover, false);
}

void AddressLabelRow::updateView(QString address, QString label){
    ui->lblAddress->setText(address);
    ui->lblLabel->setText(label);
}

void AddressLabelRow::updateState(bool isLightTheme, bool isHovered, bool isSelected){
    if(isLightTheme)
        ui->lblDivisory->setStyleSheet("background-color:#bababa");
    else
        ui->lblDivisory->setStyleSheet("background-color:#40ffffff");

     ui->btnMenu->setVisible(isHovered);
}

void AddressLabelRow::enterEvent(QEvent *) {
    ui->btnMenu->setVisible(true);
    update();
}

void AddressLabelRow::leaveEvent(QEvent *) {
    ui->btnMenu->setVisible(false);
    update();
}

AddressLabelRow::~AddressLabelRow()
{
    delete ui;
}
