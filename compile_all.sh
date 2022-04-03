#!/bin/bash

skipped=0

touch compiled_tmp
touch failed_tmp

RED='\033[1;31m'
YELLOW='\033[1;33m'
GREEN='\033[1;34m'
MAGENTA='\033[1;35m'
NC='\033[0m' # No Color

echo "" > hashes.txt

hash_generic_functions=$(md5sum ./genericFunctions.h | cut -d" " -f1)

if ! grep -q $hash_generic_functions "hashes_old.txt"; then
    printf " ${MAGENTA}global header changed, recompiling all\n"
    echo "" > hashes_old.txt
fi

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
    
    ( if g++ $elem -o ./compiled_binaries_linux/$name -I $name -I . ; then
            printf "${GREEN}compiled: $name\n${NC}"
            echo -n "c" >> compiled_tmp
            echo "$hash" >> hashes.txt
        else
            printf "${RED}failed: $name\n${NC}"
            echo -n "f" >> failed_tmp
    fi ) &
done

# wait for all async compilations to finish
wait

echo "$hash_generic_functions" >> hashes.txt
cat hashes.txt > hashes_old.txt

compiled=$(wc -m < compiled_tmp)
failed=$(wc -m < failed_tmp)

rm compiled_tmp
rm failed_tmp

printf "\n${NC}$compiled ${GREEN}compiled${NC}, $skipped ${MAGENTA}up to date${NC}, $failed ${RED}failed\n"
if (($failed > 0)); then
    echo "Press any key to continue: "
    read -n 1 k <&1
fi
