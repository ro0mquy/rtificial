#!/bin/bash

function verheader {
	local TARGET="minified/""$1"".h"
	echo "#ifndef ""$1""_H" > $TARGET
	echo "#define ""$1""_H" >> $TARGET
	echo -n "const char ""$1""_source[] = R\"shader_source(" >> $TARGET
	cat shader_code.h >> $TARGET
	echo ")shader_source\";" >> $TARGET
	echo "#endif" >> $TARGET
	rm shader_code.h
}

DIR=$1

mkdir -p minified

for f in $(find $DIR -name '*.glsl'); do
	#mono shader_minifier.exe --format none -v $f && verheader $(basename -s .glsl $f)
	cp $f shader_code.h && verheader $(basename -s .glsl $f)
done
