#pragma once
#include "boost/asio/basic_socket.hpp"
#include "boost/system/detail/error_code.hpp"
#include "boost/system/error_code.hpp"
namespace ctools{
	class Client{
	public:
		virtual bool isclose()=0;
		virtual bool needwaitreturn()=0;
		virtual void getback()=0;
		virtual void write_some(std::string&,boost::system::error_code&)=0;
		virtual std::string read_some(boost::system::error_code&)=0;
	};
}