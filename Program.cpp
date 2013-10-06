/* 
 * File:   Program.cpp
 * Author: radek
 * 
 * Created on 28 wrzesień 2013, 20:18
 */

#include "Program.h"

Program::Program(std::istream& stream, bool highlevel) : needsPreprocess(highlevel) {
    readProgram(stream);
}

Program::~Program() {
}

void Program::readProgram(std::istream& stream){
    while(stream.good()){
        std::string line;
        std::getline(stream,line,'\n');
        code.push_back(line);
        //commands.push_back(Command(line));
    }
}

void Program::scanForAcronyms(){
    for(Command& cmd : commands){
        if(cmd.acronymA!="")
            acronyms.variables.insert(std::pair<std::string, unsigned short>(cmd.acronymA,0));
        if(cmd.acronymB!="")
            acronyms.variables.insert(std::pair<std::string, unsigned short>(cmd.acronymB,0));
//        if(cmd.label!="")//no need to init them
//            acronyms.labels[cmd.label]=0;
    }
}

void Program::resolveAcronyms(){
    unsigned short currentPos=0;
    for(Command& cmd : commands){
        if(cmd.label!=""){
            acronyms.labels[cmd.label]=currentPos;
        }
        currentPos+=cmd.getSize();
    }
    currentPos+=3;
    for(std::map<std::string, unsigned short>::iterator i=acronyms.variables.begin();i!=acronyms.variables.end();i++){
        i->second=currentPos;
        currentPos++;
    }
}

void Program::applyAcronyms(){
    for(Command& cmd : commands){
        if(cmd.acronymA!=""){
            if(!cmd.parseArgument(cmd.acronymA, cmd.bytes[1], &acronyms))
                throw CompilationException("Cannot find suitable acronym: probably undeclared label or other construct");
        }
        if(cmd.acronymB!=""){
            if(!cmd.parseArgument(cmd.acronymB, cmd.bytes[2], &acronyms))
                throw CompilationException("Cannot find suitable acronym: probably undeclared label or other construct");
        }
    }
}

std::vector<unsigned short> Program::getByteCode(){
    if(!compiled){
        if(needsPreprocess){
            preprocess();
        }
        precompile();
        scanForAcronyms();
//        unsigned short programLength = 0;
//        for(Command& cmd : commands){
//            programLength+=cmd.getSize();
//        }
        resolveAcronyms();
        applyAcronyms();
        bytecode.reserve(commands.size()*3);//bytecode.reserve(programLength+3);
        for(Command& cmd : commands){
            for(unsigned short i=0;i<cmd.getSize();i++){
                bytecode.push_back(cmd.bytes[i]);
            }
        }
        bytecode.push_back(23);//set cursor to 0, so program begins another repetition, that's default behavior but user can put an infinite loop or some return on the end of the file if he wants
        bytecode.push_back(13);
        bytecode.push_back(0);
    }
    return bytecode;
}

void Program::precompile(){
    for(std::string& line : code){
        commands.push_back(Command(line));
    }
}

void Program::preprocess(){
    
}