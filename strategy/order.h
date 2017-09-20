#ifndef GUARD_ORDER
#define GUARD_ORDER

#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>

#include <currency.h>
#include <string>
#include <iosfwd>

namespace msim {

	typedef unsigned long int Amount;
	typedef unsigned long int Identity;	
	typedef Currency Price;
	enum order_type{ BUY, SELL };

	// basic order
	class Order{
	public:
		Order() {}
		Order(Price c, Amount q, order_type t)
			: _price(c), _quantity(q), _type(t) {}
		Order(const Order& order){
			_id = order._id;
			_price = order._price;
			_quantity = order._quantity;
			_type = order._type;
		}

		Identity id() const { return _id; }
		Price price() const { return _price; }
		Amount quantity() const { return _quantity; }
		order_type type() const { return _type; }

		friend std::ostream& operator<<(std::ostream&, const Order&);
	protected:
		Identity _id; // order id
		Price _price; // price of each share
		Amount _quantity; // quantity of shares	
		order_type _type; // buy or sell order

		//serialization
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive &ar, const unsigned int version){
			ar & _id;
			//ar & _price; // add serialization to Currency.h
			ar & _quantity;
			ar & _type;
		}

		// UNIMPLEMENTED:
		void generateId(){ _id = 1; } // generates unique id for this order
	};

	enum order_status{ CONFIRMED, FAILED };

	// order report - provides status of an order
	class OrderReport{
	public:
		OrderReport(Identity id, order_status st)
			: _id(id), _status(st) {}

		Identity id() const { return _id; }
		order_status status() const { return _status; }
	private:
		Identity _id;
		order_status _status;
	};	

	typedef std::string Symbol;
	enum stock_order_type{ MARKET, LIMIT };	

	//Order Implementations:

	// stock order
	class StockOrder : public Order{		
	public:
		StockOrder() {}
		StockOrder(Price c, Amount q, order_type t,
			Symbol s, stock_order_type st) : Order(c, q, t),
			_symbol(s), _stype(st) {}
		StockOrder(const StockOrder& order) : Order(order){
			_symbol = order._symbol;
			_stype = order._stype;
		}

		Symbol symbol() const { return _symbol; }
		stock_order_type stype() const { return _stype; }

		friend std::ostream& operator<<(std::ostream&, const StockOrder&);
	protected:
		Symbol _symbol; // stock symbol
		stock_order_type _stype; 

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive &ar, const unsigned int version){
			//tells compiler what the base object is:
			ar & boost::serialization::make_nvp("Order", boost::serialization::base_object<Order>(*this));
			ar & _symbol;
			ar & _stype;
		}
	};

}
#endif // !GUARD_ORDER
