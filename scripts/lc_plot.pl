#!/usr/local/bin/perl

use utf8;
use Encode qw(encode decode);
use feature 'say';
use locale;

if (!${^UTF8LOCALE}) {
    say "You are not using UTF-8 encoding. :("
}
my $charset=$ENV{LANG};
our $outputfilename=decode($charset,$ARGV[0]);
open $outputfile,'<',$outputfilename or die $!;

my $star_linenum_1=0;
my $star_linenum_2=0;

my $linenum=0;
while(<$outputfile>) {
    $linenum++;
    if ($_ =~ /^\*+$/) {
        $star_linenum_1=$star_linenum_2;
        $star_linenum_2 = $linenum; 
    }
}
say "Final set found between lines ",
    $star_linenum_1,
    " and ",
    $star_linenum_2;



@power_curve_source = [];
@power_curve_reprocessed = [];
@cross__correlation_power_curve = [];
@phase_difference = [];

