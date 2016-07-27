for table in analyses/tables/*_σ∊MC_timelag*
do
    ref_band=$(basename $table|sed 's|\([^≺]*\)_≺_[^≺_]*_[^_]*_[^_]*\.tab|\1|')
    echo_band=$(basename $table|sed 's|[^≺]*_≺_\([^≺_]*\)_[^_]*_[^_]*\.tab|\1|')
    if [[ $ref_band == $echo_band ]]; then continue; fi
    err_str=$(basename $table|sed 's|[^≺]*_≺_[^≺_]*_\(σ∊[CLM][MFC]\)\_[^_]*.tab|\1|')

    CMtable=$(echo $table|sed 's|∊MC|∊CM|')

    plotfile=analyses/plots/${echo_band}_error_compare.png

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
        sed "s|%TITLE|Error Fit Comparison for Time Lag of $echo_band_noUTF|"|
        sed "s|%SUBTITLE|referred to ${ref_band_noUTF}, as reported by Fausnaugh et. al, STORM III, 2016|"|
        sed "s|%FILE1|$table|"|
        sed "s|%LABEL1|${echo_band_noUTF} with Monte Carlo Fit|"|
        sed "s|%FILE2|$CMtable|"|
        sed "s|%LABEL2|${echo_band_noUTF} with Covariance Matrix Fit|"|
        sed "s|%YMIN|$ymin|"|sed "s|%YMAX|$ymax|"|
        sed "s|%OUTPUTFILE|$plotfile|" > tmp.gp
    gnuplot tmp.gp

done