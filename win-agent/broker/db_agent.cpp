// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.
#include "stdafx.h"

#include "local_rpc_handler.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TPipeServer.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift\protocol\TProtocolTap.h>

#include <boost\thread.hpp>
#include <boost\thread\mutex.hpp>

#include <deque>
#include <vector>

#include "DatabaseInterface.h"
#include "RabbitMessageSender.h"

#include "DatabaseWriter.h"
#include "MessageSender.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::rpc::thrift;

std::deque<EventSample> message_queue;
std::deque<std::vector<uint8_t> > raw_message_queue;
boost::mutex queue_mutex;
boost::mutex queue_mutex2;



int main(int argc, char **argv) {


	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}

	DatabaseWriter dbWriter(boost::posix_time::millisec(1000));
	MessageSender msgSender(boost::posix_time::millisec(3000));

	boost::thread db_writer(dbWriter);
	boost::thread db_reader(msgSender);

	shared_ptr<LocalRpcHandler> handler(new LocalRpcHandler(message_queue));
	shared_ptr<TProcessor> processor(new LocalRpcProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TPipeServer("\\\\.\\pipe\\Pipe1"));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	
	shared_ptr<TProtocolFactory> protocolFactory(new TJSONProtocolFactory());
	//shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
	server.serve();
	

	// klient ktory wysyla to co jest w bazie	
	// sprawdza czy jest cos do wyslania w bazie (od najstarszych)
	// sprawdza czy jest polaczenie z serwerem
	// wysyla

	return 0;
}

