#!/usr/bin/env bash

for analysis in analyses/*
do
    # Grab and determine labels of analyses
    ref_band=$(basename $analysis|sed 's|[^≻]*_≻_\([^≻_]*\)_[^_]*_[^_]*|\1|')
    echo_band=$(basename $analysis|sed 's|\([^≻]*\)_≻_[^≻_]*_[^_]*_[^_]*|\1|')
    timestep_str=$(basename $analysis|sed 's|[^≻]*_≻_[^≻_]*_\([^_]*\)_σ∊[CLM][MFC]|\1|')
    err_str=$(basename $analysis|sed 's|[^≻]*_≻_[^≻_]*_[^_]*_\(σ∊[CLM][MFC]\)|\1|')

    newname="${ref_band}_≺_${echo_band}_${timestep_str}_${err_str}"
    mv $analysis analyses/$newname
done