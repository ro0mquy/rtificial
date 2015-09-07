BUILDDIR=PremakeBuilds/gmake

premake5 gmake
cd $BUILDDIR
make -f vinyl.make config=debug_4klang
make -f vinyl.make config=release_4klang
