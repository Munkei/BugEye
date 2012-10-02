#!/usr/bin/perl

#          Copyright Theo Willows 2012.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

# Test script for BugEye

use Modern::Perl '2013';

use IPC::Run3;
use TAP::Parser qw/all/;
use Test::More;

my @args;
my $compiler   = 'g++';
my $err;
my $include    = '-I./../';
my $normal_exe = 'Foo.out';
my $out;
my $src        = 'Foo.cpp';
my $test_exe   = 'Foo_TEST.out';
my $wall       = '-Wall';

plan tests => 11;

# Normal

@args = ( $compiler, $wall, $include, $src, "-o$normal_exe" );
run3( \@args, undef, \$out, \$err );
unless ( is( $?, 0, 'Normal compile' ) ) {
  diag($err);
  BAIL_OUT('Compilation failure');
}

@args = ("./$normal_exe");
run3( \@args, undef, \$out, \$err );
is( $?, 0, 'Normal run exit status' );

is(
   $out,
   "This is the code that runs when we're not testing\n",
   'Normal run output'
  );

# Test

my $total       = 4;            # not todo:s
my $failed      = 2;            # not todo:s
my $passed      = 6;            # ok:s + all todo:s
my $todo        = 4;            # all todo:s
my $todo_passed = 2;            # all todo-and-ok:s

@args = ( $compiler, "-D_TEST", $wall, $include, $src, "-o$test_exe" );
run3( \@args, undef, \$out, \$err );
unless ( is( $?, 0, 'Test compile' ) ) {
  diag($err);
  BAIL_OUT('Compilation failure');
}

@args = ("./$test_exe");
run3( \@args, undef, \$out, \$err );
is( $?>>8, $failed, 'Test run exit code' );

my $parser = TAP::Parser->new( { tap => $out } );
$parser->run;

my $errors = $parser->parse_errors;
is( $errors, 0, "Parser errors" ) or diag( $parser->parse_errors );

ok( $parser->is_good_plan, "Good plan" );

is( $parser->passed,      $passed,      "Number of passed tests" );
is( $parser->failed,      $failed,      "Number of failed tests" );
is( $parser->todo,        $todo,        "Number of TODO tests" );
is( $parser->todo_passed, $todo_passed, "Number of passed TODO tests" );
