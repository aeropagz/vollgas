const unsigned int DEFAULT_WORD = 0b11111111111100000000000000000001;

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