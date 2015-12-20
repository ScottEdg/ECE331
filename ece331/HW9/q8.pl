#!/usr/bim/perl

use warnings;
use strict;
use bignum;
#prints integer value sent from STDIN 

print int "0x" . join("",split(":",<STDIN>)) + 0, "\n";

#"0x" tells perl that the following is to be treated as a hex number
#The + 0 tells perl to treat everthing before it as a number not a string
