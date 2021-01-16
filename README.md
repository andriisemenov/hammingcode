# hammingcode
Hamming code (8,4) example implementation for C++

Inputs for this implementation are:
INPUT_LEN - number of bits to encode (must be divisible by 4)
orig_data - input data stream, array with 32-bit values, only INPUT_LEN number of bits will be encoded starting from MSB
NUM_ERRORS - number of errors introduced
error_data - array with positions to flip (simutale error)




## Example output:

Original data :

1 1 0 1 1 1 1 1 0 1 0 0 1 0 0 1 0 1 1 0 1 1 1 0 1 1 1 0 0 1 0 0 

Encoded data : 

1 1 0 0 1 1 0 0 1 1 1 1 1 1 1 1 0 1 0 1 0 1 0 1 1 0 0 1 1 0 0 1 0 1 1 0 0 1 1 0 1 1 1 1 0 0 0 0 1 1 1 1 0 0 0 0 0 1 0 1 0 1 0 1 

Introducing errors :

1 0 0 0 1 1 0 0 1 0 1 1 1 1 1 1 0 1 0 1 1 1 0 1 1 0 0 1 1 0 0 1 0 1 1 0 0 1 1 0 1 1 1 1 0 0 0 0 1 1 1 1 0 0 0 0 0 1 0 1 0 1 0 1 

Error on position 1, corrected.

Error on position 9, corrected.

Error on position 20, corrected.

1 1 0 0 1 1 0 0 1 1 1 1 1 1 1 1 0 1 0 1 0 1 0 1 1 0 0 1 1 0 0 1 0 1 1 0 0 1 1 0 1 1 1 1 0 0 0 0 1 1 1 1 0 0 0 0 0 1 0 1 0 1 0 1 

Original data : 

1 1 0 1 1 1 1 1 0 1 0 0 1 0 0 1 0 1 1 0 1 1 1 0 1 1 1 0 0 1 0 0 

Received and corrected data : 

1 1 0 1 1 1 1 1 0 1 0 0 1 0 0 1 0 1 1 0 1 1 1 0 1 1 1 0 0 1 0 0 
