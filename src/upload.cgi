#!/usr/bin/perl -wT

use strict;
use CGI;
use CGI::Carp qw ( fatalsToBrowser );
use File::Basename;

$CGI::POST_MAX = 1024 * 5000;

my $safe_filename_characters = "a-zA-Z0-9_.-";
my $upload_dir = "/var/www/html/files";

my $query = new CGI; 
my $filename = $query->param("modulo"); 

if ( !$filename ) { 
	print $query->header ( );
	print "There was a problem uploading your photo (try a smaller file)."; 
	exit; 
}

my ( $name, $path, $extension ) = fileparse ( $filename, '..*' ); 
$filename = $name . $extension;

$filename =~ tr/ /_/; 
$filename =~ s/[^$safe_filename_characters]//g;

if ( $filename =~ /^([$safe_filename_characters]+)$/ ) { 
	$filename = $1; 
} 
else { 
	die "Filename contains invalid characters"; 
}

my $upload_filehandle = $query->upload("modulo");

open ( UPLOADFILE, ">$upload_dir/$filename" ) or die "$!"; 
binmode UPLOADFILE;

while ( <$upload_filehandle> ){
	print UPLOADFILE;
}

close UPLOADFILE;

print "Location: modules.cgi\n\n";