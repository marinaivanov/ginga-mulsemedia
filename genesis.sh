#!/bin/bash

cd ..

echo "atualizando pacotes..."
sudo apt-get update
sudo apt-get upgrade

echo "instalando pacotes dev essenciais..."
sudo apt-get install build-essential libreadline-dev

echo "instalando cURL..."
sudo apt-get install curl

echo "instalando git..."
sudo apt-get install git

echo "Instalando servidor e cliente Mosquitto..."
sudo apt-get install mosquitto
sudo apt-get install mosquitto-clients
echo "Diretorio com Read.me do mosquitto: /usr/share/doc/mosquitto"
echo "Diretorio com .config do mosquitto: /etc/mosquitto/mosquitto.conf"

echo "Instalando biblioteca so Tobii e Gerenciador Tobii Pro.."
git clone https://github.com/Eitol/tobii_eye_tracker_linux_installer.git
cd tobii_eye_tracker_linux_installer
sudo bash ./install_all.sh
cd ..
sudo rm -r tobii_eye_tracker_linux_installer


echo "Instalando NCLua..." 
git clone https://github.com/TeleMidia/nclua.git
sudo apt-get install -y git gcc g++ autotools-dev dh-autoreconf \
  liblua5.2-dev libglib2.0-dev libpango1.0-dev \
  librsvg2-dev libgtk-3-dev libsoup2.4-dev -qq
cd nclua
./bootstrap
./configure --prefix=/usr/
make
sudo make install

cd ..

echo "Instalando Restclient para comunicacao com Moodo..."
git clone https://github.com/mrtazz/restclient-cpp.git
cd restclient-cpp
./autogen.sh
./configure
sudo make install

cd ..

echo "Instalando Ginga..." 
sudo apt-get install -y git gcc g++ autotools-dev dh-autoreconf \
    cmake cmake-data liblua5.2-dev libglib2.0-dev libpango1.0-dev \
    librsvg2-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
    gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-libav \
    libgtk-3-dev libsoup2.4-dev -qq
cd ginga-mulsemedia
./bootstrap
./configure --prefix=/usr/
make

echo "Criando diretorios de arquivos necessários para o Ginga..."
mkdir ~/gingaFiles
mkdir ~/gingaFiles/users
mkdir ~/gingaFiles/interactionConfig
cp interaction-config.json ~/gingaFiles/interactionConfig

