##### UartTest:
The following logic has been implemented

Program Disturb – for one block
for ( 64 pages ):
	Erase block
	Write test data ( 0XAA ) to all byte/all pages
	for ( 5000 times):
		Write ( i'th page )
	for  ( 64 pages ):
		Record Bit Flips corresponding to each page
  
##### NandFlashDataParse.py

- Find indexes of all the stable bits
- Find Correlation ( Bits that are constantly showing the EXACT same value without flip )
- Generating the fingerprint for the device
  
