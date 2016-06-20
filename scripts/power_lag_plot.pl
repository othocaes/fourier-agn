#!/usr/local/bin/perl

use utf8;
use Encode qw(encode decode);
use feature 'say';
use locale;
use Switch;


# Enables debug output.
our $debug=0;


# This section locate the output data of interest in a
# psdlab output file.
if (!${^UTF8LOCALE}) {
    say encode($charset,"You are not using UTF-8 encoding. :(");
}
my $charset=$ENV{LANG};
our $outputfilename=decode($charset,$ARGV[0]);
open $outputfile,'<',$outputfilename or die $!;

my $star_linenum_1=0;
my $star_linenum_2=0;
my $star_bytenum_1=0;
my $star_bytenum_2=0;

my $linenum=0;
while(<$outputfile>) {
    $linenum++;
    if ($_ =~ /^\*+$/) {
        $star_linenum_1=$star_linenum_2;
        $star_linenum_2 = $linenum;
        $star_bytenum_1=$star_bytenum_2;
        #$star_bytenum_2 = $outputfile.tell();
        $star_bytenum_2 = tell($outputfile);
    }
}
if ($debug) {
    say encode($charset,"Final set found between lines "),
        $star_linenum_1,
        " and ",
        $star_linenum_2;

    say encode($charset,"Final set found between bytes "),
        $star_bytenum_1,
        " and ",
        $star_bytenum_2;
}
seek($outputfile,$star_bytenum_1,0);
<$outputfile>;
$bin_bounds_line = <$outputfile>;
#say $bin_bounds_line;
$bin_bounds_line =~ s/^#\s*(.*)$/$1/;
#$line =~ /
@bin_bounds = split /\s/,$bin_bounds_line;
if ($debug) {
    print encode($charset,"Found bin boundaries: ");
    foreach (@bin_bounds) {print encode($charset,"$_ ");}
    say encode($charset," ");
}


# These arrays store the various quantities of interest.
@freq_coords_mean = [];
@freq_coords_err = [];
@arr_power_curve_source_mean = [];
@arr_power_curve_source_err = [];
@arr_power_curve_reprocessed_mean = [];
@arr_power_curve_reprocessed_err = [];
@arr_cross_correlation_power_curve_mean = [];
@arr_cross_correlation_power_curve_err = [];
@arr_phase_difference_mean = [];
@arr_phase_difference_err = [];
@arr_time_lag_mean = [];
@arr_time_lag_err = [];

# This captures the frequency bins.
my $upper_bound = 0;
my $lower_bound = 0;
foreach (@bin_bounds) {
    $lower_bound = $upper_bound;
    $upper_bound = $_;
    if ($lower_bound == 0) {next}
    my $mean = ($upper_bound + $lower_bound)/2;
    my $err = ($upper_bound - $lower_bound)/2;
    #say ($mean,":",$err);
    push(@freq_coords_mean,$mean);
    push(@freq_coords_err,$err);
}

$numbins = scalar @freq_coords_mean;
say encode($charset,"$numbins frequency boundaries captured.");


