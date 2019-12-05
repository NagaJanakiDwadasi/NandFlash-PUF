This code performs program disturb by programming a page and obseving bit flips in the adjacent pages by read operation and comparison with FF.<br/>
<br/>
Steps performed in the program are:<br/>
<br/>
&nbsp;> Initialization of Hardware Abstraction Layer (HAL)<br/>
&nbsp;> Configuration of System Clock<br/>
&nbsp;> Initialization of GPIO, FMC, UART<br/>
&nbsp;> Initialization of NAND Controller<br/>
&nbsp;&nbsp;&nbsp;&nbsp;> Device configuration with timings (setup time, wait time, hold time, etc..)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;> Features initialization (BANK, ECC computation, Block size, page size, etc..)<br/>
&nbsp;> Define and initialize address for read/write<br/>
&nbsp;> Erase Block<br/>
&nbsp;> Load transmission buffer with AA for page size<br/>
&nbsp;> Program page 2 of Block 1 with AA for 10,000 iterations<br/>
&nbsp;> After 10k iterations read two immediate adjacent pages of the programmed page to observe bit flips from erased state<br/>
&nbsp;> Identify stable bytes by comparison<br/>
&nbsp;> Identify the indexes of these stable bytes in the page and print them<br/>
<br/> Byte comparison logic is available in this code. Bitwise comparison logic is available in same page program disturb code.
