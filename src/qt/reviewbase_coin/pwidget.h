// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PWIDGET_H
#define PWIDGET_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QThread>
#include "qt/reviewbase_coin/prunnable.h"

class ReviewBase_CoinGUI;
class ClientModel;
class WalletModel;

namespace Ui {
class PWidget;
}

class PWidget : public QWidget, public Runnable
{
    Q_OBJECT
public:
    explicit PWidget(ReviewBase_CoinGUI* _window = nullptr, QWidget *parent = nullptr);
    explicit PWidget(PWidget *parent = nullptr);

    void setClientModel(ClientModel* model);
    void setWalletModel(WalletModel* model);

    ReviewBase_CoinGUI* getWindow() { return this->window; }

    void run(int type) override;
    void onError(int type, QString error) override;

signals:
    void message(const QString& title, const QString& body, unsigned int style, bool* ret = nullptr);
    void showHide(bool show);
    bool execDialog(QDialog *dialog, int xDiv = 3, int yDiv = 5);

protected slots:
    virtual void changeTheme(bool isLightTheme, QString &theme);
    void onChangeTheme(bool isLightTheme, QString &theme);

protected:
    ReviewBase_CoinGUI* window = nullptr;
    ClientModel* clientModel = nullptr;
    WalletModel* walletModel = nullptr;

    virtual void loadClientModel();
    virtual void loadWalletModel();

    void showHideOp(bool show);
    bool execute(int type);
    void inform(const QString& message);
    void warn(const QString& title, const QString& message);
    bool ask(const QString& title, const QString& message);
    void showDialog(QDialog *dialog, int xDiv = 3, int yDiv = 5);
    void emitMessage(const QString& title, const QString& message, unsigned int style, bool* ret = nullptr);

    bool verifyWalletUnlocked();
    bool quitWorker(bool forceTermination);

private:
    QThread* thread = nullptr;

    void init();
};

#endif // PWIDGET_H
