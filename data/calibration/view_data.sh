#!/bin/bash

diff -y -W 87 lc_1.dat lc_2.dat > diff.tmp; diff -y -W183 diff.tmp lc_drive.dat
rm diff.tmp
