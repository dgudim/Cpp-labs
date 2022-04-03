#!/bin/bash

RED='\033[1;31m'
YELLOW='\033[1;33m'
GREEN='\033[1;34m'
MAGENTA='\033[1;35m'
NC='\033[0m' # No Color

echo "" > hashes.txt

compiled=0
failed=0
skipped=0

for elem in $(find . -type f -iregex ".*\.cpp")
do
	name=$(echo "$elem" | cut -f2 -d'/')
	printf "${YELLOW}compiling: $name\n${NC}"
	hash=$(md5sum $elem | cut -d" " -f1) 
	
	if grep -q $hash "hashes_old.txt"; then
		printf " ${MAGENTA}up-to-date, skipping\n"
		skipped=$((skipped+1))
		echo "$hash" >> hashes.txt
		continue
	fi
	
	if g++ $elem -o ./compiled_binaries_linux/$name -I $name -I . ; then
		compiled=$((compiled+1))
		printf "${GREEN}compiled: $name\n${NC}"
		echo "$hash" >> hashes.txt
	else
		failed=$((failed+1))
		printf "${RED}failed: $name\n${NC}"
	fi
done

cat hashes.txt > hashes_old.txt
printf "\n${NC}$compiled ${GREEN}compiled${NC}, $skipped ${MAGENTA}up to date${NC}, $failed ${RED}failed\n"
if (($failed > 0)); then
	echo "Press any key to continue: "
	read -n 1 k <&1
fi
