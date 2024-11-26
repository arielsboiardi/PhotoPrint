#include<iostream>

#include<asio.hpp>

class BX3SSU 
{

public:
	BX3SSU(asio::io_service& io, const std::string& port_name);
	~BX3SSU();

    void move(int x, int y);
	void setSpeed(int speed);
	bool isConnected();

	int speed = 5;
	
private:
	asio::serial_port serial;
	asio::streambuf serialData;
	std::istream serialStream;
	std::string serialMessage;

	void readAnswer(std::string &answer);
};

BX3SSU::BX3SSU(asio::io_service& io, const std::string& port_name): 
serial(io),
serialStream(&serialData)
{
	// Opening serial communication
	serial.open(port_name);
	serial.set_option(asio::serial_port_base::baud_rate(19200));
	serial.set_option(asio::serial_port_base::character_size(8));
	serial.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::two));
	serial.set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::even));
	serial.set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));

	std::vector<std::string> startup_sequence{
		"3LOG IN\r\n",
		"3UNIT?\r\n",
		"3XYINIT?\r\n",
		"3UNIT?\r\n",
		"3XYINIT\r\n",
		"3UNIT?\r\n",
		"3XYP?\r\n",
		"3JGXY 1\r\n",
		"3JGXDR 1\r\n",
		"3JGYDR 1\r\n",
		"3XYSPD "+std::to_string(speed)+"\r\n"
	};

	for (size_t step = 0; step < startup_sequence.size(); step++)
	{
		std::string command = startup_sequence[step];
		asio::write(serial, asio::buffer(command, command.length()));
		readAnswer(serialMessage);

		if(step == 0 && serialMessage != "3LOG +"){
			std::cerr << "Error: Stage not connected" << std::endl;
			break;
		}
	}
		

};

BX3SSU::~BX3SSU()
{
	// Stopping stage
	std::string command = "3LOG OUT\r\n";
	asio::write(serial, asio::buffer(command, command.length()));
	readAnswer(serialMessage);

	// Logging out
	command = "3LOG OUT\r\n";
	asio::write(serial, asio::buffer(command, command.length()));
	readAnswer(serialMessage);

	// Closing serial communication
	serial.close();
};

void BX3SSU::move(int x, int y)
{
	std::string StageMoveCmd = "3XYM";
	StageMoveCmd = StageMoveCmd+" "+std::to_string(x)+","+std::to_string(y)+"\r\n";
	asio::write(serial, asio::buffer(StageMoveCmd,StageMoveCmd.length()));

	readAnswer(serialMessage);
	std::cout << serialMessage << std::endl;
}

void BX3SSU::setSpeed(int speed)
{
	std::string StageSpeedCmd = "3XYSPD ";
	StageSpeedCmd = StageSpeedCmd+std::to_string(speed)+"\r\n";
	asio::write(serial, asio::buffer(StageSpeedCmd,StageSpeedCmd.length()));

	readAnswer(serialMessage);
	std::cout << serialMessage << std::endl;
}

void BX3SSU::readAnswer(std::string &answer)
{
	std::size_t nReadBits = asio::read_until(serial, serialData, "\r\n");
	std::getline(serialStream, answer);
	answer.pop_back();

	if(answer.substr(0,2) == "3x"){
		std::cout << "Wrong command" << std::endl;
	}
}

bool BX3SSU::isConnected()
{
	return serial.is_open();
}