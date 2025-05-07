#! /usr/bin/env bash

LC_ALL=C awk '{for (i=1; i<=NF; i++) print $i}' | sort -S 80%