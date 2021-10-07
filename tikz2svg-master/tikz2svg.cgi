#!/usr/bin/env perl
################################################################################
# tikz2svg.cgi:	TikZ to SVG converter server-side CGI
# Requires:	pdflatex with standalone and tikz packages, pdf2svg,
#		Digest::HMAC_SHA1
# Author:	Huidae Cho
# Since:	January 12, 2019
#
# Copyright (C) 2019, Huidae Cho <https://idea.isnew.info>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
################################################################################
use strict;
eval "use Digest::HMAC_SHA1 qw(hmac_sha1);1" or die;
my %cfg = do ".tikz2svgrc"; # Change this path to yours and secure this file
my $debug_started = 0;
my $QUERY_STRING = $ENV{QUERY_STRING};
my $REQUEST_METHOD = $ENV{REQUEST_METHOD};
my $CONTENT_TYPE = $ENV{CONTENT_TYPE};
my $CONTENT_LENGTH = $ENV{CONTENT_LENGTH};

sub tikz2svg{
	my ($tex, $jobname) = @_;

	my @lines = split /^/, $tex;
	my $first;
	foreach($_ = shift @lines){
		next if /^[ \t]*(?:%.*)?$/;
		$first = $_;
		last;
	}
	$first =~ s/^[ \t]+|[\r\n]+//g;
	die_text("First word not a command") if $first !~ /^\\/;

	my $type;
	($_ = $first) =~ s/%.*//;
	if(/^\\documentclass.*\{standalone\}/){
		$type = 1;
	}elsif(/^\\documentclass/ && !/\{standalone\}/){
		$type = 2;
	}elsif(/^\\begin\{document\}/){
		$type = 3;
	}elsif(/^\\begin\{tikzpicture\}/ || /^\\tikz(?:$|[^a-z])/){
		$type = 4;
	}else{
		$type = 5;
	}

	my ($begin, $end);
	$begin = $first if $type == 1;
	$begin = "\\documentclass[tikz]{standalone}\n" if $type >= 2;
	$begin .= $first if $type == 3;
	if($type >= 4){
		$begin .= "\\begin{document}\n";
		$end = "\\end{document}\n";
	}
	$begin .= $first if $type == 4;
	if($type >= 5){
		$begin .= "\\begin{tikzpicture}\n";
		$end = "\\end{tikzpicture}\n".$end;
	}
	$begin .= $first if $type == 5;

	return unless chdir "/tmp";

	$tex = "$begin@lines$end";

	local $" = "";
	open PDFLATEX, "| $cfg{pdflatex} -jobname=$jobname -- > $jobname.tikz2svg.log";
	print PDFLATEX $tex;
	close PDFLATEX;

	return unless -f "$jobname.pdf";

	print `$cfg{pdf2svg} $jobname.pdf /dev/stdout`;
	unlink glob "$jobname.*";
}

sub debug{
	my $msg = shift;
	unless($debug_started){
		$debug_started = 1;
		print "Content-Type: text/plain\n\n";
	}
	printf "%s\n", $msg;
}

sub die_text{
	print "Content-Type: text/plain\n\n";
	print shift;
	die;
}

sub decode_url{
	my $url = shift;
	$url =~ s/%(..)/@{[pack "c", hex($1)]}/g;
	return $url;
}

sub get_var{
	my (%var, $v);
	foreach(split /&/, $QUERY_STRING){
		/^([^=]*)=(.*)$/;
		$v = $1; $var{$v} = $2;
		$var{$v} =~ y/+/ /; $var{$v} =~ s/%0D//g;
		$var{$v} = decode_url($var{$v});
	}
	return %var if $CONTENT_LENGTH eq "" || $CONTENT_LENGTH <= 0;

	my $content = "";
	my $total_len = 0;
	while(my $buf_len = read STDIN, my $buf, $CONTENT_LENGTH - $total_len){
		$total_len += $buf_len;
		$content .= $buf;
		die_text("Internal errors") if $total_len > $CONTENT_LENGTH;
		last if $total_len == $CONTENT_LENGTH;
	}
	if($CONTENT_TYPE eq "application/x-www-form-urlencoded"){
		foreach(split /&/, $content){
			/^([^=]*)=(.*)$/;
			$v = $1; $var{$v} = $2;
			$var{$v} =~ y/+/ /; $var{$v} =~ s/%0D//g;
			$var{$v} = decode_url($var{$v});
		}
	}elsif($CONTENT_TYPE =~ m#^multipart/form-data; boundary=(.*)$#){
		my $boundary = $1;
		$content = substr $content, length($boundary) + 4,
			$CONTENT_LENGTH - 2 * (length($boundary) + 4) - 4;
		foreach(split /\r\n--$boundary\r\n/, $content){
			my ($header, $body) = /^(.*?)\r\n\r\n(.*)$/s;
			$header =~ / name="(.*?)"/;
			my $name = $1;
			if($header =~ / filename="(.*?)"/){
				($var{$name} = $1) =~ s#^.*[/\\]##;
				$var{"$name="} = $body;
			}else{
				$body =~ s/\r//g;
				$var{$name} = $body;
			}
		}
	}
	return %var;
}

sub generate_random_string{
	my $len = shift;
	# http://www.perlmonks.org/?node_id=233023
	my @chars = ("a".."z", "A".."Z", "0".."9");
	my $str;
	$str .= $chars[rand @chars] for 1..$len;
	return $str;
}

sub generate_salt{
	# salt length: 8
	return generate_random_string(8);
}

# http://www.perlmonks.org/?node_id=631963
# Thanks to Jochen Hoenicke <hoenicke@gmail.com>
# (one of the authors of Palm Keyring)
sub pbkdf2{
	my ($prf, $password, $salt, $iter, $keylen) = @_;
	my ($k, $t, $u, $ui, $i);
	$t = "";
	for($k = 1; length($t) < $keylen; $k++){
		$u = $ui = $prf->($salt.pack('N', $k), $password);
		for($i = 1; $i < $iter; $i++){
			$ui = $prf->($ui, $password);
			$u ^= $ui;
		}
		$t .= $u;
	}
	return substr $t, 0, $keylen;
}

# PBKDF2 for password hashing
# http://www.ict.griffith.edu.au/anthony/software/pbkdf2.pl
# Anthony Thyssen
sub get_pbkdf2_key{
	# key length: 128
	my ($password, $salt) = @_;
	my $prf = \&hmac_sha1;
	my $iter = 8192;
	my $keylen = 64;
	return unpack("H*", pbkdf2($prf, $password, $salt, $iter, $keylen));
}

sub hash_password{
	# hashed password length: 2*8+128=144
	my ($pw, $salt) = @_;
	my $salt = generate_salt() unless defined $salt;
	return unpack("H*", $salt).get_pbkdf2_key($pw, $salt);
}

if(1){
if(-t STDIN){
	printf "%s\n", hash_password($cfg{secret})
		if $#ARGV == 0 && $ARGV[0] eq "key";
	exit 0;
}

die if $REQUEST_METHOD ne "POST";
}

my %var = get_var();
my $salt = int(time/300);
die if $var{key} ne hash_password($cfg{secret}, $salt);

print "Content-Type: image/svg+xml\n\n";
tikz2svg($var{tex}, $var{jobname});