# This section collects the various quantities. The mode counter
# increments to designate the data being captured.
my $mode=0;
while (<$outputfile>) {
    next if $_ =~ /\*+/;
    switch ($mode) {
        case 0 {
            our $power_curve_source_mean = my $power_curve_source_err = $_;
            $power_curve_source_mean =~ s/^([\-\+e0-9\.]+)+\s+[\-\+e0-9\.]+\s*$/$1/;
            $power_curve_source_err =~ s/^[\-\+e0-9\.]+\s+([\-\+e0-9\.]+)\s*$/$1/;
            if ($debug) {
                say encode($charset,"");
                say encode($charset,"                    New Bin");
                say encode($charset,"─────────────────────────────────────────────────");
                print encode($charset,"Driving light curve power from $_");
                say encode($charset,"Average: $power_curve_source_mean, Err: $power_curve_source_err");
            }
            push(@arr_power_curve_source_mean,$power_curve_source_mean);
            push(@arr_power_curve_source_err,$power_curve_source_err);
            $mode++;
        }
        case 1 {
            my $power_curve_reprocessed_mean = my $power_curve_reprocessed_err = $_;
            $power_curve_reprocessed_mean =~ s/^([\-\+e0-9\.]+)+\s+[\-\+e0-9\.]+\s*$/$1/;
            $power_curve_reprocessed_err =~ s/^[\-\+e0-9\.]+\s+([\-\+e0-9\.]+)\s*$/$1/;
            if ($debug) {
                print encode($charset,"Reprocessed light curve power from $_");
                say encode($charset,"Average: $power_curve_reprocessed_mean, Err: $power_curve_reprocessed_err");
            }
            push(@arr_power_curve_reprocessed_mean,$power_curve_reprocessed_mean);
            push(@arr_power_curve_reprocessed_err,$power_curve_reprocessed_err);
            $mode++;
        }
        case 2 {
            my $cross_correlation_power_curve_mean = my $cross_correlation_power_curve_err = $_;
            $cross_correlation_power_curve_mean =~ s/^([\-\+e0-9\.]+)+\s+[\-\+e0-9\.]+\s*$/$1/;
            $cross_correlation_power_curve_err =~ s/^[\-\+e0-9\.]+\s+([\-\+e0-9\.]+)\s*$/$1/;
            if ($debug) {
                print encode($charset,"Cross-Correlation from $_");
                say encode($charset,"Average: $cross_correlation_power_curve_mean, Err: $cross_correlation_power_curve_err");
            }
            push(@arr_cross_correlation_power_curve_mean,$cross_correlation_power_curve_mean);
            push(@arr_cross_correlation_power_curve_err,$cross_correlation_power_curve_err);
            $mode++;
        }
        case 3 {
            my $phase_difference_mean = my $phase_difference_err = $_;
            $phase_difference_mean =~ s/^([\-\+e0-9\.]+)\s+[\-\+e0-9\.]+\s*$/$1/;
            $phase_difference_err =~ s/^[\-\+e0-9\.]+\s+([\-\+e0-9\.]+)\s*$/$1/;
            if ($debug) {
                print encode($charset,"Phase different from $_");
                say encode($charset,"Average: $phase_difference_mean, Err: $phase_difference_err");
            }
            push(@arr_phase_difference_mean,$phase_difference_mean);
            push(@arr_phase_difference_err,$phase_difference_err);
            my %freqrecord = ( "arr_power_curve_source_mean" => arr_power_curve_source_mean);
            $mode = 0;
        }
    } # End switch
}

$numvals = scalar @arr_power_curve_source_mean;
say encode($charset,"Retrieved $numvals sets of quantities.");

# This section builds the records
foreach(@freq_coords_mean) {

}


open($datafile,'>',"tmp.sourcePSD") or die $!;
open($datafile,'>',"tmp.reprocPSD") or die $!;

# foreach(@freq_coords_mean)













=pod
    Tried to set up the program to find the boundaries on its own.
    Should return to complete the program this way; just a syntax
    problem.

#while (<$outputfile>) {say $_;}
#while (<$outputfile> =~ /^[^#]+(.*)$/) {}
#if (eof($outputfile)) {
#    say encode($charset,"Could not scrape values of energy bin boundaries.");
#    exit 9;
#}
=cut






#$thing = <$outputfile>;
#say encode($charset,"$thing");







# switch ($val) {
#             case 1      { print encode($charset,"number 1" )}
#             case "a"    { print encode($charset,"string a" )}
#             case [1..10,42] { print encode($charset,"number in list" )}
#             case (@array)   { print encode($charset,"number in list" )}
#             case /\w+/  { print encode($charset,"pattern" )}
#             case qr/\w+/    { print encode($charset,"pattern" )}
#             case (%hash)    { print encode($charset,"entry in hash" )}
#             case (\%hash)   { print encode($charset,"entry in hash" )}
#             case (\&sub)    { print encode($charset,"arg to subroutine" )}
#             else        { print encode($charset,"previous case not true" )}
#         }