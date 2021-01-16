/*  This example shows the implementation of the extended Hamming code to detect and 
*   correct errors in a stream of data. This code does not help with burst errors and
*   can only detect up to 2 errors and correct 1 error per 8 bit packet.
*   The example is optimized for more efficient data storage, using 1 32-bit array entry
*   for 32 bits of information.
*   
*   Implemented by Andrii Semenov */

#include <iostream>
#include <vector>

#define BUF_SIZE 50u
/*Change INPUT_LEN if adding/removing bits from orig_data[]. Should be divisible by 4*/
#define INPUT_LEN 16u
#define BITS_PER_ENTRY 32u
#define RES_LEN (INPUT_LEN * 2)
#define RES_BUF_LEN ((INPUT_LEN * 2) / BITS_PER_ENTRY) + 1
#define BIT_MASK 0x00000001u << BITS_PER_ENTRY - 1

#define DATA_BITS 4u
#define ENC_BITS 8u

#define NUM_ERRORS 3

/*Less efficient to use MSB and shift it back to 0, but more visually pleasing*/
#define GetBit(A, k) ({int retval; retval = (BIT_MASK & (A[(k / BITS_PER_ENTRY)] << (k % BITS_PER_ENTRY))) >> BITS_PER_ENTRY - 1; retval; })
/*Setting the bit: A - array, k - index, b - boolean(T/F) */
#define SetBit(A, k, b) (A[(k / BITS_PER_ENTRY)] = b ? A[(k / BITS_PER_ENTRY)] | (1 << (BITS_PER_ENTRY - 1 - (k % BITS_PER_ENTRY))) : A[(k / BITS_PER_ENTRY)] & ~(1 << (BITS_PER_ENTRY - 1 - (k % BITS_PER_ENTRY))))

/* Storing data in a 32 bit array */
uint32_t orig_data[] = {
    0b11011111010010010110111011100100,
};

uint32_t error_data[NUM_ERRORS] = {
    1, 9, 20};

uint32_t enc_data[RES_BUF_LEN];

uint32_t dec_data[(INPUT_LEN / BITS_PER_ENTRY) + 1];

void flipBit(uint32_t *data, int pos)
{
    SetBit(enc_data, pos, 0x01 & (~GetBit(enc_data, pos)));
}

static void printData(uint32_t *data, int len)
{
    for (uint32_t i = 0; i < len; i++)
    {
        std::cout << GetBit(data, i) << " ";
    }
}

static void encode(uint32_t *data, int len)
{
    /*Encoding in Extended Hamming code (8,4)*/
    /*Executing the for loop in blocks of 4 bits of original data*/

    for (int i = 0; i < (len / DATA_BITS); i++)
    {

        int cur_pos_orig = i * DATA_BITS; // Curent base position for original data
        int cur_pos_enc = i * ENC_BITS;   // Curent base position for encoded data

        SetBit(enc_data, (cur_pos_enc + 0), GetBit(data, (cur_pos_orig + 0)));
        SetBit(enc_data, (cur_pos_enc + 1), GetBit(data, (cur_pos_orig + 1)));
        SetBit(enc_data, (cur_pos_enc + 2), GetBit(data, (cur_pos_orig + 2)));
        SetBit(enc_data, (cur_pos_enc + 4), GetBit(data, (cur_pos_orig + 3)));
        /*Even parity bits*/
        SetBit(enc_data, (cur_pos_enc + 3), GetBit(enc_data, (cur_pos_enc + 0)) ^ GetBit(enc_data, (cur_pos_enc + 1)) ^ GetBit(enc_data, (cur_pos_enc + 2)));
        SetBit(enc_data, (cur_pos_enc + 5), GetBit(enc_data, (cur_pos_enc + 0)) ^ GetBit(enc_data, (cur_pos_enc + 1)) ^ GetBit(enc_data, (cur_pos_enc + 4)));
        SetBit(enc_data, (cur_pos_enc + 6), GetBit(enc_data, (cur_pos_enc + 0)) ^ GetBit(enc_data, (cur_pos_enc + 2)) ^ GetBit(enc_data, (cur_pos_enc + 4)));

        /*Overall parity*/
        SetBit(enc_data, (cur_pos_enc + 7), GetBit(enc_data, (cur_pos_enc + 0)) ^ GetBit(enc_data, (cur_pos_enc + 1)) ^ GetBit(enc_data, (cur_pos_enc + 2)) ^ GetBit(enc_data, (cur_pos_enc + 3)) ^ GetBit(enc_data, (cur_pos_enc + 4)) ^ GetBit(enc_data, (cur_pos_enc + 5)) ^ GetBit(enc_data, (cur_pos_enc + 6)));
    }
}

