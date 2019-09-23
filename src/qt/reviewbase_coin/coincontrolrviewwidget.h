// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINCONTROLRVIEWWIDGET_H
#define COINCONTROLRVIEWWIDGET_H

#include <QDialog>

namespace Ui {
class CoinControlRviewWidget;
}

class CoinControlRviewWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CoinControlRviewWidget(QWidget *parent = nullptr);
    ~CoinControlRviewWidget();

private:
    Ui::CoinControlRviewWidget *ui;
};

#endif // COINCONTROLRVIEWWIDGET_H
