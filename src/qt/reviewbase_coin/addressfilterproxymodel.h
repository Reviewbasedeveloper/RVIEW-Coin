// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ReviewBase_Coin_CORE_NEW_GUI_ADDRESSFILTERPROXYMODEL_H
#define ReviewBase_Coin_CORE_NEW_GUI_ADDRESSFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "addresstablemodel.h"


class AddressFilterProxyModel final : public QSortFilterProxyModel
{
    const QString m_type;

public:
    AddressFilterProxyModel(const QString& type, QObject* parent)
            : QSortFilterProxyModel(parent)
            , m_type(type) {
        setDynamicSortFilter(true);
        setFilterCaseSensitivity(Qt::CaseInsensitive);
        setSortCaseSensitivity(Qt::CaseInsensitive);
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

protected:
    bool filterAcceptsRow(int row, const QModelIndex& parent) const override;
};


#endif //ReviewBase_Coin_CORE_NEW_GUI_ADDRESSFILTERPROXYMODEL_H
