CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix build/,$(notdir $(CPP_FILES:.cpp=.o)))

LD_FLAGS := -lGL $(shell sdl2-config --libs) -Wall -Wextra
CXX_FLAGS := -std=c++11 $(shell sdl2-config --cflags) -Wall -Wextra

# add header dependency generation
DEPS := $(OBJ_FILES:.o=.d)
CXX_FLAGS += -MMD

.PHONY: all
all: $(OBJ_FILES)
	$(CXX) $(LD_FLAGS) -o build/ventilate $^

build/%.o: src/%.cpp src/glcorearb.h
	$(CXX)  $(CXX_FLAGS) -c -o $@ $<

src/glcorearb.h:
	curl https://www.opengl.org/registry/api/GL/glcorearb.h > src/glcorearb.h

.PHONY: clean
clean:
	rm -f build/*

-include $(DEPS)
