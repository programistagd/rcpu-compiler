/* 
 * File:   Program.cpp
 * Author: radek
 * 
 * Created on 28 wrzesień 2013, 20:18
 */

#include "Program.h"

Program::Program(std::istream& stream) {
    readProgram(stream);
}

Program::~Program() {
}

void Program::readProgram(std::istream& stream){
    while(stream.good()){
        std::string line;
        stream>>line;
        commands.push_back(Command(line));
    }
}

void Program::scanForAcronyms(){
    for(Command& cmd : commands){
        if(cmd.acronymA!="")
            acronyms.variables[cmd.acronymA]=0;
        if(cmd.acronymB!="")
            acronyms.variables[cmd.acronymB]=0;
        if(cmd.label!="")
            acronyms.labels[cmd.label]=0;
    }
}

void Program::resolveAcronyms(){
    
}

std::vector<unsigned short> Program::getByteCode(){
    
}