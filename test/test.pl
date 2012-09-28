#!/usr/bin/perl

# Test script for BugEye

use Modern::Perl '2013';

use Test::More;
use IPC::Run3;

my @args;
my $compiler   = 'g++';
my $err;
my $flags      = '-I./../';
my $normal_exe = 'Foo.out';
my $out;
my $src        = 'Foo.cpp';
my $test_exe   = 'Foo_TEST.out';

plan tests => 6;

# Normal

@args = ($compiler, $flags, $src, "-o$normal_exe");
run3(\@args, undef, \$out, \$err);
is($?, 0, 'Normal compile') or BAIL_OUT('Compilation failure');

@args = ("./$normal_exe");
run3(\@args, undef, \$out, \$err);
is($?, 0, 'Normal run exit status');

is($out,
   "This is the code that runs when we're not testing\n",
   'Normal run output');

# Test

@args = ($compiler, "-D_TEST", $flags, $src, "-o$test_exe");
run3(\@args, undef, \$out, \$err);
is($?, 0, 'Test compile') or BAIL_OUT('Compilation failure');

@args = ("./$test_exe");
run3(\@args, undef, \$out, \$err);
is($?, 0, 'Test run exit code');

is($out,
   "This is the test function for Foo\n",
   'Test run output');
