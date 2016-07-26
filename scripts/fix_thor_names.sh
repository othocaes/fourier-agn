#!/usr/bin/env bash


for analysis in analysis/*
do
    basename $analysis|
        sed 's|A_|Å\)|g'|
        sed 's|_|\(𝛌＝|'|
        sed 's|__|\)≻|'|
        sed 's|_|\(𝛌＝|'|
        sed 's|_|≻|'|