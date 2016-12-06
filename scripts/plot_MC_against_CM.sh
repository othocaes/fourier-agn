mkdir -p analyses/plots

for table in analyses/tables/timelag*MC}.tab
do
    #ref_band=$(basename $table|sed 's|\([^≺]*\)_≺_[^≺_]*_[^_]*_[^_]*\.tab|\1|')
    #echo_band=$(basename $table|sed 's|[^≺]*_≺_\([^≺_]*\)_[^_]*_[^_]*\.tab|\1|')
    CMtable=$(echo $table|sed 's|∊MC|∊CM|')

    ref_band=$(basename $table|sed 's|timelag_\([^≺]*\)[_ ]≺[_ ][^≺_ ]*[_ ]{[^_ ]*}.tab|\1|')
    echo_band=$(basename $table|sed 's|timelag_[^≺]*[_ ]≺[_ ]\([^≺_ ]*\)[_ ]{[^_ ]*}.tab|\1|')
    if [[ $ref_band == $echo_band ]]; then continue; fi
    err_str=$(basename $table|sed 's|timelag_[^≺]*[_ ]≺[_ ][^≺_ ]*[_ ]{[^_ ]*\(σ∊[CLM][MFC]\)}.tab|\1|')

    echo Plotting timelag error fit comparison for $echo_band.

    plotfile=analyses/plots/timelag_${echo_band}_error_compare.pdf

    echo_band_noUTF=$(echo $echo_band|
        sed 's|𝛌|λ|g'|
        #sed 's|(|_|g'|
        #sed 's|)|_|g'|
        sed 's|＝|=|g'|
        sed 's|Å|A|g')

    ref_band_noUTF=$(echo $ref_band|
        sed 's|𝛌|λ|g'|
        #sed 's|(|_|g'|
        #sed 's|)|_|g'|
        sed 's|＝|=|g'|
        sed 's|Å|A|g')

    cat scripts/templates/timelag_compare.gp|
        sed "s|%TITLE%|Error Fit Comparison for Time Lag of $echo_band|"|
        sed "s|%SUBTITLE%|referred to ${ref_band}, as reported by Fausnaugh et. al, STORM III, 2016|"|
        sed "s|%FILE1%|$table|"|
        sed "s|%LABEL1%|${echo_band} Delay with Monte Carlo Fit|"|
        sed "s|%FILE2%|$CMtable|"|
        sed "s|%LABEL2%|${echo_band} Delay with Covariance Matrix Fit|"|
        sed "s|%YMIN%|$ymin|"|sed "s|%YMAX|$ymax|"|
        sed "s|%OUTPUTFILE%|$plotfile|" > tmp.gp
    gnuplot tmp.gp

done


for table in analyses/tables/PSD*MC}.tab
do
    #ref_band=$(basename $table|sed 's|\([^≺]*\)_≺_[^≺_]*_[^_]*_[^_]*\.tab|\1|')
    #echo_band=$(basename $table|sed 's|[^≺]*_≺_\([^≺_]*\)_[^_]*_[^_]*\.tab|\1|')
    CMtable=$(echo $table|sed 's|∊MC|∊CM|')

    echo_band=$(basename $table|sed 's|PSD[_ ]\([0-9]\{4\}\)Å[_ ]{[^_ ]*}.tab|\1Å|')
    if [[ $ref_band == $echo_band ]]; then continue; fi
    err_str=$(basename $table|sed 's|PSD[ _]\([0-9]\{4\}\)Å[_ ]{[^_ ]*\(σ∊[CLM][MFC]\)}.tab|\1|')

    echo Plotting PSD error fit comparison for $echo_band.

    plotfile=analyses/plots/PSD_${echo_band}_error_compare.pdf

    echo_band_noUTF=$(echo $echo_band|
        sed 's|𝛌|λ|g'|
        #sed 's|(|_|g'|
        #sed 's|)|_|g'|
        sed 's|＝|=|g'|
        sed 's|Å|A|g')

    ref_band_noUTF=$(echo $ref_band|
        sed 's|𝛌|λ|g'|
        #sed 's|(|_|g'|
        #sed 's|)|_|g'|
        sed 's|＝|=|g'|
        sed 's|Å|A|g')

    cat scripts/templates/psd.gp|
        sed "s|%TITLE%|Error Fit Comparison for PSD of $echo_band|"|
        sed "s|%SUBTITLE%|as reported by Fausnaugh et. al, STORM III, 2016|"|
        sed "s|%FILE1%|$table|"|
        sed "s|%LABEL1%|${echo_band} PSD with Monte Carlo Fit|"|
        sed "s|%FILE2%|$CMtable|"|
        sed "s|%LABEL2%|${echo_band} PSD with Covariance Matrix Fit|"|
        sed "s|%YMIN%|$ymin|"|sed "s|%YMAX|$ymax|"|
        sed "s|%OUTPUTFILE%|$plotfile|" > tmp.gp
    gnuplot tmp.gp

done