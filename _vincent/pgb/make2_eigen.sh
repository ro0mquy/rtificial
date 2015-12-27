#!/bin/env sh
g++ -o pgb2_eigen pgb2_eigen.cpp -std=c++11 -O3 -Wall -Wextra -lsndfile $(pkg-config --cflags eigen3) -g
