#!/bin/bash

code=$1
message=$2

lowercase=`head -n1 $code`
uppercase=`tail -n1 $code`

cat $message| tr "[$lowercase]" "[a-z]"|tr "[$uppercase]" "[A-Z]"
