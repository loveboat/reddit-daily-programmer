//
//  main.cpp
//  easy
//
//  Created by David Lusby on 17/05/2014.
//  Copyright (c) 2014 David Lusby. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <locale>

using namespace std;

typedef vector<string> strings;

bool readFile(strings& dictionary, strings& compressedText) {
    ifstream file;
    file.open("/Users/lusbyd/Developer/reddit-daily-programmer/162/easy/easy/data");
    
    bool rtn = false;
    
    string line;
    if (file.is_open()) {
        
        getline(file, line);
        int lineCount = 0;
        istringstream sstream(line);
        sstream >> lineCount;
        
        int count = 0;
        
        while (!file.eof()) {
            getline(file, line);
            
            // build goddamn fucking dictionary
            if (count < lineCount) {
                dictionary.push_back(line);
            }
            else {
                // save the macho-man text!
                compressedText.push_back(line);
            }
            count++;
        }
        rtn = true;
    }
    file.close();
    return rtn;
}

string toUpper(const string& word) {
    stringstream output;
    locale loc;
    for (string::size_type a=0; a < word.length(); ++a)
        output << toupper(word[a],loc);
    return output.str();
}

string toTitle(const string& word) {
    locale loc;
    return toupper(word[0], loc) + word.substr(1);
}

string lookupTheShit(const strings& dictionary, const string& offsetStr) {
    int offset = 0;
    istringstream sstream(offsetStr);
    sstream >> offset;
    
    return dictionary[offset];
}

string getMode(const string& token, string& offset) {
    string mode;
    char lastChar = token.at( token.length() - 1 );
    if (lastChar == '^') {
        offset = token.substr(0, token.length() - 1);
        mode = "TITLE";
    }
    else if (lastChar == '!') {
        offset = token.substr(0, token.length() - 1);
        mode = "UPPER";
    }
    else {
        offset = token;
        mode = "LOWER";
    }
    return mode;
}

int main(int argc, const char * argv[])
{
    strings dictionary, compressedText;
    if (!readFile(dictionary, compressedText)) {
        cout << "Damn, where that file at?!";
    }
    
    stringstream output;
    
    for (int i = 0; i < compressedText.size(); ++i) {
        string line = compressedText[i];
        
        istringstream iss(line);
        strings tokens;
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter<strings >(tokens));
        
        bool skipSpace = true;
        
        for (int j = 0; j < tokens.size(); ++j) {
            string token = tokens[j];
            
            if (token == "R") {
                output << endl;
            }
            else if (token == "E") {
                
                cout << output.str();
                break;
            }
            else if (token == "." || token == "-" || token == "!" ||
                     token == "," || token == "?" || token == ";" ||
                     token == ":") {
                
                if (token == "-")
                    skipSpace = true;
                
                output << token;
            }
            else {
                string offset;
                const string mode = getMode(token, offset);
                
                string word = lookupTheShit(dictionary, offset);
                if (mode == "UPPER") {
                    word = toUpper(word);
                }
                else if (mode == "TITLE") {
                    word = toTitle(word);
                }
                
                if (!skipSpace) {
                    output << " ";
                }
                skipSpace = false;
                output << word;
            }
        }
    }
    
    return 0;
}

