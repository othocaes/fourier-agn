#!/usr/local/bin/perl
use feature say;
use utf8;

# reads (stdin) a heasoft data set (single filter) as compiled circa 2016 by Cackett @ WSU
# outputs (stdout) a lightcurve in psdlag circa 2016 Zoghbi @ UMY input format

# smallest time interval.
my $Δt = 0.1;
# normalization factor based on inspection of data
my $flux_norm = 1e14;

# This sets up the capture widths for rounding
# Add two because need to read more digits for rounding
$keep_digits = -1 * log10($Δt) + 2;
$throw_digits = 5 - $keep_digits;

# Skip first line -- shouldn't need this for a single filter
<>;

#Start file  Δt and with number of light curves (1)
my $linetoprint="# $Δt 1";

my $num_avg = 0;
while (<>) {
    $_ =~ /^.{8}\s+([0-9]{4})\s+([0-9]{5})\.([0-9]{$keep_digits})[0-9]{$throw_digits}(.*)/;
    # wavelength/label, characteristic, mantissa, everything else
    ($λ, $t𝓃, $t𝜀, $vals) = ($1,$2,$3,$4);

    # Round t according to Δt
    if (!(chop($t𝜀) == 0) + chop($t𝜀) > 5) {$t𝜀++;}
    if ($t𝜀 >= (1/$Δt)) {
        $t𝓃 += 1;
        $t𝜀 = 0;
    }
    if ($Δt >= 1) { $t𝜀 = 0; }

    # Print all values for this line
    # say "$λ\t$t𝓃.$t𝜀$vals";

    # Compile variables for this line
    ($flux_μ,$flux_σ,$mag_μ,$mag_σ,$telescope_id) = split ' ',$vals;
    $flux_μ *= $flux_norm;
    $flux_σ *= $flux_norm;

    # Print light curve for psdlag
    # This mess to average measurements occuring at the same time coordinate
    $this_t = "$t𝓃.$t𝜀";
    if ($this_t == $last_t) {
    	if ($heap_count == 1) {
    	        if ( $linetoprint =~ /([0-9\.]+)\s+([0-9e\-\.]+)\s+([0-9e\-\.]+)/ ) {
    	            our $new_flux_μ = $flux_μ + $2;
    	            # Could just take the max error
    	            # $new_flux_σ = max($flux_σ,$3);
    	            # Average error seems fair
    	            our $new_flux_σ = $flux_σ + $3;
    	            $num_avg++;
    	        }
    	        else { die "Malformed data"; }
    	}
    	else {
    		$new_flux_μ += $flux_μ;
    		$new_flux_σ += $flux_σ;
    	}
        $heap_count++;
    }
    else {
    	if ($heap_count > 1) {
	    	$new_flux_μ /= $heap_count;
	    	$new_flux_σ /= $heap_count;
	    	$linetoprint="$last_t  $new_flux_μ  $new_flux_σ";
	        our $heap_count = 1;
    	}
        say $linetoprint;
        $linetoprint = "$this_t  $flux_μ  $flux_σ";
    }
    $last_t = $this_t;
}
say $linetoprint;


sub max ($$) { $_[$_[0] < $_[1]] }
sub min ($$) { $_[$_[0] > $_[1]] }
sub log10 {
  my $n = shift;
  return log($n)/log(10);
}
