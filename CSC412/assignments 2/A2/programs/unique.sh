#! /usr/bin/env bash

tr -s '[:space:]' '\n' | uniq -c | awk '{print $2}'
