#!/usr/bin/env perl


use feature 'say';
#use autodie qw(:all);


=pod

open my $GP, '|-', 'gnuplot';

print {$GP} <<'__GNUPLOT__';
    set terminal x11
    # set output "test.png"
    set xrange [-5:5];
    plot sin(x);
    pause -1;
__GNUPLOT__

close $GP;

=cut

say "Press any key to exit.";
my $a=<>;
system("gnuplot file");
#`gnuplot file`;
