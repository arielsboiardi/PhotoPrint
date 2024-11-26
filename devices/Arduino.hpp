#include<iostream>

#include<asio.hpp>

class Arduino 
{

public:
	Arduino(asio::io_service& io, const std::string& port_name);
	~Arduino();

	void LEDSwitch(std::string state);
	
private: 

	asio::serial_port serial;
	asio::streambuf serialData;
	std::istream serialStream;
	std::string serialMessage;
};

Arduino::Arduino(asio::io_service& io, const std::string& port_name): 
	serial(io), 
	serialStream(&serialData)
{
	try
	{
		serial.open(port_name);	
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	serial.set_option(asio::serial_port_base::baud_rate(9600));
	serial.set_option(asio::serial_port_base::character_size(8));

	std::size_t readBits = asio::read_until(serial, serialData, "\r\n");
	std::getline(serialStream, serialMessage);
	serialData.consume(readBits);


};

Arduino::~Arduino()
{
	serial.close();
};

void Arduino::LEDSwitch(std::string state)
{
    serial.write_some(asio::buffer(state));
}
