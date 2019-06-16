/*
	First name	Chun Ngai
	Last name		Lam
	Student ID	01345059 (e.g. 01234567)
	Username		cnl17 (e.g. aaa123)
*/

//IMPORTANT: before submitting your source code, please make sure to comment your main function (needed for automated testing)


#include <iostream>
#include <string>
#include <map>
//#include <ctime>
#include <vector> //or #include <list>
//#include <chrono>

// do not use using namespace std

// do not alter the struct declaration
struct hufftreenode {
	char character;
	int frequency;
	hufftreenode* left;
	hufftreenode* right;
};

// do not alter the typedef 
typedef hufftreenode* hufftreeptr;


// you can define and use any additional *structures* if you might need to

// YOUR CODE HERE FOR ADDIIONAL STRUCTS DECLARATION, IF ANY



/////////////////////////////////////////
// Functions declarations (prototypes) //
/////////////////////////////////////////

// do not alter these functions declarations

//utility function
bool valid_hufftree(const hufftreeptr hufftree);
 // returns true if the input hufftree is a valid Huffman tree
 // i.e. all the terminal nodes (leaves) have characters,
 // all the non-leaf nodes have two sub-trees each,
 // and the occurrence frequency of a non-leaf node equals
 // the sum of the occurrence frequencies of its two sub-trees.



std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable);
// encodes an input message (a string of characters) into an encoded message (string of bits) using the Huffman coding
// builds the Huffman tree and its associated encoding table (i.e. the association between the characters in the tree leaves and their encodings (string of bits))

std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree);
// decodes an encoded message (a string of bits) into the original message (a string of characters) using the Huffman tree built during the encoding


void buildHuffTable(const hufftreeptr& hufftree,std::map<char, std::string>& hufftable, std::string encodedChar);
// you can define and use any additional *functions* if you might need to

// use for analysis
// std::string randomCharGenerator(int lengthOfString);

// YOUR CODE HERE FOR ADDIIONAL FUNCTIONS DECLARATION (NOT THE IMPLEMENTATION), IF ANY




///////////////////
// main function //
///////////////////

//IMPORTANT (Reminder): before submitting your source code, please make sure to comment your main function (needed for automated testing)
//int main() {
//    // YOUR CODE HERE FOR TESTING YOUR PROGRAM
//
//    // an example of basic encoding/decoding
//
//
//    std::string message, encodedmessage, decodedmessage;
//    hufftreeptr hufftree;
//    std::map<char, int> freqtable;
//    std::map<char, std::string> hufftable;
//
//    message = "go go gophers";
//
//
//    // 1) encoding (compression)
//    encodedmessage = huffencode(message, freqtable, hufftree, hufftable);
//    // freqtable should be as in Table 1.
//    // hufftree might be as in Figure 2 (remember, the Huffman coding tree IS NOT UNIQUE. If we swap two siblings in a Huffman tree, it is still a Huffman tree of the same message.)
//    // hufftable should correspond to hufftree, and might be as in Table 2 <' ',"100">, <'g',"00">, <'o',"01">, <'p',"1110">, <'h',"1101">, <'e',"101">, <'r',"1111">, <'s',"1100">.
//    // encodedmessage might then be the 37-bits string "0001100000110000011110110110111111100" (i.e. "00.01.100.00.01.100.00.01.1110.1101.101.1111.1100")
//
//
//    if(valid_hufftree(hufftree))  {
//        std::cout << "valid Huffman tree." << std::endl;
//    }
//    else {
//        std::cout << "not valid Huffman tree." << std::endl;
//    }
//
//    // 2) decoding (uncompression)
//    decodedmessage = huffdecode(encodedmessage, hufftree);
//    // should be "go go gophers"
//
//    if(decodedmessage == message) {
//        std::cout << "decoding OK." << std::endl;
//    }
//    else {
//        std::cout << "decoding not OK." << std::endl;
//    }
//    // to view the compression ratio
////    double doubleEncodedMessageLength = encodedmessage.length() + 0;
////    double compressionRatio = 100 * doubleEncodedMessageLength/ (8 * message.length());
////    std::cout.precision(5);
////    std::cout<<"The compression ratio of this is: " << compressionRatio << "%"<< std::endl;
//
//    return 0;
//}


/////////////////////////////////////////////
// Functions definitions (implementations) //
/////////////////////////////////////////////

