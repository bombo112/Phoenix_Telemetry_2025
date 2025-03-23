

class send
{
private:
    uint8_t type;
    uint8_t lengde;
    uint8_t *data;
public:
    // Constructor: allocates memory for 'data' and copies the input array
    send(uint8_t TYPE, uint8_t LENGDE, uint8_t *DATA)
    {
        // Allocate memory for data
        data = new uint8_t[lengde];
        // Copy the array content from DATA to data
        memcpy(data, DATA, lengde);
    }
    
    // Destructor: free the allocated memory
    ~send()
    {
        delete[] data;
    }
};


