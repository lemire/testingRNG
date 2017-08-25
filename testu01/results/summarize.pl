#!/usr/bin/perl -w

# Summarize (XXX)Crush Test Results

use strict;

my %nonpassesFor;
my %testNameFor;
my %crushCountFor;

foreach my $file (@ARGV) {
    open my $fh, '<', $file;

    # Skip detailed log    
    while (<$fh>) {
	last if m/^========= Summary results/;
    }

    my $generator;
    while (<$fh>) {
	$generator = $1 if m/^ Generator:\s+(.*\S)/;
	next unless s/^ ([ \d]\d[ \d]) +//;
	die "No generator defined?" unless defined $generator;
	my $testNo = $1+0;
	s/\s+((?:1 - +|)\S+)\s*\z// or die "No p-value found!";
	my $pValue = $1;
	my $testName = $_;
	# print "$testNo: $testName --> $pValue\n";
	push @{$nonpassesFor{$generator}{$testNo}}, $pValue;
	$testNameFor{$testNo} = $testName;
    }
    if($generator) {
       ++$crushCountFor{$generator};
    } else {
      print "Warning: No generator in $file. \n";
    }
}

foreach my $generator (sort keys %nonpassesFor) {
    my $tooManyThreshold = 1 + $crushCountFor{$generator}/10;
    print "Summary for $generator ($crushCountFor{$generator} crushes):\n";
    my $nonpassesForTest = $nonpassesFor{$generator};
    my @blips;
    my $fails = 0;
    foreach my $test (sort { $::a <=> $::b } keys %$nonpassesForTest) {
	my $nonpassList = $nonpassesForTest->{$test};
	my @lowValues  = grep {
	    my $pValue = $_;
	    # Normalize the p-value to make bad values close to zero.
	    $pValue =~ s/1 - //;
	    $pValue =~ s/eps1?/0/;
	    $pValue = 1 - $pValue if $pValue > 0.5;
	    # L'Ecuyer uses 1e-10 and 1e-15 for clear fails in different 
	    # places, we'll use the stricter one.
	    $pValue < 1e-10;
	} @$nonpassList;
	my $isMany = @$nonpassList > $tooManyThreshold;
	if (@lowValues) {
	    print "- #$test: $testNameFor{$test}: FAIL!! -- p-values too unlikely (", join(", ", @lowValues), ")\n";
	    ++$fails;
	} elsif ($isMany) {
	    print "- #$test: $testNameFor{$test}: fail -- too many unusual p-values (", join(", ", @$nonpassList), ")\n";
	    ++$fails;
	} else {
	    push @blips, "#$test" foreach @$nonpassList;
	}
    }
    if (@blips) {
	print "- ", scalar(@blips), " unnoteworthy blips (", 
	      join(", ", @blips), ")\n";
    } elsif (!$fails) {
	print "- WARNING: no unnoteworthy blips (!!?!)\n";
    }
    print "\n";
}
