/*
Travis Torline
CSCI 2270 Assignment 1
Instructor Rhonda Hoenigman
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

/*
create a structure to hold items that have no match
each item has a type, a price, and are "for sale" or "wanted"
in this structure, "for sale" is represented with 1 and "wanted" with 0
*/
struct GarageItem{

    //initialize member variables as empty
    string type = "";
    int price = 0, saleOrWanted = 0;

    //default constructor
    GarageItem(){}

    //constructor that passes through type, price, and "for sale" or "wanted"
    GarageItem(string t, int p, int sow){
        type = t;
        price = p;
        saleOrWanted = sow;
    }

};

/*
A function to remove spaces from the lines as they are read in and make all characters lowercase
Pre-conditions: currentLine is a string
Post-conditions: removeSpaces returns a string with no spaces
*/
string removeSpacesLowerCase(string currentLine);

/*
A function to check to see if there is a match to the current item being read in the listOfItemsArray
Pre-conditions: type, saleOrWanted, and price are all strings
                index is an integer that is the number of items in the listOfItems array
                listOfItems is an array of GarageItem structures
Post-conditions: checkForMatch returns the index of the match if there is a match, -1 if there isn't
*/
int checkForMatch(string type, string saleOrWanted, string price, int index, GarageItem listOfItems[]);

/*
A function to delete matches from the array listOfItems and shift the items in front of it left.
Pre-conditions: listOfItems is an array of GarageItem structures
                index is an int equal to the size of the array being passed.
                addToArray is the index being deleted and 0<=addToArray<=size of array
Post-conditions: The array that is being passed through has the matched item deleted and the items in front of it shifted left.
*/
void deleteAndShift(struct GarageItem listOfItems[], int &index, int addToArray);

/*
A function to print all items on the board that did not have a match
Pre-conditions: listOfItems is an array of the GarageItem structure
                index is an int equal to the size of the listOfItems array
Post-conditions: The Function will print out both the remaining 'wanted' and the remaining'for sale' items.
*/
void printFinalState(GarageItem listOfItems[], int index);

int main(){

    //Get the file from the user
    string filename = "";
    getline(cin,filename);

    //Open the file passed by the user
    ifstream in_stream(filename);

    //Check to make sure opening worked.
    //If it did not, output a message and return -1 so we know there was an error.
    if(in_stream.is_open()){

        //An array to store any unmatched items
        GarageItem listOfItems[100];

        //Keep track of how many items are in the array
        int index = 0;

        //Will store the index value where a match was found, or -1 if no match was found
        int addToArray;

        //use getline to read until the end of the file
        string currentLine = "";

        cout<<"Items sold:"<<endl;

        while(getline(in_stream, currentLine)){

            //remove the spaces and make lowercase in case of errors in the item's description
            currentLine = removeSpacesLowerCase(currentLine);

            //variables to store data in as we parse each line
            //since they come from a text file, they are all strings for now.
            string type, saleOrWanted, price;


            //use a stringstream to separate each line into its three parts.
            stringstream itemDescription;
            itemDescription << currentLine;

            getline(itemDescription, type, ',');

            getline(itemDescription, saleOrWanted, ',');

            getline(itemDescription, price);

            //Check to see if a match is in the listOfItems array
            addToArray = checkForMatch(type, saleOrWanted, price, index, listOfItems);

            //if no match was found, we add this item to the array following the format of the GarageItem structure
            if(addToArray == -1){
                listOfItems[index].type = type;
                listOfItems[index].price = stoi(price);

                //in the GarageItem structure, "forsale" = 1 and "wanted" = 0
                if(saleOrWanted == "forsale"){
                    listOfItems[index].saleOrWanted = 1;
                }else{
                    listOfItems[index].saleOrWanted = 0;
                }

                //Only increase the index when we add another item to the array listOfElements
                index++;
            }

            //If a match IS found, we want to print the transaction and remove the item from the array listOfItems.
            //Do NOT add the current item to the array.
            else{
                //want to print the item's actual price, not how much the buyer is willing to pay for it
                if(saleOrWanted == "forsale"){
                    cout<<type<<" "<<price<<endl;
                }else{
                    //remember, addToArray stores the index of the found match
                    cout<<type<<" "<<listOfItems[addToArray].price<<endl;
                }

                //Delete the matched element and shift the elements in front of it left.
                deleteAndShift(listOfItems, index, addToArray);
            }

        }

        //print the items that had no match
        cout<<"Items remaining in message board after reading all lines of the file:"<<endl;
        printFinalState(listOfItems, index);

    }else{
        cout<<"Error opening file!"<<endl;
        return -1;
    }

    //Make sure to close the file
    in_stream.close();
    return 0;
}

string removeSpacesLowerCase(string currentLine){

    //Store the line without spaces in newLine
    string newLine = "";

    //used to store lowercase characters of the current character being read
    char lowerCase;

    //Check each value to see whether or not it's a space.
    //If it's a space, it is not added to newLine
    //Make each character lowercase
    for(int i = 0; i < currentLine.length(); i++){
        if(!isspace(currentLine[i])){
            lowerCase = tolower(currentLine[i]);
            newLine = newLine + lowerCase;
        }
    }

    return newLine;

}

int checkForMatch(string type, string saleOrWanted, string price, int index, GarageItem listOfItems[]){

    //Since price is passed as a string, turn it to an int since that's how it is stored in the structure
    int newPrice = stoi(price);

    //loop through every item in the listOfItems array
    for(int i = 0; i < index; i++){

        //if "forsale", then we want to search for wanted items (saleOrWanted = 0) that have a price greater than or equal to the for sale item's price.
        if(saleOrWanted == "forsale"){

            if(type == listOfItems[i].type && newPrice <= listOfItems[i].price && listOfItems[i].saleOrWanted == 0){
                return i;
            }

        //Otherwise, we have a wanted item. That means we want to look for a for sale item (saleOrWanted = 1) that has a price less than or equal to the wanted item's price.
        }else{

            if(type == listOfItems[i].type && newPrice >= listOfItems[i].price && listOfItems[i].saleOrWanted == 1){
                return i;
            }

        }
    }

    //Made it through the whole array without finding a match. -1 is a value that couldn't possibly be an index.
    return -1;

}

void deleteAndShift(struct GarageItem listOfItems[], int &index, int addToArray){

    //Starting at where the match was found, make each element equal to the element on its right
    for(int i = addToArray; i < index; i++){
        listOfItems[i] = listOfItems[i + 1];
    }

    index--;

}

void printFinalState(GarageItem listOfItems[], int index){

    for(int i = 0; i < index; i++){

        //check to see if the message should have 'wanted' or 'for sale' in it.
        if(listOfItems[i].saleOrWanted == 0){
            cout<<listOfItems[i].type<<", "<<"wanted, "<<listOfItems[i].price<<endl;
        }else{
            cout<<listOfItems[i].type<<", "<<"for sale, "<<listOfItems[i].price<<endl;
        }

    }

}
