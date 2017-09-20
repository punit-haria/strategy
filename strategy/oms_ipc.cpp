#include <sstream>
#include <boost\archive\text_oarchive.hpp>

#include <ipc.h>
#include <order.h>
#include <oms.h>

#include <iostream>

namespace msim{ // trader

	void trader_report_thread(ipc_client*);

	TraderOMS_IPC::TraderOMS_IPC(const char* market_host, const char* order_port, const char* report_port) :
		order_stream(market_host, order_port){}//, 
		//report_stream(market_host, report_port), 
		//report_thread(trader_report_thread, &report_stream)	{

	//}

	void TraderOMS_IPC::executeOrder(const Order& order){
		// stringstream for serialization
		std::cout << "creating archive from stringstream\n";
		std::ostringstream oss;
		// serialize object
		boost::archive::text_oarchive archive(oss);
		std::cout << "attempting to serialize\n";
		archive << order;
		std::cout << "serialization complete, sending data\n";
		// execute order
		order_stream << oss.str().c_str();
		std::cout << oss.str().size() << std::endl;
		std::cout << oss.str() << std::endl;
		std::cout << "sent data\n";
	}

	void TraderOMS_IPC::getNewReports(){

	}

	TraderOMS_IPC::~TraderOMS_IPC(){
		// wait for report thread to finish
		std::cout << "closing...\n";
		//report_thread.join();

		//report_stream.disconnect();
		order_stream.disconnect();
		std::cout << "closed\n";
	}

	// report thread - may need a mutex for ipc_client
	void trader_report_thread(ipc_client* report_stream){
		char buf[100];
		int numbytes;
		// receive loop
		//while( ( numbytes = (*report_stream >> buf) ) > 0){
		// do stuff with received reports
		//}
		// exits when market closes connection
		report_stream->disconnect();
	}
}

namespace msim{ // market

	void market_order_thread(ipc_server*);

	MarketOMS_IPC::MarketOMS_IPC(const char* order_port, const char* report_port) :
		order_stream(order_port), 
		//report_stream(report_port),
		order_thread(market_order_thread, &order_stream){
			//report_stream.wait_for_incoming();

	}

	void MarketOMS_IPC::getNewOrders(){

	}

	void MarketOMS_IPC::sendOrderReport(const OrderReport& or){

	}

	MarketOMS_IPC::~MarketOMS_IPC(){
		// wait for report thread to finish
		order_thread.join();
		//report_stream.close_client();
		order_stream.close_client();
	}

	// report thread - may need a mutex for ipc_server
	void market_order_thread(ipc_server* order_stream){
		order_stream->wait_for_incoming();
		std::cout << "received order connection from trader\n";
		// receive byte data
		char buf[150];
		int numbytes;
		while((numbytes = ((*order_stream) >> buf)) > 0);
		std::cout << "received order from trader\n";
		std::cout << buf << std::endl;
		StockOrder order;		
		try{
			// stringstream for serialization
			std::stringstream iss(buf);			
			// unserialize object
			boost::archive::text_iarchive archive(iss);			
			std::cout << "attempting to unserialize\n";
			archive >> order;
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;		
		}
		std::cout << "unserialized order\n";
		// print order
		std::cout << order;
		// close client connection
		std::cout << "closing...\n";
		order_stream->close_client();
	}

}
