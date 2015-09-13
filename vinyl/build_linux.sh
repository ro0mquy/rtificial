BUILDDIR=Builds/gmake

if [[ "$1" = "clean" ]]; then
	echo "\n=== Cleaning ==="
	rm -rf $BUILDDIR/obj/
	rm -ff $BUILDDIR/bin/
fi

echo "\n=== Generating Makefile ==="
premake5 gmake
cd $BUILDDIR


CONFIGS=(
	"debug_4klang"
	"release_4klang"
	"debug_vorbis"
	"release_vorbis"
)

for CONFIG in $CONFIGS; do
	echo "\n=== Building config $CONFIG ==="
	CXXFLAGS="-Wno-unused-value" CXX=clang++ make -f vinyl.make config=$CONFIG
done
