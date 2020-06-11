# Emil Carr-Ross' Image Processing Programs

These are a few programs I wrote to process images, useful for making interesting graphics.

### pixelsorting

Sorts pixels using bubblesort for a cool 'glitch' effect

![Black Lives Matter gif made with pixelsorting](https://emilcarr.scot/wp-content/uploads/2020/06/blm-red.gif)

> USAGE: pixelsorting [, options] infile outfile
> 
> Pixel sort an image with various options:
>   -h [ --help ]                   Display this help message and exit.
>   -i [ --infile ] arg (=in.jpg)   The input file for processing.
>   -o [ --outfile ] arg (=out.jpg) The file in which to store the processed 
>                                   output.
>   -d [ --depth ] arg (=50)        The maximum number of sorting passes.
>   -t [ --threshold ] arg (=0)     The threshold for pixel swapping.
>   -a [ --axis ] arg (=x)          The axis along which to sort (x,y,-x,-y,l,-l)
>                                   (chainable).
>   --pswap arg (=0)                % probability to ignore pixel swap.
>   --paxis arg (=0)                % probability to skip one row/column.

### itoascii

Convert an image to ascii art. Uses a precompiled brightness map to map a pixel's perceived brightness to an ascii character.

> USAGE: itoascii [, options] infile
> 
> Convert an image to ASCII art.:
>   -h [ --help ]                 Display this help message and exit.
>   --infile arg (=in.jpg)        The image to convert to ASCII.
>   -s [ --downscaling ] arg (=4) The downscaling factor.

## License ©

This project is Copyright 2020 Emil Carr Ross with the [GPL v3.0 License](https://www.gnu.org/licenses/gpl-3.0.html)

