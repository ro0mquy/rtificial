#!/bin/bash
chmod +x post-checkout
cp post-checkout ../../.git/hooks
cd ../../.git/hooks
ln -s post-checkout post-merge
