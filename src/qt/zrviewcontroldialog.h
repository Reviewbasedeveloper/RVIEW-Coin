// Copyright (c) 2017-2018 The PIVX developers
// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZRVIEWCONTROLDIALOG_H
#define ZRVIEWCONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "zrview/zerocoin.h"
#include "privacydialog.h"

class CZerocoinMint;
class WalletModel;

namespace Ui {
class ZRviewControlDialog;
}

class CZRviewControlWidgetItem : public QTreeWidgetItem
{
public:
    explicit CZRviewControlWidgetItem(QTreeWidget *parent, int type = Type) : QTreeWidgetItem(parent, type) {}
    explicit CZRviewControlWidgetItem(int type = Type) : QTreeWidgetItem(type) {}
    explicit CZRviewControlWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent, type) {}

    bool operator<(const QTreeWidgetItem &other) const;
};

class ZRviewControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZRviewControlDialog(QWidget *parent);
    ~ZRviewControlDialog();

    void setModel(WalletModel* model);

    static std::set<std::string> setSelectedMints;
    static std::set<CMintMeta> setMints;
    static std::vector<CMintMeta> GetSelectedMints();

private:
    Ui::ZRviewControlDialog *ui;
    WalletModel* model;

    void updateList();
    void updateLabels();

    enum {
        COLUMN_CHECKBOX,
        COLUMN_DENOMINATION,
        COLUMN_PUBCOIN,
        COLUMN_VERSION,
        COLUMN_PRECOMPUTE,
        COLUMN_CONFIRMATIONS,
        COLUMN_ISSPENDABLE
    };
    friend class CZRviewControlWidgetItem;

private slots:
    void updateSelection(QTreeWidgetItem* item, int column);
    void ButtonAllClicked();
};

#endif // ZRVIEWCONTROLDIALOG_H
