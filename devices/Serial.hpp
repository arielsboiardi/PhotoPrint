#include<iostream>

#include<asio.hpp>

class Serial
{

public:
	Serial(asio::io_service& io);
	~Serial();

	std::vector<std::string> enumerate_ports();

private:
	asio::serial_port serial;

};

Serial::Serial(asio::io_service& io): serial(io)
{

};

Serial::~Serial()
{
	serial.close();
};

std::vector<std::string> Serial::enumerate_ports()
{

    std::vector<std::string> ports;

    char portName[10];
    for (int i = 0; i < 10; i++) {
        try {
            sprintf_s(portName, "COM%i", i);
            serial.open(portName);
            if (serial.is_open()) 
            {
                ports.push_back(portName);
                serial.close();
            }
        }
        catch (...) {
            // std::cout << printf("Port COM%i is not a possible USB port\n", i);
        }
        
    }

    return ports;
}
