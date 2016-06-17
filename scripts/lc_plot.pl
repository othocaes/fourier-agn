#!/usr/local/bin/perl

use utf8;
use Encode qw(encode decode);
use feature 'say';
use locale;

if !${^UTF8LOCALE} {
    say "You are not using UTF-8 encoding. :("
}
my $charset=$ENV{LANG};
our $driving_curve_name=decode($charset,$ARGV[0]);
our @reprocessed_curves = [];
while (my $)
our $driving_curve_name=decode($charset,$ARGV[0]);
open $driving_curve_file,$driving_curve_name,< or die $!;



