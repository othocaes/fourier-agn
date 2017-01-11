#!/usr/bin/env perl

# This extracts the gnuplot-ready tables from a psdlag analysis and saves them
# to temporary files. propogate_tables.sh uses this script to create the
# tables it saves to analyses/tables.

use utf8;
use Encode qw(encode decode);
use feature 'say';
use locale;
use Switch;

use constant PI    => 4 * atan2(1, 1);

# Enables various levels of output.
our $verbose=1;
our $debug=0;


# This section locates the output data of interest in a
# psdlab output file.
if (!${^UTF8LOCALE}) {
    say encode($charset,"You are not using UTF-8 encoding. :(");
}
my $charset=$ENV{LANG};


# This program attempts to fill function_bin with the tabulated PSDs
# and time lags.
our %function_bin = ();

open $freq_file,'<',"freq.out" or die $!;
open $ref_psd_file,'<',"ref_psd.out" or die $!;
open $echo_psd_file,'<',"echo_psd.out" or die $!;
open $crsspctrm_file,'<',"crsspctrm.out" or die $!;
open $timelag_file,'<',"timelag.out" or die $!;

=pod

    This section collects the various quantities.

=cut

@bin_bounds = split /\s/,<$freq_file>;
@ref_psd = split /\s/,<$ref_psd_file>;
@ref_psd_σ = split /\s/,<$ref_psd_file>;
@echo_psd = split /\s/,<$echo_psd_file>;
@echo_psd_σ = split /\s/,<$echo_psd_file>;
@crsspctrm_psd = split /\s/,<$crsspctrm_file>;
@crsspctrm_psd_σ = split /\s/,<$crsspctrm_file>;
@timelag = split /\s/,<$timelag_file>;
@timelag_σ = split /\s/,<$timelag_file>;

if ($debug) {
    print encode($charset,"Found bin boundaries: ");
    foreach (@bin_bounds) {print encode($charset,"$_ ");}
    say encode($charset," ");
}
my $count = 0;
my $upper_bound = 0;
my $lower_bound = 0;
foreach (@bin_bounds) {
    $lower_bound = $upper_bound;
    $upper_bound = $_;
    if ($lower_bound == 0) {next}
    my $μ = ($upper_bound + $lower_bound)/2;
    my $Δ = ($upper_bound - $lower_bound)/2;
    #say ($μ,":",$Δ);
    # push(@freq_coords_mean,$μ);
    # push(@freq_coords_σ,$Δ);
    $function_bin{$μ} = {"Δ" => $Δ};
    $function_bin{$μ}{"ref_PSD_μ"} = $ref_psd[$count];
    $function_bin{$μ}{"ref_PSD_σ"} = $ref_psd_σ[$count];
    $function_bin{$μ}{"echo_PSD_μ"} = $echo_psd[$count];
    $function_bin{$μ}{"echo_PSD_σ"} = $echo_psd_σ[$count];
    $function_bin{$μ}{"crsspctrm_μ"} = $crosssp_psd[$count];
    $function_bin{$μ}{"crsspctrm_σ"} = $crosssp_psd_σ[$count];
    # $function_bin{$μ}{"φdiff_μ"} = $μ;
    # $function_bin{$μ}{"φdiff_σ"} = $σ;
    # $μ = $μ/(2*PI*$μ);
    # $σ = $σ/(2*PI*$μ);
    $function_bin{$μ}{"timelag_μ"} = $timelag[$count];
    $function_bin{$μ}{"timelag_σ"} = $timelag_σ[$count];
    $count = $count + 1;
}

close $freq_file;
close $ref_psd_file;
close $echo_psd_file;
close $crsspctrm_file;
close $timelag_file;

$numbins = keys %function_bin;
say encode($charset,"$numbins frequency bins captured in output.");

if($verbose) {
    say encode($charset,"freq μ        freq σ");
    while (each %function_bin ) {
        say encode($charset,
            sprintf("%f      %f",$_,$function_bin{$_}{"Δ"}));
    }
}


say encode($charset,"");

if($debug) {
    while (each %function_bin ) {
        print encode($charset,$_ . ": ");
        while ( my ($key,$value) = each %{$function_bin{$_}} ) {
            print encode($charset,$key . " => " . $value . ";  ");
        }
        say encode($charset,"");
    }
}

close($outputfile);

open($datafile,'>',"tmp.refPSD") or die $!;
while( each %function_bin) {
    say $datafile
        $_ . " " .
        $function_bin{$_}{"ref_PSD_μ"} . " " .
        $function_bin{$_}{"Δ"} . " " .
        $function_bin{$_}{"ref_PSD_σ"};
}
close($datafile);

open($datafile,'>',"tmp.echoPSD") or die $!;
while( each %function_bin) {
    say $datafile
        $_ . " " .
        $function_bin{$_}{"echo_PSD_μ"} . " " .
        $function_bin{$_}{"Δ"} . " " .
        $function_bin{$_}{"echo_PSD_σ"};
}
close($datafile);

open($datafile,'>',"tmp.crsspctrmPSD") or die $!;
while( each %function_bin) {
    say $datafile
        $_ . " " .
        $function_bin{$_}{"crsspctrm_PSD_μ"} . " " .
        $function_bin{$_}{"Δ"} . " " .
        $function_bin{$_}{"crsspctrm_PSD_σ"};
}
close($datafile);

open($datafile,'>',"tmp.timelag") or die $!;
while( each %function_bin) {
    say $datafile
        $_ . " " .
        $function_bin{$_}{"timelag_μ"} . " " .
        $function_bin{$_}{"Δ"} . " " .
        $function_bin{$_}{"timelag_σ"};
}
close($datafile);


#open($datafile,'>',"tmp.echoPSD") or die $!;
