const unsigned int DEFAULT_WORD = 0b11111111111100000000000000000001;


struct OutputBuffer
{
    unsigned char encodedBits[16];
    unsigned int position;
    unsigned int length;
};

void writeBitIntoBuffer(unsigned char buffer[], unsigned int position, unsigned char value)
{
    int bytePos = position / 8;
    int bitPos = position % 8;
    printk("byte: %d, bit: %d, pos: %u, value: %u\n", bytePos, bitPos, position, value);
    if (value)
    {
        buffer[bytePos] |= (1 << bitPos);
    }
    else
    {
        buffer[bytePos] &= ~(1 << bitPos);
    }
}

char readBitFromBuffer(unsigned char buffer[], unsigned int position)
{
    int bytePos = position / 8;
    int bitPos = position % 8;
    unsigned char read = buffer[bytePos] & (1 << bitPos);
    return read >> bitPos;
}

void encodePayload(struct OutputBuffer *out, unsigned int word)
{
    printk("start encoding \n");
    out->position = 0;
    int i = 0;

    while (i < 32)
    {
        if (word & (1 << i))
        {
            writeBitIntoBuffer(out->encodedBits, out->position++, 1);
            writeBitIntoBuffer(out->encodedBits, out->position++, 0);
        }
        else
        {
            writeBitIntoBuffer(out->encodedBits, out->position++, 1);
            writeBitIntoBuffer(out->encodedBits, out->position++, 1);
            writeBitIntoBuffer(out->encodedBits, out->position++, 0);
            writeBitIntoBuffer(out->encodedBits, out->position++, 0);
        }
        i++;
    }
    printk("finish encoding \n");
    out->length = out->position;
    out->position = 0;
    printk("payload length: %u", out->length);
}

void setDirection(unsigned int *word, unsigned char direction)
{
    if (direction > 1){
        printk("unvalid param, must be 0 or 1");
    }
    *word |= (direction << 18);
}


void setSpeed(unsigned int *word, unsigned char speed) {
    *word |= (speed << 2); 
}

void setMotor(unsigned int *word, unsigned char addr) {
    *word |= (addr << 12);
}