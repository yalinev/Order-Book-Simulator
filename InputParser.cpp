#include <bits/stdc++.h>
#include "OrderBook.h"
#include "InputParser.h"
using namespace std;

// used to trim the whitespace (front & back)
string trim(const string &s){
    // i = front, j = back 
    size_t i = 0, j = s.size();
    while(i < j && isspace(s[i])) i++; 
    while(j > i && isspace(s[j - 1])) j--; 

    return s.substr(i, j - i); 
}

// split at commas and return vector of strings
vector<string> splitInput(const string& input){
    vector<string> modified; 
    string curr; 

    for(char c: input){
        if(c == ','){
            modified.push_back(trim(curr));
            // clear for next char after comma 
            curr = ""; 
        }else{
            curr.push_back(c); 
        }
    }
    modified.push_back(trim(curr));
    return modified;
}

// Assume only "B" || "S" for buy or sell 
Side getSide(const string& s){
    if(s == "B") return Side::Buy; 
    if(s == "S") return Side::Sell; 

    // default to buy 
    return Side::Buy; 
}

Event getEvent(const string& input){
    // call splitInput() to put in a vector 
    vector<string> splittedInput = splitInput(input);

    Event e{}; 

    // action (add/delete), id, side (buy/sell), price, quantity
    // "A/D, int, B/S, int, int"

    // get first char of first string 
    char action = splittedInput[0][0]; 

    // if ADD 'A'
    if(action == 'A'){
        e.action = ActionType::Add; 
        e.id = stoi(splittedInput[1]); 
        e.side = getSide(splittedInput[2]); 
        e.price = stoi(splittedInput[3]); 
        e.quantity = stoi(splittedInput[4]); 
        e.deleteID = -1; // set as invalid for ADD 
    // else DELETE, "D, deleteID"
    }else{ 
        e.action = ActionType::Delete; 
        e.deleteID = stoi(splittedInput[1]); 
        e.id = -1; // DNE 
        e.side = Side::Buy; // Buy is default 
        e.price = 0; 
        e.quantity = 0; 
    }
    return e; 
}

