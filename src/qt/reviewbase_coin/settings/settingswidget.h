// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include "qt/reviewbase_coin/pwidget.h"
#include "qt/reviewbase_coin/settings/settingsbackupwallet.h"
#include "qt/reviewbase_coin/settings/settingsbittoolwidget.h"
#include "qt/reviewbase_coin/settings/settingssignmessagewidgets.h"
#include "qt/reviewbase_coin/settings/settingswalletrepairwidget.h"
#include "qt/reviewbase_coin/settings/settingswalletoptionswidget.h"
#include "qt/reviewbase_coin/settings/settingsmainoptionswidget.h"
#include "qt/reviewbase_coin/settings/settingsdisplayoptionswidget.h"
#include "qt/reviewbase_coin/settings/settingsmultisendwidget.h"
#include "qt/reviewbase_coin/settings/settingsinformationwidget.h"
#include "qt/reviewbase_coin/settings/settingsconsolewidget.h"

class ReviewBase_CoinGUI;

QT_BEGIN_NAMESPACE
class QDataWidgetMapper;
QT_END_NAMESPACE

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public PWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(ReviewBase_CoinGUI* parent);
    ~SettingsWidget();

    void loadClientModel() override;
    void loadWalletModel() override;
    void setMapper();
    void showDebugConsole();

signals:
    /** Get restart command-line parameters and handle restart */
    void handleRestart(QStringList args);

private slots:
    // File
    void onFileClicked();
    void onBackupWalletClicked();
    void onSignMessageClicked();

    // Wallet Configuration
    void onConfigurationClicked();
    void onBipToolClicked();
    void onMultisendClicked();

    // Options
    void onOptionsClicked();
    void onMainOptionsClicked();
    void onWalletOptionsClicked();
    void onDisplayOptionsClicked();

    void onDiscardChanges();

    // Tools
    void onToolsClicked();
    void onInformationClicked();
    void onDebugConsoleClicked();
    void onWalletRepairClicked();

    // Help
    void onHelpClicked();
    void onAboutClicked();

    void onResetAction();
    void onSaveOptionsClicked();
private:
    Ui::SettingsWidget *ui;

    SettingsBackupWallet *settingsBackupWallet;
    SettingsBitToolWidget *settingsBitToolWidget;
    SettingsSignMessageWidgets *settingsSingMessageWidgets;
    SettingsWalletRepairWidget *settingsWalletRepairWidget;
    SettingsWalletOptionsWidget *settingsWalletOptionsWidget;
    SettingsMainOptionsWidget *settingsMainOptionsWidget;
    SettingsDisplayOptionsWidget *settingsDisplayOptionsWidget;
    SettingsMultisendWidget *settingsMultisendWidget;
    SettingsInformationWidget *settingsInformationWidget;
    SettingsConsoleWidget *settingsConsoleWidget;

    QDataWidgetMapper* mapper;

    QList<QPushButton*> options;

    void selectOption(QPushButton* option);
    void openStandardDialog(QString title = "", QString body = "", QString okBtn = "OK", QString cancelBtn = "");
};

#endif // SETTINGSWIDGET_H
