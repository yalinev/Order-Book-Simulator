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
} 

// match while bestBid >= bestAsk
// bids: descending, highest price first 
// asks: ascending, lowest price first
void OrderBook::matchOrders(){
    // loop while bid and ask not empty and best bid >= best ask
    while(!bids.empty() && !asks.empty()){

         // get best level of queue
         auto bidIt = bids.begin();
         auto askIt = asks.begin();

        // best bid and ask prices first 
         int bestBid = bidIt->first;
         int bestAsk = askIt->first;

         // check if there are still possible matches
         if(bestBid < bestAsk){
            break; 
         }

         // get queue at best prices 
         auto &bidOrders = bidIt->second;
         auto &askOrders = askIt->second;

         // get from the front of the queue 
         Order &buyOrder = bidOrders.front();
         Order &sellOrder = askOrders.front();

        // get trade quantity = min(bid quantity, ask quantity)
         int tradeNum = min(buyOrder.quantity, sellOrder.quantity); 

        // resting order price 

        // apply trade, update quantities, and remove if fully filled
            // remove from bids and ask accordingly


    }
    
}










