#!/bin/sh -l

make build

if [ $? -eq 0 ]
then
  echo "Building Successful."
else
  echo "Build Failed"
  exit 1
fi

make test

if [ $? -eq 0 ]
then
  echo "Success: All tests passed."
else
  echo "Failed: Some tests failed."
  exit 1
fi