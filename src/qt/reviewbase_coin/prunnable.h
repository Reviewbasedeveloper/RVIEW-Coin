// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ReviewBase_Coin_CORE_NEW_GUI_PRUNNABLE_H
#define ReviewBase_Coin_CORE_NEW_GUI_PRUNNABLE_H

class Runnable {
public:
    virtual void run(int type) = 0;
    virtual void onError(int type, QString error) = 0;
};

#endif //ReviewBase_Coin_CORE_NEW_GUI_PRUNNABLE_H
