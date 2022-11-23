const unsigned int DEFAULT_WORD = 0b1111 1111 1111 0000 0000 0000 0000 0001;

unsigned int setDirection(unsigned int *word, unsigned char direction)
{
    if (direction > 1){
        printk("unvalid param, must be 0 or 1");
    }
    *word |= (direction << 18);
}


unsigned int setSpeed(unsigned int *word, unsigned char speed) {
    *word |= (speed << 2); 
}

unsigned int setMotor(unsigned int *word, unsigned char addr) {
    *word |= (addr << 12)
}