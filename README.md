# Machine Learning Number Detection

A *slow and unoptimized* number detection neural network by Kento Nishi.

## About
This project uses code from [Simple Neural Network](https://github.com/KentoNishi/Simple-Neural-Network) and [Bitmap Numbers Dataset](https://github.com/KentoNishi/Bitmap-Numbers-Dataset).

## Details
All images in this repository are resized to ``16x16`` with 24 bit color.
The first 100 images of each digit are converted into the ``data.in`` format for the neural network. The pixel values are encoded into a floating point number the following fashion:
```
1-(((r+g+b)/3)/255)
```
This creates a value for each pixel that is always between 0 and 1.

## Files
The ``data`` subdirectory contains the sample dataset used for creating the dataset, as well as extra C++ scripts. These undocumented scripts are used for resizing images and converting files.

## Issues
* Configuration parameters were manually selected, so they may be highly inefficient.