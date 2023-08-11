#!/usr/bin/perl -w 

use strict;
use Digest::MD5 qw(md5 md5_base64);

{

   my $LEETSALT = "9a577f69a783780e";
   my $SALTEND = "d3d7505a82e8b493";


	my $target = "ac697cd0e0981917d04211e1b6cb077d";
	while(<>){
		chomp();
		s/\s//g;
		my $bar = $LEETSALT.$_.$SALTEND;

#		my $md5sum = md5($bar);
#		print "DEBUG: $_: $md5sum\n";

		if(md5_base64(md5_base64($bar)) eq $target){
			print "DEBUG: FOUND!\n";
			print "$_\n";
			exit();
		}
	}

}
