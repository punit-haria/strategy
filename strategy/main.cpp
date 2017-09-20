#include <ipc.h>
#include <oms.h>
#include <order.h>

//#define SERVER
#include <Windows.h>

int main(void){
#ifdef SERVER
	msim::MarketOMS_IPC oms("3490","3491");
#else 
	//usage: market_host, order_port, report_port
	msim::TraderOMS_IPC oms("localhost","3490","3491");
	msim::StockOrder s1(12.22, 100000, msim::BUY, "AAPL", msim::MARKET);
	//msim::StockOrder s2(57.69, 500, msim::SELL, "GOOG", msim::LIMIT);
	oms.executeOrder(s1);
	Sleep(10000);
#endif
}

int entry_v0(void){
#ifdef SERVER
	msim::ipc_server ipcs("3490");
	ipcs.wait_for_incoming();
	for(int i = 0; i < 20; ++i){
		char val[] = "HELLO WORLD!!";
		ipcs.send(val);
		char val2[] = "HOW ARE YOU?";
		ipcs.send(val2);
	}
	ipcs.close_client();
#else
	msim::ipc_client ipcc("localhost","3490");
	int numbytes = 1;
	while(numbytes > 0){
		char buf[100];
		numbytes = (ipcc >> buf);
		for(int i = 0; i < numbytes; ++i)
			printf("%c",buf[i]); 
		printf("\n"); 
	}
	ipcc.disconnect();
#endif
	return 0;
}
