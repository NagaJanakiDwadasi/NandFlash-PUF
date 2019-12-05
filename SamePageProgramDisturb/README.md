This code performs program disturb and obseves bit flips in the same page by read operation and comparison with programmed value.

Steps performed in the program are:

> Initialization of Hardware Abstraction Layer (HAL)
> Configuration of System Clock
> Initialization of GPIO, FMC, UART
> Initialization of NAND Controller 
  > Device configuration with timings (setup time, wait time, hold time, etc..)
  > Features initialization (BANK, ECC computation, Block size, page size, etc..)
> Define and initialize address for read/write
> Erase Block
> Load transmission buffer with AA for page size
> Program page 2 of Block 1 with AA for 10,000 iterations
> Simultaneously read page to observe bit flips
> After last write, read the page to identify stable bits by comparison
> Identify the indexes of these stable bits in the page and print them
