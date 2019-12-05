-------- PROJECT GENERATOR --------
PROJECT NAME :	gdOS
PROJECT DIRECTORY :	C:\WorkSpace\gdOS\gdOS
CPU SERIES :	300H
CPU TYPE :	36079
TOOLCHAIN NAME :	Hitachi H8S,H8/300 Standard Toolchain
TOOLCHAIN VERSION :	6.2.1.0
GENERATION FILES :
    C:\WorkSpace\gdOS\gdOS\dbsct.c
        Setting of B,R Section
    C:\WorkSpace\gdOS\gdOS\typedefine.h
        Aliases of Integer Type
    C:\WorkSpace\gdOS\gdOS\iodefine.h
        Definition of I/O Register
    C:\WorkSpace\gdOS\gdOS\intprg.c
        Interrupt Program
    C:\WorkSpace\gdOS\gdOS\resetprg.c
        Reset Program
    C:\WorkSpace\gdOS\gdOS\gdOS.c
        Main Program
    C:\WorkSpace\gdOS\gdOS\stacksct.h
        Setting of Stack area
START ADDRESS OF SECTION :
 H'000000400	PResetPRG,PIntPRG
 H'000000800	P,C,C$DSEC,C$BSEC,D
 H'000FFE000	B,R
 H'000FFFD80	S

* When the user program is executed,
* the interrupt mask has been masked.
* 
* **** H8/36079 Advanced ****

SELECT TARGET :
    H8/300HA Simulator
DATE & TIME : 2009/01/09 21:42:01
