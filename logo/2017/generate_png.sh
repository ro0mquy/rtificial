#!/bin/bash

if [[ -z "$1" ]]; then
	echo "SVG file needed."
	exit 1
fi

if [[ "$1" == "-h" ]]; then
	echo "generate_png.sh FILE [-w]"
	echo
	echo "Generate PNG files in resolutions:"
	echo "  16x16, 32x32, 64x64, 128x128,"
	echo "  256x256, 512x512, 1024x1024"
	echo "A transparent version, one with white"
	echo "background and an inverted one will"
	echo "generated."
	echo
	echo "FILE: The SVG file to generate PNGs from"
	echo "  -w: Variable width. The height will be"
	echo "        16, 32, 64, 128, 256, 512, 1024"
	echo "        and the aspect ration will be kept"
	exit 0
fi


SVG=$1
BASE=`basename "$SVG" .svg`

mkdir -p "png/$BASE"
BASE="png/$BASE/rt"

for i in $(seq 4 10); do
	py=$((2**$i))
	px=$py
	if [[ "$2" == "-w" ]]; then
		export_width=""
		export_area="drawing"
		h=$(inkscape --query-height $SVG)
		w=$(inkscape --query-width $SVG)
		px=$(printf "%.0f" "$(($py/$h*$w))")
	else
		export_width="--export-width=$py"
		export_area="page"
	fi

	inkscape --without-gui \
		--file=$SVG \
		--export-area-$export_area \
		--export-png="$BASE-transparent-${px}x${py}.png" \
		 $export_width \
		--export-height=$py
	echo
	inkscape --without-gui \
		--file=$SVG \
		--export-area-$export_area \
		--export-png="$BASE-light-${px}x${py}.png" \
		 $export_width \
		--export-height=$py \
		--export-background="#ffffff"
	echo
	echo "convert $BASE-dark-${px}x${py}.png"
	convert "$BASE-light-${px}x${py}.png" -negate "$BASE-dark-${px}x${py}.png"
	echo
done

if [[ "$2" != "-w" ]]; then
	STYLES=("transparent" "light" "dark")
	for style in $STYLES; do
		echo "convert $BASE-$style-16x16.gif"
		convert "$BASE-$style-16x16.png" "$BASE-$style-16x16.gif"
		echo "convert $BASE-$style-16x16.ico"
		convert "$BASE-$style-16x16.png" "$BASE-$style-16x16.ico"
	done
fi
