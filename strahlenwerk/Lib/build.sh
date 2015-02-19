#!/bin/bash
if [ -d efsw ]; then
	cd efsw
	hg pull
else
	hg clone https://bitbucket.org/SpartanJ/efsw
	cd efsw
fi
cmake -G 'Unix Makefiles' -DSTATIC_LIB=ON
make

# these should be already checked into git
#cd ..
#mkdir include
#ln -s ../efsw/include/efsw include/efsw
