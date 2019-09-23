// Copyright (c) 2017-2019 The PIVX developers
// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "privacydialog.h"
#include "ui_privacydialog.h"

#include "addressbookpage.h"
#include "addresstablemodel.h"
#include "bitcoinunits.h"
#include "coincontroldialog.h"
#include "libzerocoin/Denominations.h"
#include "optionsmodel.h"
#include "sendcoinsentry.h"
#include "walletmodel.h"
#include "coincontrol.h"
#include "zrviewcontroldialog.h"
#include "spork.h"
#include "askpassphrasedialog.h"

#include <QClipboard>
#include <QSettings>
#include <utilmoneystr.h>
#include <QtWidgets>
#include <zrview/deterministicmint.h>
#include <zrview/accumulators.h>

PrivacyDialog::PrivacyDialog(QWidget* parent) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowCloseButtonHint),
                                                ui(new Ui::PrivacyDialog),
                                                walletModel(0),
                                                currentBalance(-1),
                                                fDenomsMinimized(true)
{
    nDisplayUnit = 0; // just make sure it's not unitialized
    ui->setupUi(this);

    // "Spending 999999 zRVIEW ought to be enough for anybody." - Bill Gates, 2017
    ui->zRVIEWpayAmount->setValidator( new QDoubleValidator(0.0, 21000000.0, 20, this) );
    //ui->labelMintAmountValue->setValidator( new QIntValidator(0, 999999, this) );     // disable MINT

    // Default texts for (mini-) coincontrol
    //ui->labelCoinControlQuantity->setText (tr("Coins automatically selected"));       // disable MINT
    //ui->labelCoinControlAmount->setText (tr("Coins automatically selected"));         // disable MINT
    ui->labelzRVIEWSyncStatus->setText("(" + tr("out of sync") + ")");

    // Sunken frame for minting messages
    ui->TEMintStatus->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    ui->TEMintStatus->setLineWidth (2);
    ui->TEMintStatus->setMidLineWidth (2);
    ui->TEMintStatus->setPlainText(tr("Mint Status: Okay"));

    // Coin Control signals
    /*                                                                            [disable MINT and coinControl]
    connect(ui->pushButtonCoinControl, SIGNAL(clicked()), this, SLOT(coinControlButtonClicked()));

    // Coin Control: clipboard actions
    QAction* clipboardQuantityAction = new QAction(tr("Copy quantity"), this);
    QAction* clipboardAmountAction = new QAction(tr("Copy amount"), this);
    connect(clipboardQuantityAction, SIGNAL(triggered()), this, SLOT(coinControlClipboardQuantity()));
    connect(clipboardAmountAction, SIGNAL(triggered()), this, SLOT(coinControlClipboardAmount()));
    ui->labelCoinControlQuantity->addAction(clipboardQuantityAction);
    ui->labelCoinControlAmount->addAction(clipboardAmountAction);
    */

    // Denomination labels
    ui->labelzDenom1Text->setText(tr("Denom. with value <b>1</b>:"));
    ui->labelzDenom2Text->setText(tr("Denom. with value <b>5</b>:"));
    ui->labelzDenom3Text->setText(tr("Denom. with value <b>10</b>:"));
    ui->labelzDenom4Text->setText(tr("Denom. with value <b>50</b>:"));
    ui->labelzDenom5Text->setText(tr("Denom. with value <b>100</b>:"));
    ui->labelzDenom6Text->setText(tr("Denom. with value <b>500</b>:"));
    ui->labelzDenom7Text->setText(tr("Denom. with value <b>1000</b>:"));
    ui->labelzDenom8Text->setText(tr("Denom. with value <b>5000</b>:"));

    // AutoMint status
    ui->label_AutoMintStatus->setText(tr("AutoMint Status:"));

    // Global Supply labels
    ui->labelZsupplyText1->setText(tr("Denom. <b>1</b>:"));
    ui->labelZsupplyText5->setText(tr("Denom. <b>5</b>:"));
    ui->labelZsupplyText10->setText(tr("Denom. <b>10</b>:"));
    ui->labelZsupplyText50->setText(tr("Denom. <b>50</b>:"));
    ui->labelZsupplyText100->setText(tr("Denom. <b>100</b>:"));
    ui->labelZsupplyText500->setText(tr("Denom. <b>500</b>:"));
    ui->labelZsupplyText1000->setText(tr("Denom. <b>1000</b>:"));
    ui->labelZsupplyText5000->setText(tr("Denom. <b>5000</b>:"));

    // ReviewBase_Coin settings
    QSettings settings;
    if (!settings.contains("fMinimizeChange")){
        fMinimizeChange = false;
        settings.setValue("fMinimizeChange", fMinimizeChange);
    }
    else{
        fMinimizeChange = settings.value("fMinimizeChange").toBool();
    }

    ui->checkBoxMinimizeChange->setChecked(fMinimizeChange);

    // Start with displaying the "out of sync" warnings
    showOutOfSyncWarning(true);

    // Hide those placeholder elements needed for CoinControl interaction
    ui->WarningLabel->hide();    // Explanatory text visible in QT-Creator
    ui->dummyHideWidget->hide(); // Dummy widget with elements to hide

    // Set labels/buttons depending on SPORK_16 status
    updateSPORK16Status();

    // init Denoms section
    if(!settings.contains("fDenomsSectionMinimized"))
        settings.setValue("fDenomsSectionMinimized", true);
    minimizeDenomsSection(settings.value("fDenomsSectionMinimized").toBool());

    ui->checkBoxMintChange->setVisible(false);
}

