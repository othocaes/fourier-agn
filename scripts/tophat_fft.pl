#!/usr/bin/env perl

use feature say;
use utf8;
use PDL;
use PDL::FFT;
use PDL::IO::Misc;

my $twoPI = 2*3.1415926539;

# space parameters
our $xres=.001;
our $xmin = 0;
our $xmax = 250;
our $num_bins = ($xmax-$xmin)/$xres;

# tophat parameters
my $μ1=15;
my $Δ1=5; # full width
my $μ2=15;
my $Δ2=5; # full width
my $μ3=15;
my $Δ3=5; # full width

say "Creating $num_bins bins";

# Complex Number z(x) = u(x) + iv(x)
my @th_list=tophat($μ1,$Δ1);
my $u = pdl(@th_list);
my $v = zeroes($u);


foreach (('1','2','3')) {

    # Output time-domain tophat
    open $tophattab, '>', "analyses/tables/tophat${$_}.tab";
    for ($i=0; $i < $num_bins; $i++) {
        my $x_coord = ($xmax-$xmin)*($i/$num_bins) + $xmin;
        say $tophattab "$x_coord $th_list[$i]";
    }
    close $tophattab;

    # Transform z(x) to Z(1/x) = U(1/x) + iV(1/x)
    my $U = $u;
    my $V = $v;
    fft($U,$V);
    # Determine frequency coordinates
    my $num_elements = nelem($U);
    say "Found $num_elements elements.";
    $f = $U->xlinvals(-($num_elements/2-1)/$num_elements/$xres,1/2/$xres)->rotate(-($num_elements/2 -1));

    my $φdiff = atan2($V,$U);
    my $timelag = $φdiff/($twoPI*$f);

    wcols $f,$timelag,"analyses/tables/tophat_fft${$_}.tab";

}

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