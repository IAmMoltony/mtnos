#!/usr/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NOCOLOR='\033[0m'

function quit {
    cd ..
    printf "${RED}Build failed${NOCOLOR}\n"
    exit 1
}

echo "Build started..."
cd gnu-efi
make bootloader || quit
cd ../kernel
make image || quit
cd ..
printf "${GREEN}Build success${NOCOLOR}\n"