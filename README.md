# Steganography - Text Encoder/Decoder
A simple program that will encode the characters of a given text within the pixels of a given image.
It will also decode an image which has previously had a text encoded in it.

## General Info
The program is compiled into an executable called steganography and it takes two command line arguments.
The first argument is the the path to the image you want to use and the second argument specifies whether
you want to encode or decode the image. To have the program encode text the second argument should be 1, to
decode text the second argument should be 2. If you select to encode text in the image the text will be read
in from stdin. If you select to decode an image the text will be printed to stdout.

## Technologies
Project is created with:
* C++14
* SFML version 2.5

