#!/bin/bash
echo "static const char scene_${2}_source[] = \"\\"
sed 's/$/\\n\\/' "${1}/scene.glsl"
echo "\";"
