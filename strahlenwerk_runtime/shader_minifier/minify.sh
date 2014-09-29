#!/bin/bash

DIR=$1

mkdir minified

for f in $(find $DIR -name '*.glsl'); do
	mono shader_minifier.exe --format none -v $f && mv shader_code.h minified/$(basename $f)
done
