#!/usr/bin/env perl

use feature say;
use utf8; 
use PDL;
use PDL::FFT;
use PDL::IO::Misc;

my $twoPI = 2*3.1415926539;

# space parameters
our $Δt=.001;
our $xmin = 0;
our $xmax = 250;
our $num_bins = ($xmax-$xmin)/$Δt;

say "Creating $num_bins bins";

# Complex Number z(x) = u(x) + iv(x)
my @th_list=tophat(10,7);
my $u = pdl(@th_list);
my $v = zeroes($u);

# Output time-domain tophat
open $tophattab, '>', 'analyses/tables/tophat.tab';
for ($i=0; $i < $num_bins; $i++) {
    my $x_coord = ($xmax-$xmin)*($i/$num_bins) + $xmin;
    say $tophattab "$x_coord $th_list[$i]";
}
close $tophattab;

# Transform z(x) to z(1/x) = u(1/x) + iv(1/x)
fft($u,$v);
# Determine frequency coordinates
my $num_elements = nelem($u);
say "Found $num_elements elements.";
$f = $u->xlinvals(-($num_elements/2-1)/$num_elements/$Δt,1/2/$Δt)->rotate(-($num_elements/2 -1));

my $φdiff = atan2($v,$u);
my $timelag = $φdiff/($twoPI*$f);

wcols $f,$timelag,'analyses/tables/tophat_fft.tab';


sub tophat {
    (my $mean,my $width) = @_;
    my $halfwidth = $width/2;
    my @vals = ();
    for ($i=0; $i < $num_bins; $i++) {
        my $x_coord = ($xmax-$xmin)*($i/$num_bins) + $xmin;
        if ($x_coord >= ($mean - $halfwidth ) && $x_coord <= ($mean + $halfwidth)) { push @vals, 1/$width; }
        else { push @vals, 0; }
    }
    return @vals;
}