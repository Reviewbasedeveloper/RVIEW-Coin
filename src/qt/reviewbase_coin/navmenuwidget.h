// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NAVMENUWIDGET_H
#define NAVMENUWIDGET_H

#include <QWidget>

class ReviewBase_CoinGUI;

namespace Ui {
class NavMenuWidget;
}

class NavMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavMenuWidget(ReviewBase_CoinGUI* mainWindow, QWidget *parent = nullptr);
    ~NavMenuWidget();

public slots:
    void selectSettings();

private slots:
    void onSendClicked();
    void onDashboardClicked();
    void onAddressClicked();
    void onMasterNodesClicked();
    void onSettingsClicked();
    void onReceiveClicked();
    void updateButtonStyles();
private:
    Ui::NavMenuWidget *ui;
    ReviewBase_CoinGUI* window;
    QList<QWidget*> btns;

    void connectActions();
    void onNavSelected(QWidget* active, bool startup = false);
};

#endif // NAVMENUWIDGET_H
