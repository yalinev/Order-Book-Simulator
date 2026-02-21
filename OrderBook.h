#pragma once 
#include <unordered_map> 
#include <map> 
#include <deque>

// enum class Side, buy vs sell
// enum = user defined type, access w/ :: 
enum class Side {Buy, Sell};

// Action type, add or delete
enum class ActionType {Add, Delete}; 

// struct Order: stores order ID, side, price, quantity
struct Order{
    int id;  
    Side side; 
    int price; 
    int quantity; 
};

// struct Trade: store one trade result: buyid, sellid, price, qunatity 
struct Trade{
    int buyID;
    int sellID;
    int price;
    int quantity;
};

// struct Event: represents an input line  
struct Event{
    ActionType action; // add or delete 

    int deleteID; // if delete 

    // if add 
    Side side; // buy or sell 
    int id; 
    int price; 
    int quantity; 
};

class OrderBook{
    private:
    // add order by id, side price, quantity
    void addOrder(const Order& order);
    void deleteOrder(int orderID); 
    // natch while bestBid >= bestAsk
    void matchOrders(); 

    // for quick delete using the ID 
    std::unordered_map<int, std::pair<Side, int>> ordersByID; 

    // bid price = highest price at which buyer is willing to pay (descending)
    // best bid = highest buy price 
    std::map<int, std::deque<Order>, std::greater<int>> bids; 

    //ask price = lowest price at which seller is willing to sell (ascending)
    // best ask = lowest sell price
    std::map<int, std::deque<Order>> asks;

    public: 
    // to take in add/delete events 
    void processEvent(const Event& event);
}; 










