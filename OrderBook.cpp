#include <iostream> 
#include "OrderBook.h"

using namespace std;

// to take in add/delete events 
void OrderBook::processEvent(const Event& event){
    // check if delete or add 
    if(event.action == ActionType::Add){
        Order order{event.id, event.side, event.price, event.quantity}; 
        addOrder(order);
        // only match if adding order 
        matchOrders(); 
    }else{
        deleteOrder(event.deleteID);
    }
}

// add order by id, side price, quantity
// unordered_map<int, pair<Side, int>> ordersByID;
void OrderBook::addOrder(const Order& order){
    ordersByID[order.id] = {order.side, order.price};

    // check if buy or sell
    if(order.side == Side::Buy){
        // add to bids 
        bids[order.price].push_back(order);
    }else{
        // add to asks 
        asks[order.price].push_back(order);
    }
}

// remove order by id
// key: orderID, value: pair<side, price>
void OrderBook::deleteOrder(int orderID){
    // check if it exists in book 
    auto it = ordersByID.find(orderID); 
    if(it == ordersByID.end()){
       return; 
    }

    // get side and price
    Side side = it->second.first;
    int price = it->second.second; 

    // check which side (buy/sell) and delete from corresponding map
    // if buy, delete from bids || if sell, delete from asks
    if(side == Side::Buy){
        auto ordersAtPrice = bids.find(price); 
        // if exists
        if(ordersAtPrice != bids.end()){
            auto& orders = ordersAtPrice->second; 

            for(auto dequeIt = orders.begin(); dequeIt != orders.end(); ++dequeIt){
                if(dequeIt->id == orderID){
                    orders.erase(dequeIt); 
                    break; 
                }
            }

    }
}



// natch while bestBid >= bestAsk
void matchOrders(){

}








