# BMPIO-BITMAP-LIB-C
Useful and handy C/C++ compatiable header library for import and export bitmap images
# How to use
include the header into your project to use,this was written in C for the max performance and compatibility.
# Functions
BMPIO functions start with bmpio
include:
bmpio_load_from()
bmpio_load_as()
bmpio_set_dpi()
bmpio_loader()
bmpio_writer()
# Parameters
bmpio_load_from() takes two keywords:
1."FILE" - to load from path
2."PIPELINE" - to load from stdo
note: when load from stdo,filename(path) is the command line execution that produces a stream of desired data(rgb/rgba).

bmpio_load_as() takes two keywords:
1."RGB" - to import image as rgb
2."RGBA" - to import image as rgba

bmpio_set_dpi() takes one integer for specifying pixel to meter(optional).

bmpio_loader() takes four parameters:
1.in filename/cmd command
2.pointer to return image width
3.pointer to return image height
4.pointer to return image bitdepth
note: it returns a unsigned char pointer to the loaded image data if success otherwise NULL.

bmpio_writer() takes five parameters:
1.out filename
2.image width
3.image height
4.format(3 for rgb and 4 for rgba)
5.pointer to the image data
note: format can be greater than 4,this is agnostic format.

# Limitation
The loader can only load common 24 bits bitmap images.
