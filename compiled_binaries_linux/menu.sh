#!/bin/bash
cd ..
./compile_all.sh
cd compiled_binaries_linux

printf "\033[0m"

FILES=$(ls -I '*.*')
CHOICES=""

for file in $FILES 
do
	CHOICES="$CHOICES $file #"
done

result=$(whiptail --title "Menu" --menu "Select a program" 25 78 16 $CHOICES 3>&1 1>&2 2>&3)

clear
./$result
clear
