CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix build_linux/,$(notdir $(CPP_FILES:.cpp=.o))) src/music/4klang.linux.o
LD_FLAGS := -m32 -lGL -lX11 -lasound -pthread -Wl,--verbose
CXX_FLAGS := -std=c++11 -m32
#CXX := clang++

# Linux Build
CXX_FLAGS += -DBUILD_LINUX

DEPS := $(OBJ_FILES:.o=.d)
CXX_FLAGS += -MMD

demo: $(OBJ_FILES)
	$(CXX) $(LD_FLAGS) -o $@ $^

build_linux/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f build_linux/* demo

-include $(DEPS)
