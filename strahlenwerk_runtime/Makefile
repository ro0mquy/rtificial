CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix build_linux/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -lGL -lX11
CXX_FLAGS := -std=c++11 -masm=intel
CXX := clang++

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
