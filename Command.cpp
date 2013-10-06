/* 
 * File:   Command.cpp
 * Author: radek
 * 
 * Created on 28 wrzesień 2013, 20:12
 */

#include "Command.h"
#include <vector>
#include <sstream>

Command::Command(std::string code) {
    size=0;
    std::vector<std::string> tokens;
    std::stringstream tokenizer(code);
    while (tokenizer.good()){
        std::string tmp;
        tokenizer >> tmp;
        tokens.push_back(tmp);
    }
    unsigned short cursor = 0;
    if(tokens.size()==0 or tokens[0].length()==0)
        return;
    if(tokens[0].at(0)=='#')
        return;//comment
    _code=tokens;
    if(tokens[0].at(0)==':'){
        cursor=1;
        label = tokens[0].substr(1);
    }
    if(tokens.size()==cursor+1){
        if(tokens[cursor]=="NOP"){
            id=0;
        }
        else{
            throw CompilationException("Not known command "+tokens[cursor]+" taking 0 arguments\n");
        }
        size=1;
    }
    else
    if(tokens.size()==cursor+2){
        if(tokens[cursor]=="PUSH"){
            bytes[0]=35;
            id=35;
        }
        else if(tokens[cursor]=="POP"){
            bytes[0]=37;
            id=37;
        }
        else if(tokens[cursor]=="CALL"){
            bytes[0]=40;
            id=40;
        }
        else if(tokens[cursor]=="NOT"){
            bytes[0]=10;
            id=10;
        }
        else{
            throw CompilationException("Not known command "+tokens[cursor]+" taking 1 argument\n");
        }
        bool parsed=parseArgument(tokens[cursor+1],bytes[1]);
        if(!parsed){
            acronymA=tokens[cursor+1];
        }
        size=2;
    }
    else
    if(tokens.size()==cursor+3){
        if(tokens[cursor]=="ADD"){
            bytes[0]=2;
            id=2;
        }
        else if(tokens[cursor]=="SUB"){
            bytes[0]=3;
            id=3;
        }
        else if(tokens[cursor]=="MUL"){
            bytes[0]=4;
            id=4;
        }
        else if(tokens[cursor]=="DIV"){
            bytes[0]=5;
            id=5;
        }
        else if(tokens[cursor]=="MOD"){
            bytes[0]=6;
            id=6;
        }
        else if(tokens[cursor]=="AND"){
            bytes[0]=7;
            id=7;
        }
        else if(tokens[cursor]=="OR"){
            bytes[0]=8;
            id=8;
        }
        else if(tokens[cursor]=="XOR"){
            bytes[0]=9;
            id=9;
        }
        else if(tokens[cursor]=="IFE"){
            bytes[0]=16;
            id=16;
        }
        else if(tokens[cursor]=="IFG"){
            bytes[0]=17;
            id=17;
        }
        else if(tokens[cursor]=="SET"){
            if(tokens[cursor+2].at(0)=='r'){
                if(tokens[cursor+1].at(0)=='r'){
                    bytes[0]=20;
                    id=20;
                }
                else if(tokens[cursor+1].at(0)=='$' or (tokens[cursor+1].at(0)=='&' and tokens[cursor+1].at(1)=='$')){
                    bytes[0]=22;
                    id=22;
                }
                else{
                    throw CompilationException("Bad parameters to SET instruction\n");
                }
            }
            else if(tokens[cursor+1].at(0)=='r'){
                if(tokens[cursor+2].at(0)=='$'){
                    bytes[0]=21;
                    id=21;
                }
                else{
                    bytes[0]=23;
                    id=23;
                }
            }
            else{
                throw CompilationException("Bad parameters to SET instruction\n");
            }
        }
        else{
            throw CompilationException("Not known command "+tokens[cursor]+" taking 2 arguments\n");
        }
        bool parsed=parseArgument(tokens[cursor+1],bytes[1]);
        if(!parsed){
            acronymA=tokens[cursor+1];
        }
        parsed=parseArgument(tokens[cursor+2],bytes[2]);
        if(!parsed){
            acronymB=tokens[cursor+2];
        }
        size=3;
    }    
}

Command::~Command() {
}

inline unsigned short atoi(std::string a){
    std::stringstream s(a);
    unsigned short i;
    s>>i;
    return i;
}

bool Command::parseArgument(std::string code, unsigned short& byte, Acronyms* acronyms){
    char id=code.at(1);
    switch(id){
        case 'r':
            byte=atoi(code.substr(1));
            return true;
            break;
        case '\'':
            byte=(unsigned short)code.at(1);
            return true;
            break;
        case '$':
            if(acronyms==NULL){
                return false;
            }
            if(acronyms->variables.find(code.substr(1))==acronyms->variables.end()){
                return false;
            }
            byte=acronyms->variables[code.substr(1)];
            return true;
            break;
        case ':':
            if(acronyms==NULL){
                return false;
            }
            if(acronyms->labels.find(code.substr(1))==acronyms->labels.end()){
                return false;
            }
            byte=acronyms->labels[code.substr(1)];
            return true;
            break;
        case '&':
            if(code.at(2)!='$'){
                throw CompilationException("Unknown construct "+code+", & can only take $variables\n");
                return false;//TODO does it make sense?
            }
            if(acronyms==NULL){
                return false;
            }
            if(acronyms->variables.find(code.substr(2))==acronyms->variables.end()){
                return false;
            }
            byte=acronyms->variables[code.substr(2)];
            return true;
            break;
        default:
            byte=atoi(code);//TODO maybe check if this is really a number and not some other crap and repor it
            return true;
            
    }
}

unsigned short Command::getSize(){
    return size;
}

CompilationException::CompilationException(std::string s):msg(s){}
std::string CompilationException::getMessage(){return msg;}