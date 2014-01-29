#!/usr/bin/sh
mkdir -p scenes/$1
cp scenes/template/scene.glsl scenes/$1/
cp scenes/template/config.json scenes/$1/
cp scenes/template/timeline.json scenes/$1/
