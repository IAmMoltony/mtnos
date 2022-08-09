#!/usr/bin/bash

function quit {
    cd ..
    exit 1
}

cd gnu-efi
make bootloader || quit
cd ../kernel
make image || quit
cd ..