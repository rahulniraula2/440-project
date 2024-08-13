Project Build Instructions

## To Build and Run

make all > output.txt

## To Generate Assembly

make assembly

## To Specify a Compiler Optimization Level

You can use the -o flag to specify the optimization level. For example:

make -o 3 all > output.txt

This sets the optimization level to the highest (-O3). To set no optimization, you can use:

make -o 0 all > output.txt

For more details on GCC optimization options, refer to the GCC Optimize Options Documentation: https://gcc.gnu.org/onlinedocs/gcc-3.2/gcc/Optimize-Options.html#:~:text=The%20compiler%20does%20not%20perform,function%20inlining%2C%20and%20register%20renaming.

## change file being used

change line 14 on main.c char file_to_encode = "src/resources/big.txt";
