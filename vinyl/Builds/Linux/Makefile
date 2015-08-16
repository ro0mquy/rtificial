CPP_FILES := $(wildcard ../../Source/**/*.cpp)
OBJ_FILES := $(notdir $(CPP_FILES:.cpp=.o)) ../../Source/music/4klang.linux.o
LD_FLAGS := -m32 -lGL -lX11 -lasound -pthread
CXX_FLAGS := -std=c++11 -m32 -Wall -Wextra
CXX := clang++

DEPS := $(OBJ_FILES:.o=.d)
CXX_FLAGS += -MMD

all: vinyl
debug: vinyl

# define _DEBUG when compiling with debug-support
# for compatibility with visual studio
debug: CXX_FLAGS += -g -D_DEBUG

vinyl: $(OBJ_FILES)
	$(CXX) $(LD_FLAGS) -o build/$@ $^

build/%.o: ../../Source/%.cpp
	$(CXX) $(CXX_FLAGS) -c -o build/$@ $<

.PHONY: clean
clean:
	rm -f build/* vinyl

-include $(DEPS)
