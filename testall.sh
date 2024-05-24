#!/bin/sh

for i in testfiles/*.pas; do
	echo "testing $i"
	./parser <  $i
done
