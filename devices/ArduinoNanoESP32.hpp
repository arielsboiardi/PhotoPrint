#include<iostream>
#include<asio.hpp>

class ArduinoNanoESP32 
{

public:
	ArduinoNanoESP32(asio::io_service& io, const std::string& port_name);
	~ArduinoNanoESP32();
	
	// Stage control functions
	void move_stage(int x, int y);
	void set_stage_speed(int speed);
	void stop_stage();

	// Projector control functions
	void projector_on(bool toggle);

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

// Stage control functions

void ArduinoNanoESP32::move_stage(int x, int y)
{
	// Move stage
	std::string command = "stage -move " + std::to_string(x) + "," + std::to_string(y) + "\n";
	asio::write(serial, asio::buffer(command, command.length()));
};

void ArduinoNanoESP32::set_stage_speed(int speed)
{
	// Set stage speed
	std::string command = "stage -speed " + std::to_string(speed) + "\n";
	asio::write(serial, asio::buffer(command, command.length()));
};

void ArduinoNanoESP32::stop_stage()
{
	// Stop stage
	asio::write(serial, asio::buffer("stage -stop\n", 12));
};

// Projector control functions
void ArduinoNanoESP32::projector_on(bool toggle=true)
{
	if (toggle)
	{
		// Turn on projector
		asio::write(serial, asio::buffer("projector -on\n", 14));
	}
	else
	{
		// Turn off projector
		asio::write(serial, asio::buffer("projector -off\n", 15));
	}
}