PrivacyDialog::~PrivacyDialog()
{
    QSettings settings;
    settings.setValue("fDenomsSectionMinimized", fDenomsMinimized);
    delete ui;
}

void PrivacyDialog::setModel(WalletModel* walletModel)
{
    this->walletModel = walletModel;

    if (walletModel && walletModel->getOptionsModel()) {
        // Keep up to date with wallet
        setBalance(walletModel->getBalance(), walletModel->getUnconfirmedBalance(), walletModel->getImmatureBalance(),
                   walletModel->getWatchBalance(), walletModel->getWatchUnconfirmedBalance(), walletModel->getWatchImmatureBalance());

        connect(walletModel, SIGNAL(balanceChanged(CAmount, CAmount, CAmount,  CAmount, CAmount, CAmount)), this,
                SLOT(setBalance(CAmount, CAmount, CAmount, CAmount, CAmount, CAmount)));
    }
}

void PrivacyDialog::on_pasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->payTo->setText(QApplication::clipboard()->text());
}

void PrivacyDialog::on_addressBookButton_clicked()
{
    if (!walletModel || !walletModel->getOptionsModel())
        return;

    AddressBookPage dlg(AddressBookPage::ForSelection, AddressBookPage::SendingTab, this);
    dlg.setModel(walletModel->getAddressTableModel());
    if (dlg.exec()) {
        ui->payTo->setText(dlg.getReturnValue());
        ui->zRVIEWpayAmount->setFocus();
    }
}

void PrivacyDialog::on_pushButtonMintReset_clicked()
{
    ui->TEMintStatus->setPlainText(tr("rescanning complete blockchain, this will need up to 30 minutes depending on your hardware.\nPlease be patient..."));
    ui->TEMintStatus->repaint ();

    int64_t nTime = GetTimeMillis();
    std::string strResetMintResult;
    double fDuration = (double)(GetTimeMillis() - nTime)/1000.0;
    ui->TEMintStatus->setPlainText(QString::fromStdString(strResetMintResult) + tr("Duration: ") + QString::number(fDuration) + tr(" sec.\n"));
    ui->TEMintStatus->repaint ();
    ui->TEMintStatus->verticalScrollBar()->setValue(ui->TEMintStatus->verticalScrollBar()->maximum()); // Automatically scroll to end of text

    return;
}


void PrivacyDialog::on_pushButtonSpentReset_clicked()
{
    ui->TEMintStatus->setPlainText(tr("Starting ResetSpentZerocoin: "));
    ui->TEMintStatus->repaint ();
    int64_t nTime = GetTimeMillis();
    double fDuration = (double)(GetTimeMillis() - nTime)/1000.0;
    ui->TEMintStatus->setPlainText( tr("Duration: ") + QString::number(fDuration) + tr(" sec.\n"));
    ui->TEMintStatus->repaint ();
    ui->TEMintStatus->verticalScrollBar()->setValue(ui->TEMintStatus->verticalScrollBar()->maximum()); // Automatically scroll to end of text

    return;
}

