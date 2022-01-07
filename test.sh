#!/bin/bash
assert() {
  expected="$1"
  input="$2"
  
  ./9cc "$input" > tmp.s
  docker run --rm -v /Users/katouryoushi/project/02cs_basic/compilerbook/9cc:/9cc -w /9cc compilerbook cc -o tmp tmp.s
  docker run --rm -v /Users/katouryoushi/project/02cs_basic/compilerbook/9cc:/9cc -w /9cc compilerbook ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34 - 5 "

echo OK
