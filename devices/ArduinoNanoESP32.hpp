#include<iostream>
#include<asio.hpp>

class ArduinoNanoESP32 
{

public:
	ArduinoNanoESP32(asio::io_service& io, const std::string& port_name);
	~ArduinoNanoESP32();
	
	void move_stage(int x, int y);
	void set_stage_speed(int speed);
	void is_connected();

private: 

	asio::serial_port serial;

};

ArduinoNanoESP32::ArduinoNanoESP32(asio::io_service& io, const std::string& port_name): serial(io)
{
	// Open the serial port for Arduino Nano
	serial.open(port_name);
	serial.set_option(asio::serial_port::baud_rate(9600));
	serial.set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
	serial.set_option(asio::serial_port::parity(asio::serial_port::parity::none));
	serial.set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
	serial.set_option(asio::serial_port::character_size(asio::serial_port::character_size(8)));

	// Start BX3SSU from Arduino Nano
	asio::write(serial, asio::buffer("stage -start\n", 13));

};

ArduinoNanoESP32::~ArduinoNanoESP32()
{
	// Log Arduino Nano out off BX3SSU controller
	asio::write(serial, asio::buffer("stage -stop\n", 12));

	// Close the serial port
	serial.close();
};