void PrivacyDialog::on_pushButtonSpendzRVIEW_clicked()
{

    if (!walletModel || !walletModel->getOptionsModel() || !pwalletMain)
        return;

    if(GetAdjustedTime() > GetSporkValue(SPORK_16_ZEROCOIN_MAINTENANCE_MODE)) {
        QMessageBox::information(this, tr("Mint Zerocoin"),
                                 tr("zRVIEW is currently undergoing maintenance."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    // Request unlock if wallet was locked or unlocked for mixing:
    WalletModel::EncryptionStatus encStatus = walletModel->getEncryptionStatus();
    if (encStatus == walletModel->Locked || encStatus == walletModel->UnlockedForAnonymizationOnly) {
        WalletModel::UnlockContext ctx(walletModel->requestUnlock(AskPassphraseDialog::Context::Send_zRVIEW, true));
        if (!ctx.isValid()) {
            // Unlock wallet was cancelled
            return;
        }
        return;
    }
}

void PrivacyDialog::on_pushButtonZRviewControl_clicked()
{
    if (!walletModel || !walletModel->getOptionsModel())
        return;

    ZRviewControlDialog* zRviewControl = new ZRviewControlDialog(this);
    zRviewControl->setModel(walletModel);
    zRviewControl->exec();
}

void PrivacyDialog::setZRviewControlLabels(int64_t nAmount, int nQuantity)
{
    ui->labelzRviewSelected_int->setText(QString::number(nAmount));
    ui->labelQuantitySelected_int->setText(QString::number(nQuantity));
}

static inline int64_t roundint64(double d)
{
    return (int64_t)(d > 0 ? d + 0.5 : d - 0.5);
}

void PrivacyDialog::on_payTo_textChanged(const QString& address)
{
    updateLabel(address);
}

/* DISABLE MINTs: no need for coinCointrol

// Coin Control: copy label "Quantity" to clipboard
void PrivacyDialog::coinControlClipboardQuantity()
{
    GUIUtil::setClipboard(ui->labelCoinControlQuantity->text());
}

// Coin Control: copy label "Amount" to clipboard
void PrivacyDialog::coinControlClipboardAmount()
{
    GUIUtil::setClipboard(ui->labelCoinControlAmount->text().left(ui->labelCoinControlAmount->text().indexOf(" ")));
}

// Coin Control: button inputs -> show actual coin control dialog
void PrivacyDialog::coinControlButtonClicked()
{
    if (!walletModel || !walletModel->getOptionsModel())
        return;

    CoinControlDialog dlg;
    dlg.setModel(walletModel);
    dlg.exec();
    coinControlUpdateLabels();
}

// Coin Control: update labels
void PrivacyDialog::coinControlUpdateLabels()
{
    if (!walletModel || !walletModel->getOptionsModel() || !walletModel->getOptionsModel()->getCoinControlFeatures())
        return;

     // set pay amounts
    CoinControlDialog::payAmounts.clear();

    if (CoinControlDialog::coinControl->HasSelected()) {
        // Actual coin control calculation
        CoinControlDialog::updateLabels(walletModel, this);
    } else {
        ui->labelCoinControlQuantity->setText (tr("Coins automatically selected"));
        ui->labelCoinControlAmount->setText (tr("Coins automatically selected"));
    }
}
*/

void PrivacyDialog::on_pushButtonShowDenoms_clicked()
{
    minimizeDenomsSection(false);
}

void PrivacyDialog::on_pushButtonHideDenoms_clicked()
{
    minimizeDenomsSection(true);
}

void PrivacyDialog::minimizeDenomsSection(bool fMinimize)
{
    if (fMinimize) {
        ui->balanceSupplyFrame->show();
        ui->verticalFrameRight->hide();
    } else {
        ui->balanceSupplyFrame->hide();
        ui->verticalFrameRight->show();
    }
    fDenomsMinimized = fMinimize;
}

bool PrivacyDialog::updateLabel(const QString& address)
{
    if (!walletModel)
        return false;

    // Fill in label from address book, if address has an associated label
    QString associatedLabel = walletModel->getAddressTableModel()->labelForAddress(address);
    if (!associatedLabel.isEmpty()) {
        ui->addAsLabel->setText(associatedLabel);
        return true;
    }

    return false;
}

void PrivacyDialog::setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance,
                               const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance)
{
    currentBalance = balance;
    currentUnconfirmedBalance = unconfirmedBalance;
    currentImmatureBalance = immatureBalance;
    currentWatchOnlyBalance = watchOnlyBalance;
    currentWatchUnconfBalance = watchUnconfBalance;
    currentWatchImmatureBalance = watchImmatureBalance;

    std::map<libzerocoin::CoinDenomination, CAmount> mapDenomBalances;
    std::map<libzerocoin::CoinDenomination, int> mapUnconfirmed;
    std::map<libzerocoin::CoinDenomination, int> mapImmature;
    for (const auto& denom : libzerocoin::zerocoinDenomList){
        mapDenomBalances.insert(std::make_pair(denom, 0));
        mapUnconfirmed.insert(std::make_pair(denom, 0));
        mapImmature.insert(std::make_pair(denom, 0));
    }


    int64_t nCoins = 0;
    int64_t nSumPerCoin = 0;
    int64_t nUnconfirmed = 0;
    int64_t nImmature = 0;
    QString strDenomStats, strUnconfirmed = "";

    for (const auto& denom : libzerocoin::zerocoinDenomList) {
        nCoins = libzerocoin::ZerocoinDenominationToInt(denom);
        nSumPerCoin = nCoins * mapDenomBalances.at(denom);
        nUnconfirmed = mapUnconfirmed.at(denom);
        nImmature = mapImmature.at(denom);

        strUnconfirmed = "";
        if (nUnconfirmed) {
            strUnconfirmed += QString::number(nUnconfirmed) + QString(" unconf. ");
        }
        if(nImmature) {
            strUnconfirmed += QString::number(nImmature) + QString(" immature ");
        }
        if(nImmature || nUnconfirmed) {
            strUnconfirmed = QString("( ") + strUnconfirmed + QString(") ");
        }

        strDenomStats = strUnconfirmed + QString::number(mapDenomBalances.at(denom)) + " x " +
                        QString::number(nCoins) + " = <b>" +
                        QString::number(nSumPerCoin) + " zRVIEW </b>";

        switch (nCoins) {
            case libzerocoin::CoinDenomination::ZQ_ONE:
                ui->labelzDenom1Amount->setText(strDenomStats);
                break;
            case libzerocoin::CoinDenomination::ZQ_FIVE:
                ui->labelzDenom2Amount->setText(strDenomStats);
                break;
            case libzerocoin::CoinDenomination::ZQ_TEN:
                ui->labelzDenom3Amount->setText(strDenomStats);
                break;
            case libzerocoin::CoinDenomination::ZQ_FIFTY:
                ui->labelzDenom4Amount->setText(strDenomStats);
                break;
            case libzerocoin::CoinDenomination::ZQ_ONE_HUNDRED:
                ui->labelzDenom5Amount->setText(strDenomStats);
                break;
            case libzerocoin::CoinDenomination::ZQ_FIVE_HUNDRED:
                ui->labelzDenom6Amount->setText(strDenomStats);
                break;
            case libzerocoin::CoinDenomination::ZQ_ONE_THOUSAND:
                ui->labelzDenom7Amount->setText(strDenomStats);
                break;
            case libzerocoin::CoinDenomination::ZQ_FIVE_THOUSAND:
                ui->labelzDenom8Amount->setText(strDenomStats);
                break;
            default:
                // Error Case: don't update display
                break;
        }
    }

    CAmount nLockedBalance = 0;
    if (walletModel) {
        nLockedBalance = walletModel->getLockedBalance();
    }

    // Display AutoMint status
    updateAutomintStatus();

    // Update/enable labels and buttons depending on the current SPORK_16 status
    updateSPORK16Status();

    // Display global supply
    ui->labelZsupplyAmount->setText(QString::number(chainActive.Tip()->GetZerocoinSupply()/COIN) + QString(" <b>zRVIEW </b> "));

    for (auto denom : libzerocoin::zerocoinDenomList) {
        int64_t nSupply = chainActive.Tip()->mapZerocoinSupply.at(denom);
        QString strSupply = QString::number(nSupply) + " x " + QString::number(denom) + " = <b>" +
                            QString::number(nSupply*denom) + " zRVIEW </b> ";
        switch (denom) {
            case libzerocoin::CoinDenomination::ZQ_ONE:
                ui->labelZsupplyAmount1->setText(strSupply);
                break;
            case libzerocoin::CoinDenomination::ZQ_FIVE:
                ui->labelZsupplyAmount5->setText(strSupply);
                break;
            case libzerocoin::CoinDenomination::ZQ_TEN:
                ui->labelZsupplyAmount10->setText(strSupply);
                break;
            case libzerocoin::CoinDenomination::ZQ_FIFTY:
                ui->labelZsupplyAmount50->setText(strSupply);
                break;
            case libzerocoin::CoinDenomination::ZQ_ONE_HUNDRED:
                ui->labelZsupplyAmount100->setText(strSupply);
                break;
            case libzerocoin::CoinDenomination::ZQ_FIVE_HUNDRED:
                ui->labelZsupplyAmount500->setText(strSupply);
                break;
            case libzerocoin::CoinDenomination::ZQ_ONE_THOUSAND:
                ui->labelZsupplyAmount1000->setText(strSupply);
                break;
            case libzerocoin::CoinDenomination::ZQ_FIVE_THOUSAND:
                ui->labelZsupplyAmount5000->setText(strSupply);
                break;
            default:
                // Error Case: don't update display
                break;
        }
    }
}

void PrivacyDialog::updateDisplayUnit()
{
    if (walletModel && walletModel->getOptionsModel()) {
        nDisplayUnit = walletModel->getOptionsModel()->getDisplayUnit();
        if (currentBalance != -1)
            setBalance(currentBalance, currentUnconfirmedBalance, currentImmatureBalance,
                       currentWatchOnlyBalance, currentWatchUnconfBalance, currentWatchImmatureBalance);
    }
}

void PrivacyDialog::showOutOfSyncWarning(bool fShow)
{
    ui->labelzRVIEWSyncStatus->setVisible(fShow);
}

void PrivacyDialog::keyPressEvent(QKeyEvent* event)
{
    if (event->key() != Qt::Key_Escape) // press esc -> ignore
    {
        this->QDialog::keyPressEvent(event);
    } else {
        event->ignore();
    }
}

void PrivacyDialog::updateAutomintStatus()
{
    QString strAutomintStatus = tr("AutoMint Status:");

    if (pwalletMain->isZeromintEnabled ()) {
        strAutomintStatus += tr(" <b>enabled</b>.");
    }
    else {
        strAutomintStatus += tr(" <b>disabled</b>.");
    }

    strAutomintStatus += tr(" Configured target percentage: <b>") + QString::number(pwalletMain->getZeromintPercentage()) + "%</b>";
    ui->label_AutoMintStatus->setText(strAutomintStatus);
}

void PrivacyDialog::updateSPORK16Status()
{
    // Update/enable labels, buttons and tooltips depending on the current SPORK_16 status
    //bool fButtonsEnabled =  ui->pushButtonMintzRVIEW->isEnabled();
    bool fButtonsEnabled = false;
    bool fMaintenanceMode = GetAdjustedTime() > GetSporkValue(SPORK_16_ZEROCOIN_MAINTENANCE_MODE);
    if (fMaintenanceMode && fButtonsEnabled) {
        // Mint zRVIEW
        //ui->pushButtonMintzRVIEW->setEnabled(false);
        //ui->pushButtonMintzRVIEW->setToolTip(tr("zRVIEW is currently disabled due to maintenance."));

        // Spend zRVIEW
        ui->pushButtonSpendzRVIEW->setEnabled(false);
        ui->pushButtonSpendzRVIEW->setToolTip(tr("zRVIEW is currently disabled due to maintenance."));
    } else if (!fMaintenanceMode && !fButtonsEnabled) {
        // Mint zRVIEW
        //ui->pushButtonMintzRVIEW->setEnabled(true);
        //ui->pushButtonMintzRVIEW->setToolTip(tr("PrivacyDialog", "Enter an amount of RVIEW to convert to zRVIEW", 0));

        // Spend zRVIEW
        ui->pushButtonSpendzRVIEW->setEnabled(true);
        ui->pushButtonSpendzRVIEW->setToolTip(tr("Spend Zerocoin. Without 'Pay To:' address creates payments to yourself."));
    }
}
