
Debian
====================
This directory contains files used to package reviewbase_coind/reviewbase_coin-qt
for Debian-based Linux systems. If you compile reviewbase_coind/reviewbase_coin-qt yourself, there are some useful files here.

## reviewbase_coin: URI support ##


reviewbase_coin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install reviewbase_coin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your reviewbase_coin-qt binary to `/usr/bin`
and the `../../share/pixmaps/reviewbase_coin128.png` to `/usr/share/pixmaps`

reviewbase_coin-qt.protocol (KDE)

