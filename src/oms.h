#ifndef GUARD_COMMUNICATION
#define GUARD_COMMUNICATION

#include <thread>

namespace msim{

	class Order;
	class OrderReport;

	// order management system for traders
	class TraderOMS{
	public:	
		TraderOMS(){}
		virtual ~TraderOMS(){}
		virtual void executeOrder(const Order&) = 0; // execute an order (i.e. send to market)
		virtual void getNewReports() = 0; // returns any newly received reports
	private:
		TraderOMS(const TraderOMS&);
		TraderOMS& operator=(const TraderOMS&);
	};

	// order management system for markets
	class MarketOMS{
	public:
		MarketOMS(){}
		virtual ~MarketOMS(){}
		virtual void getNewOrders() = 0; // returns any newly received orders
		virtual void sendOrderReport(const OrderReport&) = 0; // send order report to trader
	private:
		MarketOMS(const MarketOMS&);
		MarketOMS& operator=(const MarketOMS&);
	};

	// implementation details:

	// order management systems via interprocess communication
	class ipc_server;
	class ipc_client;

	class TraderOMS_IPC : public TraderOMS{
	public:
		TraderOMS_IPC(const char* market_host, const char* order_port, const char* report_port);
		~TraderOMS_IPC();
		void executeOrder(const Order&);
		void getNewReports();
	protected:
		// note: initialization lists work in  this order:
		ipc_client order_stream;
		ipc_client report_stream;
		std::thread report_thread;
	private:
		TraderOMS_IPC(const TraderOMS_IPC&);
		TraderOMS_IPC& operator=(const TraderOMS_IPC&);
	};

	class MarketOMS_IPC : public MarketOMS{
	public:
		MarketOMS_IPC(const char* order_port, const char* report_port);
		~MarketOMS_IPC();
		void getNewOrders();
		void sendOrderReport(const OrderReport&);
	protected:
		ipc_server order_stream;
		ipc_server report_stream;
		std::thread order_thread;
	private:
		MarketOMS_IPC(const MarketOMS_IPC&);
		MarketOMS_IPC& operator=(const MarketOMS_IPC&);
	};

}

#endif // !GUARD_COMMUNICATION