// YOUR CODE HERE FOR THE FUNCTIONS IMPLEMENTATIONS

bool valid_hufftree(const hufftreeptr hufftree){
    
    while(hufftree != NULL){
        if(hufftree->left == NULL && hufftree->right == NULL){
            if(hufftree->character == '\0'){
                // return false if leaf node character is empty
                return false;
            }
        }
        if((hufftree->left != NULL && hufftree->right == NULL)||(hufftree->left == NULL && hufftree->right != NULL)){
            return false;
            // if number of sub-tree is not two
        }
        if(hufftree->left != NULL && hufftree->right != NULL){
            if(hufftree->frequency != (hufftree->left->frequency + hufftree->right->frequency)){
                return false;
                // if frequency is not true
            }
        }
        return valid_hufftree(hufftree->left) & valid_hufftree(hufftree->right);
    }
    return true;
    
}

std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable){
    
    int countChar[256] = {0};
    
    //integer array to count frequency, Decimal value of the character is correspond to the array cell;
    
    for (int i = 0; message[i] != '\0'; i++) {
            countChar[message[i]]++;
    }
    
    for (int i = 0; i < 256; i++) {
        if (countChar[i] != 0) {
            freqtable[char(i)] = countChar[i];
        }
    }
    
    std::vector<hufftreeptr> forest;
    
    std::map<char, int>::iterator it;
    
    for(it = freqtable.begin(); it != freqtable.end(); it++) {
        hufftreeptr single_node_btree = new hufftreenode;
        single_node_btree->character = it->first;
        single_node_btree->frequency = it->second;
        single_node_btree->left = NULL;
        single_node_btree->right = NULL;
        forest.push_back(single_node_btree);
    }
    
    // iterates until size is 1
    while (forest.size() != 1 ) {
        hufftreeptr t1, t2, tmp;
        
        t1 = forest[0];

        int min1 = t1->frequency;
        int index = 0;
        // two for loops to find minimun frequency of the trees
        for (int i = 1; i < forest.size(); i++) {
            tmp = forest[i];
            if (min1 > tmp->frequency ) {
                t1 = forest[i];
                min1 = t1->frequency;
                index = i;
            }
        }
        
        forest.erase(forest.begin() + index);
        
        t2 = forest[0];
        int min2 = t2->frequency;
        index = 0;
    
        for (int i = 1; i < forest.size(); i++) {
            tmp = forest[i];
            if (min2 > tmp->frequency) {
                t2 = forest[i];
                min2 = t2->frequency;
                index = i;
            }
        }
        

        forest.erase(forest.begin() + index);

        hufftreeptr tmptree = new hufftreenode;
        tmptree->left = t1;
        tmptree->right = t2;
        tmptree->frequency = t1->frequency + t2->frequency;
        forest.push_back(tmptree);

        hufftree = tmptree;

    }

    std::string compressedMessage = "";

    buildHuffTable(hufftree, hufftable, compressedMessage);
    
    for (int i = 0 ; i < message.size() ; i++) {
        compressedMessage +=  hufftable.find(message[i])->second;
    }

    return compressedMessage;
}

void buildHuffTable(const hufftreeptr& hufftree,std::map<char, std::string>& hufftable, std::string encodedChar){

    hufftreeptr tmpTree = hufftree;
    
    if(tmpTree->left == NULL && tmpTree->right == NULL){
        hufftable[tmpTree->character] = encodedChar;
        // log to the hufftable when it goes to the leaf node
    }
    
    if(tmpTree->left != NULL){
        buildHuffTable(tmpTree->left, hufftable, encodedChar + "0");
    }
    
    if(tmpTree->right != NULL){
        buildHuffTable(tmpTree->right, hufftable, encodedChar + "1");
    }
    
}


std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree){

    std::string decodedMessage;
    hufftreeptr tmpTree = hufftree;
    
    for(int i = 0; i < encodedmessage.length(); i++){
        if(encodedmessage[i] == '1'){
            tmpTree = tmpTree->right;
        }
        if(encodedmessage[i] == '0'){
            tmpTree = tmpTree->left;
        }
        if(tmpTree->left == NULL && tmpTree->right == NULL){
            decodedMessage += tmpTree->character;
            tmpTree = hufftree;
        }
    }
    
    return decodedMessage;
}
