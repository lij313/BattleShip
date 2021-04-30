#!/bin/bash
for filename in ~/Desktop/3gc3-final-project/Code/Models/Texture/*
do
  echo $filename
  convert $filename -compress none ${filename::-4}.ppm
done;

for filename in ~/Desktop/3gc3-final-project/Code/Models/Texture/*.png
do
  echo $filename
  rm $filename
done;
