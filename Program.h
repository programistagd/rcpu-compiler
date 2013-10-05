/* 
 * File:   Program.h
 * Author: radek
 *
 * Created on 28 wrzesień 2013, 20:18
 */

#ifndef PROGRAM_H
#define	PROGRAM_H

#include <istream>
#include <vector>
#include <map>
class Program;
class Acronyms;
#include "Command.h"

class Acronyms{
    public:
        std::map<std::string, unsigned short> labels;
        std::map<std::string, unsigned short> variables;
        //TODO functions
};

class Program {
public:
    Program(std::istream& stream);
    virtual ~Program();
    
    std::vector<unsigned short> getByteCode();
    
    Acronyms acronyms;
    
private:
    void readProgram(std::istream& stream);
    void scanForAcronyms();
    void resolveAcronyms();
    
    std::vector<Command> commands;
    
};

#endif	/* PROGRAM_H */

