#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Format:"
    echo "./wifi.sh <SSID> <PASSWORD>"
    echo "The result will be in wifi.png"
fi

qrencode -s 6 --background=FFFFFF00 -o "./img/wifi.png" "WIFI:T:WPA;S:$1;P:$2;;"

echo "Your wifi qrcode is wifi.png !"
echo "Your wifi qrcode is now in the local img folder as wifi.png"