/*  To decode the [8,4] Hamming code, first check the parity bit. 
*   If the parity bit indicates an error, single error correction (the [7,4] Hamming code) 
*   will indicate the error location, with "no error" indicating the parity bit. 
*   If the parity bit is correct, then single error correction will indicate the (bitwise) 
*   exclusive-or of two error locations. If the locations are equal ("no error") then a 
*   double bit error either has not occurred, or has cancelled itself out. Otherwise, 
*   a double bit error has occurred.*/
static void decode(uint32_t *data, int len)
{
    for (int i = 0; i < (len / DATA_BITS); i++)
    {
        int cur_pos_enc = i * ENC_BITS;   // Curent base position for encoded data
        int cur_pos_orig = i * DATA_BITS; // Curent base position for original data
        int c1 = GetBit(enc_data, (cur_pos_enc + 6)) ^ GetBit(enc_data, (cur_pos_enc + 4)) ^ GetBit(enc_data, (cur_pos_enc + 2)) ^ GetBit(enc_data, (cur_pos_enc + 0));
        int c2 = GetBit(enc_data, (cur_pos_enc + 5)) ^ GetBit(enc_data, (cur_pos_enc + 4)) ^ GetBit(enc_data, (cur_pos_enc + 1)) ^ GetBit(enc_data, (cur_pos_enc + 0));
        int c3 = GetBit(enc_data, (cur_pos_enc + 3)) ^ GetBit(enc_data, (cur_pos_enc + 2)) ^ GetBit(enc_data, (cur_pos_enc + 1)) ^ GetBit(enc_data, (cur_pos_enc + 0));
        int c = c3 * 4 + c2 * 2 + c1;

        if (GetBit(enc_data, (cur_pos_enc + 7)) == (GetBit(enc_data, (cur_pos_enc + 0)) ^ GetBit(enc_data, (cur_pos_enc + 1)) ^ GetBit(enc_data, (cur_pos_enc + 2)) ^ GetBit(enc_data, (cur_pos_enc + 3)) ^ GetBit(enc_data, (cur_pos_enc + 4)) ^ GetBit(enc_data, (cur_pos_enc + 5)) ^ GetBit(enc_data, (cur_pos_enc + 6))))
        {
            // std::cout << "Overall parity is correct\n";
            if (c == 0)
            {
                // std::cout << "\nNo error while transmitting the data\n";
            }
            else
            {
                std::cout << "\nErrors on XOR positions: " << 7u - c << " + base position: " << cur_pos_enc << "\n";
            }
        }
        else
        {
            // std::cout << "Overall parity is incorrect\n";
            if (c == 0)
            {
                std::cout << "The parity bit is incorrect\n";
            }
            else
            {
                /*The position is inverse*/
                std::cout << "Error on position " << (cur_pos_enc + 7u - c) << ", corrected.\n";
                flipBit(enc_data, (cur_pos_enc + 7u - c));
            }
        }
        SetBit(dec_data, (cur_pos_orig + 0), GetBit(enc_data, (cur_pos_enc + 0)));
        SetBit(dec_data, (cur_pos_orig + 1), GetBit(enc_data, (cur_pos_enc + 1)));
        SetBit(dec_data, (cur_pos_orig + 2), GetBit(enc_data, (cur_pos_enc + 2)));
        SetBit(dec_data, (cur_pos_orig + 3), GetBit(enc_data, (cur_pos_enc + 4)));
    }
}

int main()
{
    std::cout << "Original data : \n";
    printData(orig_data, INPUT_LEN);
    std::cout << std::endl;

    encode(orig_data, INPUT_LEN);
    std::cout << "Encoded data : \n";
    printData(enc_data, RES_LEN);
    std::cout << std::endl;

    std::cout << "Introducing errors : \n";
    for (int i = 0; i < NUM_ERRORS; i++)
    {
        flipBit(enc_data, error_data[i]);
    }

    printData(enc_data, RES_LEN);
    std::cout << std::endl;
    decode(enc_data, RES_LEN);
    printData(enc_data, RES_LEN);
    std::cout << std::endl;

    std::cout << "Original data : \n";
    printData(orig_data, INPUT_LEN);
    std::cout << std::endl;
    std::cout << "Received and corrected data : \n";
    printData(dec_data, INPUT_LEN);
    std::cout << std::endl;
}