#!/bin/zsh

#script for copying test level file as used in testing video

for i in {1..100}; do cp "level/test.level" "level/test$i.level"; done
