#!/usr/bin/env bash

# This metascript uses the available plotting scripts to produce a list of
# document-rady plots.

# Only analyses with this error type will be represented in the atlas
errtype="CM"
ref_band="1367Å"
echo_bands=$(ls analyses/*CM*|sed 's|[^≺]*≺_\(.\{5\}\).*|\1|')

# echo Using list of reverberated bands: $echo_bands

echo Propagating tables.
scripts/propagate_tables.sh > /dev/null

case $1 in
    "PSD")
        echo "Producing PSD atlas."
        gnuplot_file=psd_atlas.gp
        gnuplot_input=$(cat scripts/templates/${gnuplot_file}|perl -pe 's|\n|␤|g')
        for tabfile in analyses/tables/PSD_*${errtype}*;
        do
            echo_band=$(basename $tabfile|
                        sed 's|PSD[_ ]\(.\{5\}\)[_ ]{[^_ ]*}.tab|\1|')
            if [[ "$echo_band" == "$ref_band" ]] ; then continue; fi
            gnuplot_input_edit=$(echo "$gnuplot_input"|
                                    sed "s|%FILE%|$tabfile|"|
                                    sed "s|%LABEL%|$echo_band|")
            gnuplot_input="${gnuplot_input_edit}"
        done
        echo "$gnuplot_input"|perl -pe 's|␤|\n|g' > ${gnuplot_file}
        gnuplot $gnuplot_file

    ;;

    "lags")
        echo "Producing time delay atlas."
        gnuplot_file=timelag_atlas.gp
        gnuplot_input=$(cat scripts/templates/${gnuplot_file}|perl -pe 's|\n|␤|g')
        for tabfile in analyses/tables/timelag_*${errtype}*;
        do
            ref_band_extracted=$(basename $tabfile|sed 's|timelag_\([^≺]*\)[_ ]≺[_ ][^≺_ ]*[_ ]{[^_ ]*}.tab|\1|')
            echo_band=$(basename $tabfile|sed 's|timelag_[^≺]*[_ ]≺[_ ]\([^≺_ ]*\)[_ ]{[^_ ]*}.tab|\1|')
            if [[ "$echo_band" == "$ref_band" ]] ; then continue; fi
            gnuplot_input_edit=$(echo "$gnuplot_input"|
                                    sed "s|%FILE%|$tabfile|"|
                                    sed "s|%LABEL%|$echo_band|")
            gnuplot_input="${gnuplot_input_edit}"
        done
        echo "$gnuplot_input"|perl -pe 's|␤|\n|g' > ${gnuplot_file}
        gnuplot $gnuplot_file
    ;;
    default)
        echo "Did not understand plot type."
    ;; 
esac