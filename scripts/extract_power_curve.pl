#!/usr/local/bin/perl

use Graphics::GnuplotIF;
my $plot = Graphics::GnuplotIF->new (
    style        => 'histogram', # one of the gnuplot line     styles (see below)
    title        => "$mytitle",      # string
    xlabel       => '$myxlabel',     # string
    ylabel       => '$myylabel',     # string
    xrange       => [@myxrange],      # array reference; autoscaling, if empty
    yrange       => [@myyrange],      # array reference; autoscaling, if empty
    plot_titles  => [ "$titles" ],      # array of strings; titles used in the legend
    persist      => 0,       # let plot windows survive after gnuplot exits
                             # 0 : close / 1 : survive
    );

    $plot_glacier->gnuplot_plot_xy ( \@x, \@y );
    $plot_glacier->gnuplot_pause();
