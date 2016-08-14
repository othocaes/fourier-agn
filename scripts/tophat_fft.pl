#!/usr/bin/env perl

use feature say;
use utf8;
use PDL;
use PDL::FFT;
use PDL::IO::Misc;

my $_2π = 2*3.1415926539;

# space parameters
our $xres= 0.1;
our $xmin = 0;
our $xmax = 2500;
our $xbins = ($xmax-$xmin)/$xres + 1;

say "Using $xbins bins";

# tophat parameters
my $μ1=7;
my $Δ1=5; # full width

my @tophat_list = ([$μ1,$Δ1],[28,8],[15,11]);

our $tophat_count = 0;
foreach my $pars (@tophat_list) {
    $tophat_count++;

    # Capture tophat into piddle
    # Complex Number z(x) = u(x) + iv(x)
    my @tophat=tophat(@$pars[0],@$pars[1]);
    my $u = pdl(@tophat);
    my $v = zeroes($u);

    $x_coords = $u->xlinvals($xmin,$xmax);

    # Output x-domain tophat
    wcols $x_coords,$u,"analyses/tables/tophat${tophat_count}.tab";

    # Transform z(x) to Z(x⁻¹) = U(x⁻¹) + iV(x⁻¹)
    my $U = $u;
    my $V = $v;
    fft($U,$V);

    # Determine frequency coordinates
    my $num_elements = nelem($U);
    say "Found $num_elements elements.";

    if ($num_elements % 2 == 0) {
        # even number of bins
        $f = $U->xlinvals(
                -(${num_elements}/2-1)/${num_elements}/${xres}, 1/2/${xres}
                )->rotate(-(${num_elements}/2 -1));
    }
    else {
        #odd number of bins
        $f = $U->xlinvals(
                    -(${num_elements}/2-0.5)/${num_elements}/${xres},
                        (${num_elements}/2-0.5)/${num_elements}/${xres}
                    )->rotate(-(${num_elements}-1)/2);
    }


    # Calculate x offset
    my $φdiff = atan2($V,$U);
    my $offset = $φdiff/($_2π*$f);

    wcols $f,$offset,"analyses/tables/tophat_fft${tophat_count}.tab";

}

sub tophat {
    (my $mean,my $width) = @_;
    my $halfwidth = $width/2;
    my @vals = ();
    for (my $x_coord = $xmin; $x_coord <= $xmax; $x_coord += $xres) {
        push @vals, ($x_coord >= ($mean - $halfwidth) &&
                    $x_coord <= ($mean + $halfwidth)) ? 1/$width : 0;
    }
    return @vals;
}