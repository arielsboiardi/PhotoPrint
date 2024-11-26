#include <iostream>

#include <Arduino.hpp>
#include <BX3SSU.hpp>
#include <Serial.hpp>


//  _________  _______   ________  _________        ________  _________  ________  ________  _______      
// |\___   ___\\  ___ \ |\   ____\|\___   ___\     |\   ____\|\___   ___\\   __  \|\   ____\|\  ___ \     
// \|___ \  \_\ \   __/|\ \  \___|\|___ \  \_|     \ \  \___|\|___ \  \_\ \  \|\  \ \  \___|\ \   __/|    
//      \ \  \ \ \  \_|/_\ \_____  \   \ \  \       \ \_____  \   \ \  \ \ \   __  \ \  \  __\ \  \_|/__  
//       \ \  \ \ \  \_|\ \|____|\  \   \ \  \       \|____|\  \   \ \  \ \ \  \ \  \ \  \|\  \ \  \_|\ \ 
//        \ \__\ \ \_______\____\_\  \   \ \__\        ____\_\  \   \ \__\ \ \__\ \__\ \_______\ \_______\
//         \|__|  \|_______|\_________\   \|__|       |\_________\   \|__|  \|__|\|__|\|_______|\|_______|
//                         \|_________|               \|_________|                                        
                                                                                                       
int main()
{
    asio::io_service io;

    Serial serial(io);
    auto ports = serial.enumerate_ports();

    for(const auto& port : ports)
    {
        std::cout << port << std::endl;
    }


    // BX3SSU stage(io,argv[1]);

    // stage.move(10000000,0);

    return 0;
}                                                                                                       

//  _________  _______   ________  _________        ________  ________  ________  ___  ___  ___  ________   ________     
// |\___   ___\\  ___ \ |\   ____\|\___   ___\     |\   __  \|\   __  \|\   ___ \|\  \|\  \|\  \|\   ___  \|\   __  \    
// \|___ \  \_\ \   __/|\ \  \___|\|___ \  \_|     \ \  \|\  \ \  \|\  \ \  \_|\ \ \  \\\  \ \  \ \  \\ \  \ \  \|\  \   
//      \ \  \ \ \  \_|/_\ \_____  \   \ \  \       \ \   __  \ \   _  _\ \  \ \\ \ \  \\\  \ \  \ \  \\ \  \ \  \\\  \  
//       \ \  \ \ \  \_|\ \|____|\  \   \ \  \       \ \  \ \  \ \  \\  \\ \  \_\\ \ \  \\\  \ \  \ \  \\ \  \ \  \\\  \ 
//        \ \__\ \ \_______\____\_\  \   \ \__\       \ \__\ \__\ \__\\ _\\ \_______\ \_______\ \__\ \__\\ \__\ \_______\
//         \|__|  \|_______|\_________\   \|__|        \|__|\|__|\|__|\|__|\|_______|\|_______|\|__|\|__| \|__|\|_______|
//                         \|_________|                                                                                  
                                                                                                                      
                                                                                                                      
// int main(int argc, const char *argv[])
// {
//     asio::io_service io;
    
//     Arduino arduino(io,argv[1]);
//     arduino.LEDSwitch(argv[2]);
    
//     return 0;
// }

// #include <iostream>
// #include <string>

// #include <asio.hpp>

// asio::streambuf serialData;

// asio::io_service io;
// asio::serial_port Serial(io);

// void SerialReadHandler(const asio::error_code &ec,std::size_t size)
// {
//     std::istream str(&serialData);
//     std::string line;
//     std::getline(str,line);
//     std::cout << line << std::endl;

//     asio::async_read_until(Serial,serialData,"\n",
//         [](const asio::error_code &ec,std::size_t size){SerialReadHandler(ec, size);});

// }



// int main()
// {
//     Serial.open("COM7");
//     Serial.set_option(asio::serial_port_base::baud_rate(9600));
//     Serial.set_option(asio::serial_port_base::character_size(8));

//     asio::async_read_until(Serial,serialData,"\n",
//         [](const asio::error_code &ec,std::size_t size){SerialReadHandler(ec, size);});

//     int n = 0;
//     while(true)
//     {
//     if(n%2==0)
//         asio::write(Serial,asio::buffer("X",1));
//     else    
//         asio::write(Serial,asio::buffer("Y",1));
//     n++;
//     std::cout << "Waiting for 1 second" << std::endl;
//     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//     }

//     io.run();    

//     std::cout << "Finished" << std::endl;

//     return 0;
// }