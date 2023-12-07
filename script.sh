#! /bin/bash

echo ""
echo "Bienvenido, Soy un script para usar un device driver"
echo ""
echo "[*] Cargando driver y creando device ..."
echo ""
make clean && make
sudo insmod reverseDriver.ko
sudo $(awk  '/mknod/' /var/log/syslog | tail -n 1 | cut -d "]" -f 2)
echo "Terminado."
echo ""

echo "[!] Mensaje maximo 80 caracteres."
read -p "Escribe un mensaje al driver: " mensaje
echo "${mensaje}" >> /dev/device_reverso
cat /dev/device_reverso #para leer el mensaje encriptado

echo ""
echo "[*] Eliminando driver y device ..."
echo ""
sudo rm /dev/device_reverso
sudo rmmod reverseDriver
make clean
echo "Terminado."
