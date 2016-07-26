#!/usr/bin/env bash

timestep="0.1"
lc_dir="data/STORM_III/lightcurves/Δt＝${timestep}"
ref_band='HST(𝛌＝1367Å)'
# Parameters for HST(𝛌＝1367Å) from MC results
refpsd_params="6.533e-02 -9.694e-02 -1.175e+00 -1.525e+00 -2.166e+00 -2.492e+00 -3.258e+00 -9.328e+00"
# Parameters for HST(𝛌＝1367Å) from covariance matrix estimates
#refpsd_params="7.376e-02 -1.976e-01 -1.182e+00 -1.521e+00 -2.144e+00 -2.503e+00 -3.580e+00 -1.233e+01"

ref_curve="${lc_dir}/${ref_band}.lc"
error_type="2"
# error types:
#   0 for covariance matrix, 1 for likelihood function, 2 for monte carlo
case $error_type in 
    "0") err_src="CM";;
    "1") err_src="LF";;
    "2") err_src="MC";;
esac

if [[ $1 == "thor" ]]
then
    echo Setting up Thor environment.
    mkdir -p thor
    mkdir -p thor/arguments
    if [[ -e thor/submissions ]]; then echo "" >> thor/submissions; fi
    date >> thor/submissions
    echo "─────────────────────────────" >> thor/submissions
fi

mkdir -p analyses

echo Using lightcurves in $lc_dir.

