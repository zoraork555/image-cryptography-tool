# image-cryptography-tool
This program takes in an image in PGM format and can either encode a message into PGM or it can decode an already encoded picture.  

## Files
main.c calls the functions created in image.c  
image.c contains all functions needed for encoding and decoding pgm images  
image.h is the header file for image.c  
a.exe is the compiled executable for this project  
all image files have encoded messages for testing the program

## Running 
> ./a encode {message} {image-file} {new-image-file}  
> ./a decode {image-file}  
