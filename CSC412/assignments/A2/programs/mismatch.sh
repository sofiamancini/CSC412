#! /usr/bin/env bash

DICT_FILE="unix_dict.text"

if [ ! -f $DICT_FILE ]; then
    exit 1
fi

while IFS= read -r word; do
    if ! grep -Fxq "$word" $DICT_FILE; then
        echo "$word"
    fi
done