for echo_curve in $lc_dir/*
do
    # Determine band and inputs for band
    echo_band=$(basename $echo_curve|sed 's@\(.*\)\.lc@\1@')
    echo -n $(date "+%R")\: Running psdlag for $echo_band against $ref_band.
    outputfile="analyses/${echo_band}_≻_${ref_band}_Δt＝${timestep}_σ∊${err_src}"
    case $echo_band in
        "g(𝛌＝4775Å)")
            initial_params="$refpsd_params -9.745e-01 -1.384e+00 -2.748e+00 -3.305e+00 -3.314e+00 -3.389e+00 -4.198e+00 -4.465e+00 -4.700e-01 -7.487e-01 -2.046e+00 -2.428e+00 -2.953e+00 -3.086e+00 -3.761e+00 -4.290e+00 9.862e-02 3.899e-01 8.650e-01 5.516e-01 2.228e-01 9.508e-01 -2.872e-01 9.059e-02"
        ;;
        "HST(𝛌＝1158Å)")
            initial_params="$refpsd_params 2.504e-01 3.210e-02 -9.091e-01 -1.226e+00 -1.790e+00 -2.227e+00 -2.929e+00 -5.131e+00 1.618e-01 -8.299e-02 -1.045e+00 -1.370e+00 -1.952e+00 -2.338e+00 -3.075e+00 -3.742e+00 -3.301e-02 -7.353e-02 -7.112e-02 -5.938e-02 -4.394e-02 -1.756e-01 -2.617e-01 -1.425e-01"
        ;;
        "HST(𝛌＝1367Å)")
            initial_params="$refpsd_params 6.534e-02 -9.689e-02 -1.175e+00 -1.525e+00 -2.167e+00 -2.492e+00 -3.258e+00 -1.115e+01 6.378e-02 -9.676e-02 -1.181e+00 -1.520e+00 -2.180e+00 -2.532e+00 -3.129e+00 -3.794e+00 -8.535e-05 -5.126e-04 -4.066e-04 1.080e-03 -2.682e-03 -5.462e-03 -2.209e-02 1.564e-01"
        ;;
        "HST(𝛌＝1479Å)")
            initial_params="$refpsd_params 1.224e-01 -7.607e-02 -1.174e+00 -1.500e+00 -2.075e+00 -2.554e+00 -3.337e+00 -1.152e+01 9.340e-02 -8.842e-02 -1.173e+00 -1.512e+00 -2.113e+00 -2.547e+00 -3.178e+00 -7.000e+00 -6.035e-02 2.044e-02 1.258e-01 5.764e-02 2.580e-02 -4.737e-02 9.579e-02 6.411e-01"
        ;;
        "HST(𝛌＝1746Å)")
            initial_params="$refpsd_params 3.934e-02 -1.638e-01 -1.339e+00 -1.655e+00 -2.232e+00 -2.793e+00 -4.025e+00 -1.072e+01 5.043e-02 -1.304e-01 -1.254e+00 -1.587e+00 -2.207e+00 -2.693e+00 -3.209e+00 -3.820e+00 3.040e-02 2.438e-02 1.450e-01 1.458e-01 1.772e-01 2.076e-01 -1.390e-01 3.929e-01"
        ;;
        "i(𝛌＝7647Å)")
            initial_params="$refpsd_params -1.510e+00 -1.872e+00 -3.441e+00 -3.722e+00 -4.794e+00 -4.336e+00 -4.644e+00 -5.061e+00 -7.402e-01 -9.967e-01 -2.393e+00 -2.723e+00 -3.701e+00 -3.742e+00 -4.317e+00 -4.979e+00 2.638e-01 7.095e-01 1.297e+00 1.035e+00 9.267e-01 8.982e-01 7.318e-01 9.092e-01"
        ;;
        "I(𝛌＝8560Å)")
            initial_params="$refpsd_params -1.497e+00 -1.786e+00 -3.555e+00 -4.203e+00 -3.826e+00 -4.096e+00 -4.304e+00 -1.109e+01 -7.542e-01 -9.532e-01 -2.397e+00 -3.214e+00 -3.008e+00 -3.489e+00 -4.353e+00 -4.061e+00 1.675e-01 7.660e-01 1.781e+00 1.971e+00 6.572e-01 8.520e-01 2.405e+00 -1.573e-01"
        ;;
        "r(𝛌＝6175Å)")
            initial_params="$refpsd_params -1.372e+00 -1.786e+00 -3.214e+00 -3.558e+00 -4.416e+00 -4.366e+00 -4.559e+00 -4.586e+00 -6.693e-01 -5.998e+00 -2.390e+00 -2.927e+00 -3.411e+00 -3.731e+00 -3.930e+00 -7.000e+00 2.905e-01 1.835e+00 8.760e-01 1.476e+00 -3.280e-01 1.439e+00 9.464e-02 1.532e+00"
        ;;
        "R(𝛌＝6439Å)")
            initial_params="$refpsd_params -1.287e+00 -1.580e+00 -2.879e+00 -3.569e+00 -3.661e+00 -4.471e+00 -3.998e+00 -5.001e+00 -6.583e-01 -6.900e+00 -2.060e+00 -2.727e+00 -2.954e+00 -3.654e+00 -4.406e+00 -5.203e+00 1.448e-01 -1.004e-01 9.754e-01 1.140e+00 9.284e-01 1.116e+00 2.092e+00 8.634e-01"
        ;;
        "Swiftb(𝛌＝4392Å)")
            initial_params="$refpsd_params -5.521e-01 -1.135e+00 -2.691e+00 -3.626e+00 -4.510e+00 -1.036e+01 -1.076e+01 -1.213e+01 -2.515e-01 -6.156e-01 -1.924e+00 -2.417e+00 -3.053e+00 -6.570e+00 -3.450e+00 -6.333e+00 7.408e-02 3.089e-01 5.057e-01 4.831e-01 5.779e-01 6.277e-01 4.823e-01 6.110e-01"
        ;;
        "Swiftu(𝛌＝3465Å)")
            initial_params="$refpsd_params -3.178e-01 -7.537e-01 -2.402e+00 -2.624e+00 -3.413e+00 -3.834e+00 -1.236e+01 -1.071e+01 -1.376e-01 -4.256e-01 -1.794e+00 -2.123e+00 -2.817e+00 -3.029e+00 -4.139e+00 -7.000e+00 7.840e-02 3.388e-01 6.616e-01 1.349e-01 9.909e-01 8.538e-01 1.397e+00 6.412e-01"
        ;;
        "Swiftv(𝛌＝5468Å)")
            initial_params="$refpsd_params -8.802e-01 -1.441e+00 -2.883e+00 -4.483e+00 -1.045e+01 -1.041e+01 -1.042e+01 -1.078e+01 -4.161e-01 -7.660e-01 -2.020e+00 -3.201e+00 -2.699e+00 -2.859e+00 -3.653e+00 -5.501e+00 1.900e-02 3.560e-01 1.007e+00 -1.270e+00 1.858e+00 9.302e-01 -1.502e+00 2.653e+00"
        ;;
        "u(𝛌＝3471Å)")
            initial_params="$refpsd_params -4.289e-01 -9.017e-01 -2.581e+00 -2.751e+00 -2.764e+00 -3.105e+00 -3.298e+00 -3.870e+00 -1.925e-01 -5.093e-01 -1.923e+00 -2.204e+00 -2.568e+00 -2.768e+00 -3.425e+00 -5.982e+00 1.646e-01 4.376e-01 7.202e-01 9.556e-01 4.856e-01 2.985e-01 -6.801e-01 -2.416e+00"
        ;;
        "UVM2(𝛌＝2246Å)")
            initial_params="$refpsd_params -1.003e-01 -5.235e-01 -1.863e+00 -2.159e+00 -2.718e+00 -3.504e+00 -1.139e+01 -4.119e+00 -2.431e-02 -3.121e-01 -1.526e+00 -1.877e+00 -2.455e+00 -2.945e+00 -3.356e+00 -3.602e+00 -1.862e-04 1.380e-01 2.475e-01 3.102e-02 6.040e-01 -1.488e-01 1.171e+00 1.617e+00"
        ;;
        "UVW1(𝛌＝2600Å)")
            initial_params="$refpsd_params -2.417e-01 -7.027e-01 -1.998e+00 -2.387e+00 -2.912e+00 -4.026e+00 -1.069e+01 -9.716e+00 -9.867e-02 -4.150e-01 -1.587e+00 -1.963e+00 -2.539e+00 -3.274e+00 -3.460e+00 -6.742e+00 6.734e-02 1.353e-01 3.585e-01 1.587e-01 2.959e-01 1.111e-01 2.364e-01 7.463e-01"
        ;;
        "UVW2(𝛌＝1928Å)")
            initial_params="$refpsd_params -1.648e-01 -4.974e-01 -1.686e+00 -1.897e+00 -2.600e+00 -2.801e+00 -3.814e+00 -9.888e+00 -6.101e-02 -3.020e-01 -1.433e+00 -1.756e+00 -2.485e+00 -2.799e+00 -3.317e+00 -7.000e+00 4.622e-02 1.473e-01 2.243e-01 1.606e-01 2.365e-01 1.484e-01 8.110e-01 9.689e-01"
        ;;
        "V(𝛌＝5404Å)")
            initial_params="$refpsd_params -1.044e+00 -1.457e+00 -2.848e+00 -2.957e+00 -4.232e+00 -4.052e+00 -4.515e+00 -5.250e+00 -5.026e-01 -7.878e-01 -2.118e+00 -2.276e+00 -3.421e+00 -3.378e+00 -4.224e+00 -4.930e+00 9.142e-02 4.814e-01 8.550e-01 3.929e-01 3.975e-01 5.311e-01 -1.203e-01 -2.905e+00"
        ;;
        "z(𝛌＝9157Å)")
            initial_params="$refpsd_params -1.852e+00 -2.156e+00 -3.798e+00 -4.146e+00 -4.577e+00 -4.338e+00 -4.672e+00 -5.230e+00 -9.339e-01 -1.131e+00 -2.536e+00 -3.031e+00 -4.060e+00 -3.801e+00 -4.139e+00 -5.151e+00 2.445e-01 8.131e-01 1.621e+00 1.460e+00 2.909e-01 6.991e-01 -5.831e-01 -2.921e-01"
        ;;
        *)
            echo -n  " Unknown band: using default parameters."
            initial_params="$refpsd_params -9.745e-01 -1.384e+00 -2.748e+00 -3.305e+00 -3.314e+00 -3.389e+00 -4.198e+00 -4.465e+00 -4.700e-01 -7.487e-01 -2.046e+00 -2.428e+00 -2.953e+00 -3.086e+00 -3.761e+00 -4.290e+00 9.862e-02 3.899e-01 8.650e-01 5.516e-01 2.228e-01 9.508e-01 -2.872e-01 9.059e-02"
        ;;
    esac

    # Write temporary input file
    echo "2" > tmp.psdlagargs
    echo "$ref_curve 0" >> tmp.psdlagargs
    echo "$echo_curve 0" >> tmp.psdlagargs
    echo "8 0.0049999999 0.018619375 0.044733049 0.069336227 0.10747115 0.16658029 0.25819945 0.40020915 0.62032418" >> tmp.psdlagargs
    echo "0" >> tmp.psdlagargs
    echo "$initial_params" >> tmp.psdlagargs
    echo "0:0 0" >> tmp.psdlagargs
    echo $error_type >> tmp.psdlagargs
    echo "0" >> tmp.psdlagargs
    echo -n "1000 50 50 mcmc_${echo_band}.dat" >> tmp.psdlagargs

    # Run psdlag with inputs
    if [[ -e $outputfile ]]
    then
        echo -n " Analysis already exists; skipping."
    else
        if [[ $1 == "thor" ]]
        then
            echo_band_noUTF=$(echo $echo_band|
                sed 's@𝛌@@g'|
                sed 's@(@_@g'|
                sed 's@)@_@g'|
                sed 's@＝@@g'|
                sed 's@Å@A@g')
            outputfile_noUTF=$(echo $outputfile|
                sed 's@𝛌@@g'|
                sed 's@(@_@g'|
                sed 's@)@_@g'|
                sed 's@＝@@g'|
                sed 's@Å@A@g'|
                sed 's@≻@_@g'|
                sed 's@σ∊@err@')
            argsfile="thor/arguments/$echo_band_noUTF.args"
            submitscript="thor/${echo_band_noUTF}.pbs"
            cp tmp.psdlagargs $argsfile
            cat scripts/templates/thor_submit.pbs|
                sed "s@%NAME%@psdlag_${echo_band_noUTF}@g"|
                sed "s@%ARGSFILE%@${argsfile}@g"|
                sed "s@%ROOTDIR%@$(pwd)@g"|
                sed "s@%OUTPUTFILE%@${outputfile_noUTF}@g" > $submitscript
            cd thor
            qsub $(basename $submitscript) >> submissions
            cd ..
        else    
            time bin/psdlag tmp.psdlagargs > $outputfile
        fi
    fi
    echo ""
done
