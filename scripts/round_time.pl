#!/usr/local/bin/perl
use feature say;
use utf8;
use feature qw/switch/;
open(my $file,$ARGV[0]) or die("could not open file");

$keep = 1;
# Add two because need to read more digits for rounding.
$keep += 2;
$throw = 5 - $keep;

<$file>;
while (<$file>) {
    $_ =~ /^.{8}\s+([0-9]{4})\s+([0-9]{5})\.([0-9]{$keep})[0-9]{$throw}(.*)/;
    $λ = $1;
    $t𝓃 = $2; # characteristic
    $t𝜀 = $3; # mantissa
    $vals = $4;
    if ($t𝜀 =~ /([0-9])([0-9])([0-9])/) {
        $t𝜀 = $1;
        given($2) {
            when (/[6-8]/) { $t𝜀 += 1; }
            when (/5/) { $t𝜀 += 1 if (!($2 == 0)); }
            }
        if ($t𝜀 > 9) {
            $t𝓃 += 1;
            $t𝜀 = 0;
        }
        # For rounding when even rules
        #say $2 % 2;
        #if (!($2%2 == 0) ) {
        #    $t𝜀 = $t𝜀 + 1;
        #}
    }
    else {
        die "Malformed data encountered";
    }
    say "$λ\t$t𝓃.$t𝜀$vals";
}
close($file);

