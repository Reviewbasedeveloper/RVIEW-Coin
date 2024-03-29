// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CONTACTDROPDOWNROW_H
#define CONTACTDROPDOWNROW_H

#include <QWidget>

namespace Ui {
class ContactDropdownRow;
}

class ContactDropdownRow : public QWidget
{
    Q_OBJECT

public:
    explicit ContactDropdownRow(bool isLightTheme, bool isHover, QWidget *parent = nullptr);
    ~ContactDropdownRow();

    void update(bool isLightTheme, bool isHover, bool isSelected);
    void setData(QString address, QString label);

private:
    Ui::ContactDropdownRow *ui;
};

#endif // CONTACTDROPDOWNROW_H
