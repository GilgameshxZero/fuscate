#include <memory>
#include <rain.hpp>

#include <fuscate.hpp>

#include <client_mode/server.hpp>
#include <server_mode/server.hpp>

#include "../build/version.hpp"
#include "rain/networking/socket.hpp"

int main(int argc, char const *argv[]) {
	return Rain::Error::consumeThrowable(
		[argc, argv]() {
			std::srand(static_cast<unsigned int>(time(nullptr)));
			std::cout << "fuscate " << FUSCATE_VERSION_MAJOR
								<< "." << FUSCATE_VERSION_MINOR << "."
								<< FUSCATE_VERSION_REVISION << "."
								<< FUSCATE_VERSION_BUILD << " / rain "
								<< RAIN_VERSION_MAJOR << "."
								<< RAIN_VERSION_MINOR << "."
								<< RAIN_VERSION_REVISION << "."
								<< RAIN_VERSION_BUILD << std::endl;

			// Parse command line options.
			bool argHelp{false};
			std::string argDirection{"server"},
				argListen{"0.0.0.0:0"}, argSpeak{"127.0.0.1:80"},
				argKey{""};

			Rain::String::CommandLineParser parser;
			parser.addParser("h", argHelp);
			parser.addParser("help", argHelp);
			parser.addParser("direction", argDirection);
			parser.addParser("listen", argListen);
			parser.addParser("speak", argSpeak);
			parser.addParser("key", argKey);
			try {
				parser.parse(argc - 1, argv + 1);
			} catch (...) {
				std::cout
					<< "Failed to parse command-line options. "
						 "Consider running with -h."
					<< std::endl;
				return -1;
			}

			if (argHelp) {
				std::cout << "Not yet implemented. Please read the "
										 "`readme.md` for now."
									<< std::endl;
				return 0;
			}
			bool isServerMode;
			if (argDirection == "server") {
				isServerMode = true;
			} else if (argDirection == "client") {
				isServerMode = false;
			} else {
				std::cout << "Invalid `direction` provided."
									<< std::endl;
				return -1;
			}

			std::unique_ptr<Rain::Networking::SocketInterface>
				server;
			if (isServerMode) {
				Fuscate::ServerMode::server();
			} else {
				Fuscate::ClientMode::server();
			}

			std::cout << "Listening to commands..." << std::endl;
			std::string command;
			while (true) {
				std::getline(std::cin, command);
				Rain::String::toLower(
					Rain::String::trimWhitespace(command));
				if (command == "help") {
					std::cout
						<< "Command options:\n"
						<< "help: Display this help message.\n"
						<< "exit: Attempt graceful shutdown and exit.\n"
						<< std::endl;
				} else if (command == "exit") {
					break;
				} else {
					std::cout << "Invalid command: " << command << '.'
										<< std::endl;
				}
			}

			// Attempt graceful close.
			std::cout << "Attempting graceful close of server..."
								<< std::endl;
			server.reset();
			std::cout << "Gracefully closed server." << std::endl;
			return 0;
		},
		RAIN_ERROR_LOCATION)();
}
