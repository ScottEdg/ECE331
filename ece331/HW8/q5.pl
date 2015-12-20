#!/usr/bin/perl
use warnings;
use strict;

my $argc = @ARGV;

if($ARGV[0] =~ m/((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)/ &&
   $ARGV[1] =~ m/((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)/ &&
   $argc == 2){
	
	#taking out the "." to in order to manipulate the IP address.
	my $ip_addr = $ARGV[0];
	my @ip_addrb = split("[.]",$ip_addr);
	my $ip_addr_val = unpack("N", pack("C4",@ip_addrb));

	#like wise for the netmask.
	my $netmask = $ARGV[1];
	my @netmaskb = split("[.]", $netmask);
	my $netmask_val = unpack("N", pack("C4",@netmaskb));

	#now to do the masking.
	my $net_addr_val = $ip_addr_val & $netmask_val;
	my $brdcst_addr_val = $ip_addr_val | ~ $netmask_val;

	#we have the numerical values, now to put back the "."'s in the right place
	my @net_addrb = unpack("C4", pack("N", $net_addr_val));
	my $net_addr = join(".", @net_addrb);

	#likewise for the Broadcast Address.
	my @brdcst_addrb = unpack("C4", pack("N", $brdcst_addr_val));
	my $brdcst_addr = join(".", @brdcst_addrb);
	
	#print for the user to see results.
	print "$net_addr\n";
	print "$brdcst_addr\n";
}
else{
	die "error: Please send only 2 valid IP addresses.\n";
}
