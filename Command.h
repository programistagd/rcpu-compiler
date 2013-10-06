/* 
 * File:   Command.h
 * Author: radek
 *
 * Created on 28 wrzesień 2013, 20:12
 */

#ifndef COMMAND_H
#define	COMMAND_H

#include <string>
#include <vector>
class Command;
#include "Program.h"

struct CompilationException{
    std::string getMessage();
    CompilationException(std::string);
private:
    std::string msg;
};

class Command {
public:
    explicit Command(std::string code);
    virtual ~Command();
    
    std::vector<std::string> _code;
    unsigned short id;
    std::string label;
    std::string acronymA, acronymB;
    
    unsigned short getSize();
    unsigned short bytes[3];
    
    bool parseArgument(std::string code, unsigned short& byte, Acronyms* acronyms = NULL);
protected:
    unsigned short size;
};

#endif	/* COMMAND_H */

