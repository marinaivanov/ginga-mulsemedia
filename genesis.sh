#!/bin/bash

cd ..

echo "updating packages..."
sudo apt-get update
sudo apt-get upgrade
echo

echo "installing essencials dev packages..."
sudo apt-get install build-essential libreadline-dev
echo

echo "installing cURL..."
sudo apt-get install curl
sudo apt-get install libcurl4-openssl-dev
echo

echo "installing git..."
sudo apt-get install git
echo

echo "removing lo longer required packages..."
sudo apt autoremove
echo

echo "installing pkg-config..."
sudo apt-get install pkg-config
echo

echo "installing autoconf..."
sudo apt-get install autoconf
echo

echo "installing Mosquitto's server and client..."
sudo apt-get install mosquitto
sudo apt-get install mosquitto-clients
echo "Mosquitto's file Read.me is in /usr/share/doc/mosquitto"
echo "Mosquitto's file .config is in /etc/mosquitto/mosquitto.conf"
echo

echo "installing Tobii library and the manager Tobii Pro.."
git clone https://github.com/Eitol/tobii_eye_tracker_linux_installer.git
cd tobii_eye_tracker_linux_installer
sudo bash ./install_all.sh
cd ..
sudo rm -r tobii_eye_tracker_linux_installer
echo
echo "correcting dependencies..."
sudo apt --fix-broken install
echo

echo "installing NCLua..." 
git clone https://github.com/TeleMidia/nclua.git
sudo apt-get install -y git gcc g++ autotools-dev dh-autoreconf \
  liblua5.2-dev libglib2.0-dev libpango1.0-dev \
  librsvg2-dev libgtk-3-dev libsoup2.4-dev -qq
cd nclua
./bootstrap
./configure --prefix=/usr/
make
sudo make install
echo

cd ..

echo "installing Restclient for communication with Moodo..."
git clone https://github.com/mrtazz/restclient-cpp.git
cd restclient-cpp
./autogen.sh
./configure
sudo make install
echo

cd ..

echo "installing Ginga..." 
sudo apt-get install -y git gcc g++ autotools-dev dh-autoreconf \
    cmake cmake-data liblua5.2-dev libglib2.0-dev libpango1.0-dev \
    librsvg2-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
    gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-libav \
    libgtk-3-dev libsoup2.4-dev -qq
cd ginga-mulsemedia
./bootstrap
./configure --prefix=/usr/
make
echo

echo "Creating file directories needed to running the Ginga..."
mkdir ~/gingaFiles
mkdir ~/gingaFiles/users
mkdir ~/gingaFiles/interactionConfig
cp interaction-config.json ~/gingaFiles/interactionConfig
echo

cd ..

