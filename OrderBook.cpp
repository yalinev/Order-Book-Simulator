#include "OrderBook.h"

#include <iostream>

using namespace std;

// add order by id, side price, quantity
// unordered_map<int, pair<Side, int>> ordersByID;
void OrderBook::addOrder(const Order& order) {
  ordersByID[order.id] = {order.side, order.price};

  // check if buy or sell
  if (order.side == Side::Buy) {
    // add to bids
    bids[order.price].push_back(order);
  } else {
    // add to asks
    asks[order.price].push_back(order);
  }
}

// remove order by id
// key: orderID, value: pair<side, price>
void OrderBook::deleteOrder(int orderID) {
  // check if it exists in book
  auto it = ordersByID.find(orderID);
  if (it == ordersByID.end()) {
    return;
  }

  // get side and price
  Side side = it->second.first;
  int price = it->second.second;

  // check which side (buy/sell) and delete from corresponding map
  // if buy, delete from bids || if sell, delete from asks
  if (side == Side::Buy) {
    auto ordersAtPrice = bids.find(price);
    // if exists
    if (ordersAtPrice != bids.end()) {
      auto& orders = ordersAtPrice->second;

      for (auto dequeIt = orders.begin(); dequeIt != orders.end(); ++dequeIt) {
        if (dequeIt->id == orderID) {
          orders.erase(dequeIt);
          break;
        }
      }
      // check if empty
      if (orders.empty()) {
        bids.erase(ordersAtPrice);
      }
    }
  } else {
    auto ordersAtPrice = asks.find(price);
    // if exists
    if (ordersAtPrice != asks.end()) {
      auto& orders = ordersAtPrice->second;

      for (auto dequeIt = orders.begin(); dequeIt != orders.end(); ++dequeIt) {
        if (dequeIt->id == orderID) {
          orders.erase(dequeIt);
          break;
        }
      }
      if (orders.empty()) {
        asks.erase(ordersAtPrice);
      }
    }
  }
  ordersByID.erase(it);
}

// match while bestBid >= bestAsk
// bids: descending, highest price first
// asks: ascending, lowest price first
void OrderBook::matchOrders() {
  // loop while bid and ask not empty and best bid >= best ask
  while (!bids.empty() && !asks.empty()) {

    // segfaulted w/o 
    if(bids.begin()->second.empty()){
        bids.erase(bids.begin());
        continue;
    }

    if(asks.begin()->second.empty()){
        asks.erase(asks.begin());
        continue;   
    }

    // get best queue levels, (best bid and ask price)
    auto bidIt = bids.begin();
    auto askIt = asks.begin();

    // get exact price of best bid and ask
    int bestBid = bidIt->first;
    int bestAsk = askIt->first;

    // if ask is higher than bid, not possible to match
    if (bestBid < bestAsk) {
      break;
    }

    // access queues, map<price, deque<orders>>
    auto& bidOrders = bidIt->second;
    auto& askOrders = askIt->second;

    // get from the front of the queue
    Order& currBuy = bidOrders.front();
    Order& currSell = askOrders.front();

    // get trade quantity = min(bid quantity, ask quantity)
    int tradeNum = min(currBuy.quantity, currSell.quantity);

    // assuming trade price = ask price
    cout << "------------------------------------------------\n"
         << "Trade Executed:\n"
         << "Buy ID: " << currBuy.id << ", "
         << "Sell ID: " << currSell.id << ", "
         << "Price: " << bestAsk << ", "
         << "Quantity: " << tradeNum << endl;

    // update quantities after trade
    currBuy.quantity -= tradeNum;
    currSell.quantity -= tradeNum;

    // check if quantity is 0, if so remove from ordersByID and q front
    if (currBuy.quantity == 0) {
      ordersByID.erase(currBuy.id);
      bidOrders.pop_front();
    }

    // same for sell side
    if (currSell.quantity == 0) {
      ordersByID.erase(currSell.id);
      askOrders.pop_front();
    }

    // update queue levels (bid and ask maps)
    // delet if no more orders at that price level
    if (bidOrders.empty()) {
      bids.erase(bidIt);
    }

    if (askOrders.empty()) {
      asks.erase(askIt);
    }
  }
}

// to take in add/delete events
void OrderBook::processEvent(const Event& event) {
  // check if delete or add
  if (event.action == ActionType::Add) {
    Order order{event.id, event.side, event.price, event.quantity};
    addOrder(order);
    // only match if adding order
    matchOrders();
  } else {
    deleteOrder(event.deleteID);
  }
}
