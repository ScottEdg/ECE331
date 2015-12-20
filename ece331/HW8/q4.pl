#!/usr/bin/perl
use warnings;
use strict;

my $argc = @ARGV; #get number of arguments
if($argc !=1 || $ARGV[0] < 0 || $ARGV[0] > ((2 ** 32) -1)){
	die "error: Please enter only 1 valid unsigned integer value.\n";
}

printf("%b\n", $ARGV[0])
