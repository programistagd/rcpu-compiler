/* 
 * File:   Command.h
 * Author: radek
 *
 * Created on 28 wrzesień 2013, 20:12
 */

#ifndef COMMAND_H
#define	COMMAND_H

#include <string>

class Command {
public:
    Command(std::string code);
    virtual ~Command();
    
    std::string _code;
    unsigned short id;
    std::string label;
    std::string acronymA, acronymB;
    enum AcronymType{
        Label, Variable //TODO function
    };
    
    short getSize();
    unsigned short bytes[3];
    
private:

};

#endif	/* COMMAND_H */

