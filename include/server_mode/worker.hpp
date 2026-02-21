#pragma once

#include <rain.hpp>
#include "rain/networking/http/request.hpp"
#include "rain/networking/http/response.hpp"
#include "rain/networking/socket-options.hpp"
#include "rain/networking/specification.hpp"

namespace Fuscate::ServerMode {
	class Worker : public Rain::Networking::Http::Worker<
									 Rain::Networking::Http::Request,
									 Rain::Networking::Http::Response,
									 4096,
									 4096,
									 16,
									 16,
									 Rain::Networking::Ipv4FamilyInterface,
									 Rain::Networking::StreamTypeInterface,
									 Rain::Networking::TcpProtocolInterface,
									 Rain::Networking::NoLingerSocketOption> {
		public:
		using Super = Rain::Networking::Http::Worker<
			Rain::Networking::Http::Request,
			Rain::Networking::Http::Response,
			4096,
			4096,
			16,
			16,
			Rain::Networking::Ipv4FamilyInterface,
			Rain::Networking::StreamTypeInterface,
			Rain::Networking::TcpProtocolInterface,
			Rain::Networking::NoLingerSocketOption>;
	};
}
