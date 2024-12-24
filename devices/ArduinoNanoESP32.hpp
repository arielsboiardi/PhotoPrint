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

	void led_on(int led_id)
	void led_off(int led_id)

private: 

	asio::serial_port serial;
	asio::streambuf serialData;
	std::istream serialStream;
	std::string serialMessage;
};

ArduinoNanoESP32::ArduinoNanoESP32(asio::io_service& io, const std::string& port_name): 
	serial(io), 
{
	// Open the serial port for Arduino Nano
	serial.open(port_name);
	serial.set_option(asio::serial_port::baud_rate(9600));
	serial.set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
	serial.set_option(asio::serial_port::parity(asio::serial_port::parity::none));
	serial.set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
	serial.set_option(asio::serial_port::character_size(asio::serial_port::character_size(8)));
};

ArduinoNanoESP32::~ArduinoNanoESP32()
{
	serial.close();
};

void ArduinoNanoESP32::led_on(int led_id)
{
	asio::write(serial, asio::buffer(std::to_string(led_id)+" on"));
};