#!/usr/bin/env perl

use feature say;
use utf8; 
use PDL::FFT;

my $PI = 3.1415926539;
my $N = 64;
my ($series, $other_series);
for (my $k=0; $k<$N; $k++) {
  $series->[$k] = sin(4*$k*$PI/$N) + cos(6*$k*$PI/$N);
}
my $fft = new Math::FFT($series);
my $coeff = $fft->rdft();
my $spectrum = $fft->spctrm;
my $original_data = $fft->invrdft($coeff);

for (my $k=0; $k<$N; $k++) {
  $other_series->[$k] = sin(16*$k*$PI/$N) + cos(8*$k*$PI/$N);
}
my $other_fft = $fft->clone($other_series);
my $other_coeff = $other_fft->rdft();
my $correlation = $fft->correl($other_fft);