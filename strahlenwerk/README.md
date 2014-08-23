# Strahlenwerk

## Building and Installing

### Install Juce
You have to `git clone --depth 1 git://github.com/julianstorer/JUCE.git` into any folder you want. Create then a symlink called `juce_modules` in `rtificial/strahlenwerk` to the modules folder of Juce.
Example:
    cd rtificial/strahlenwerk && \
        git clone --depth 1 git://github.com/julianstorer/JUCE.git ~/Coding/JUCE && \
        ln -s ~/Coding/JUCE/modules juce_modules

### Install external libraries
Go to `Lib` and execute `./build.sh`.

### Building
Go to `Build/Linux` and execute `make -j 5`.
