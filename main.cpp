



#include <iostream>
#include <fstream>
#include "Program.h"

int main(int argc, char**argv){
    bool verbose = true;
    if(argc>1){
        if(argc>2){
            if(std::string(argv[2])=="q")
                verbose=false;
        }
        if(argc>3){
            if(std::string(argv[3])=="q")
                verbose=false;
        }
        std::string fname = argv[1];
        if(verbose)std::cout<<"Opening "<<fname<<"...\n";
        std::string extension=fname.substr(fname.rfind('.')+1);
        bool highlevel = false;
        if(extension=="rasm"){
            highlevel = false;
        }
        else if(extension=="rbas"){
            highlevel = true;
        }
        else{
            if(verbose)std::cout<<"Wrong extension!\n";
            return -2;
        }
        std::ifstream input(fname);
        if(!input.is_open()){
            if(verbose)std::cout<<"File not available!\n";
            return -1;
        }
        if(verbose)std::cout<<"Compiling "<<fname<<"...\n";
        std::vector<unsigned short> bytes;
        try{
            Program program(input, highlevel);
            input.close();
            bytes = program.getByteCode();
        }
        catch(CompilationException e){
            if(verbose)std::cout<<"Compilation error!\n"<<e.getMessage()<<"\n";
            return -3;
        }
        std::string outname = "out.bin";
        if(argc>2 and std::string(argv[2])!="q"){
            outname = argv[2];
        }
        if(verbose)std::cout<<"Sources compiled\nSaving to "<<outname<<"\n";
        std::ofstream out(outname, std::ios::binary|std::ios::out|std::ios::trunc);
        out.write((char*)bytes.data(), bytes.size()*sizeof(unsigned short));
        out.flush();
        out.close();
        return 0;
    }
    else{
        if(verbose)std::cout<<"Usage:\n\trcompiler filename [out filename] [q]\n\n\t\tfilename:\n\t\t\t*.rasm - simple assembler compilation\n\t\t\t*.rbas - higher level compilation (nearly same syntax)\n\t\tq - quiet, don't write info\n";
        return 1;
    }
}

/*
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>

using namespace std;

map<string,unsigned short> vars;
int varpointer=4097;

int atoi(string s){
	if (s[0]=='\'' and s[2]=='\''){
		return (int)s[1];
	}
	if (s[0]=='$')
	{
		if(vars.find(s)==vars.end()){
			if(varpointer>65535){
				cout<<"\nError! Too many variables declared!\n\n";
				return 0;
			}
			vars[s]=varpointer;
			varpointer++;
		}
		return vars[s];
	}
	stringstream ss(s);
	int i;
	ss>>i;
	return i;
}

int main(int argc, char* argv[]){
	cout<<"Preparing...\n";
	string inputfile;
	string outputfile="";
	bool verbose=false;
if(argc>2){
		outputfile=argv[2];
		inputfile=argv[1];
		if (argc>3){
			verbose=true;
		}
}
else{
		cout<<"Not enough or too many arguments specified\nUsage: rcpuasm input [output] [-v]\n\n";
		return 2;
	}
	if(outputfile==""){
		outputfile=inputfile.substr(0,inputfile.length()-inputfile.find('.'))+".bin";
	}
	cout<<"Source file: "<<inputfile<<"\n";
	cout<<"Output file: "<<outputfile<<"\n";
	ifstream input(inputfile.c_str(),ios::in);
	vector<unsigned short> program;
	map<string,int> labels;
	map<int,string>	labelpointers;
		cout<<"Starting compilation...\n";
		int numline=0;
		int programpos=0;
	while(!input.eof()){
		string line;
		getline(input,line);
		if(line=="")
		{
			if(verbose)
				cout<<"Line "<<numline<<" is empty. Skipping\n";
			numline++;
			continue;
		}
		if(verbose)
			cout<<"Line "<<numline<<": "<<line<<": ";
		vector<string> tokens;
		istringstream iss(line);
		copy(istream_iterator<string>(iss),
		         istream_iterator<string>(),
		         back_inserter<vector<string> >(tokens));
		int s=0;
		if(tokens[s][0]==':'){
			s=1;
			labels[tokens[0]]=programpos;
		}
		string cmd=tokens[s];
		if(verbose)
			cout<<"CMD - "<<cmd<<"\n";
		if(cmd=="NOP"){
			programpos++;program.push_back((unsigned short)0);
		}
		else if(cmd=="INT"){
			programpos++;program.push_back((unsigned short)40);
		}
		else if(cmd=="SET"){
			programpos++;program.push_back((unsigned short)1);
			if(tokens.size()==3+s){
				programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
				programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
			}
			else{
				cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
				return 1;
			}
		}
		else if(cmd=="SETV"){
					programpos++;program.push_back((unsigned short)11);
					if(tokens.size()==3+s){
						if(tokens[s+1]=="CUR"){
							programpos++;program.push_back((unsigned short)0);}
						else
							{programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));}
						if(tokens[s+2][0]==':'){
							labelpointers[programpos]=tokens[s+2];
							programpos++;
							program.push_back(99);
						}
						else{
						programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));}
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="SETA"){
					programpos++;program.push_back((unsigned short)21);
					if(tokens.size()==3+s){
						programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
						programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="ADD"){
					programpos++;program.push_back((unsigned short)2);
					if(tokens.size()==3+s){
						programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
						programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="ADDV"){
					programpos++;program.push_back((unsigned short)12);
					if(tokens.size()==3+s){
						programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
						programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="ADDA"){
					programpos++;program.push_back((unsigned short)22);
					if(tokens.size()==3+s){
						programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
						programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="SUB"){
							programpos++;program.push_back((unsigned short)3);
							if(tokens.size()==3+s){
								programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
								programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
							}
							else{
								cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
								return 1;
							}
						}
				else if(cmd=="SUBV"){
							programpos++;program.push_back((unsigned short)13);
							if(tokens.size()==3+s){
								programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
								programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
							}
							else{
								cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
								return 1;
							}
						}
				else if(cmd=="SUBA"){
							programpos++;program.push_back((unsigned short)23);
							if(tokens.size()==3+s){
								programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
								programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
							}
							else{
								cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
								return 1;
							}
						}
				else if(cmd=="MUL"){
									programpos++;program.push_back((unsigned short)4);
									if(tokens.size()==3+s){
										programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
										programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
									}
									else{
										cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
										return 1;
									}
								}
						else if(cmd=="MULV"){
									programpos++;program.push_back((unsigned short)14);
									if(tokens.size()==3+s){
										programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
										programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
									}
									else{
										cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
										return 1;
									}
								}
						else if(cmd=="MULA"){
									programpos++;program.push_back((unsigned short)24);
									if(tokens.size()==3+s){
										programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
										programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
									}
									else{
										cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
										return 1;
									}
								}
						else if(cmd=="DIV"){
											programpos++;program.push_back((unsigned short)5);
											if(tokens.size()==3+s){
												programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
												programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
											}
											else{
												cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
												return 1;
											}
										}
								else if(cmd=="DIVV"){
											programpos++;program.push_back((unsigned short)15);
											if(tokens.size()==3+s){
												programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
												programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
												if(atoi(tokens[s+2])==0)
													cout<<"Warning! There will be division by zero.\n";
											}
											else{
												cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
												return 1;
											}
										}
								else if(cmd=="DIVA"){
											programpos++;program.push_back((unsigned short)25);
											if(tokens.size()==3+s){
												programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
												programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
											}
											else{
												cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
												return 1;
											}
										}
								else if(cmd=="MOD"){
													programpos++;program.push_back((unsigned short)6);
													if(tokens.size()==3+s){
														programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
														programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
													}
													else{
														cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
														return 1;
													}
												}
										else if(cmd=="MODV"){
													programpos++;program.push_back((unsigned short)16);
													if(tokens.size()==3+s){
														programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
														programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
														if(atoi(tokens[s+2])==0)
															cout<<"Warning! There will be division by zero.\n";
													}
													else{
														cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
														return 1;
													}
												}
										else if(cmd=="MODA"){
													programpos++;program.push_back((unsigned short)26);
													if(tokens.size()==3+s){
														programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
														programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
													}
													else{
														cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
														return 1;
													}
												}

		else if(cmd=="AND"){
					programpos++;program.push_back((unsigned short)7);
					if(tokens.size()==3+s){
						programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
						programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="ANDV"){
					programpos++;program.push_back((unsigned short)17);
					if(tokens.size()==3+s){
						programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
						programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="ANDA"){
					programpos++;program.push_back((unsigned short)27);
					if(tokens.size()==3+s){
						programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
						programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="OR"){
							programpos++;program.push_back((unsigned short)8);
							if(tokens.size()==3+s){
								programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
								programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
							}
							else{
								cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
								return 1;
							}
						}
				else if(cmd=="ORV"){
							programpos++;program.push_back((unsigned short)18);
							if(tokens.size()==3+s){
								programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
								programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
							}
							else{
								cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
								return 1;
							}
						}
				else if(cmd=="ORA"){
							programpos++;program.push_back((unsigned short)28);
							if(tokens.size()==3+s){
								programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
								programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
							}
							else{
								cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
								return 1;
							}
						}
				else if(cmd=="XOR"){
									programpos++;program.push_back((unsigned short)9);
									if(tokens.size()==3+s){
										programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
										programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
									}
									else{
										cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
										return 1;
									}
								}
						else if(cmd=="XORV"){
									programpos++;program.push_back((unsigned short)19);
									if(tokens.size()==3+s){
										programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
										programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
									}
									else{
										cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
										return 1;
									}
								}
						else if(cmd=="XORA"){
									programpos++;program.push_back((unsigned short)29);
									if(tokens.size()==3+s){
										programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
										programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
									}
									else{
										cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
										return 1;
									}
								}
		else if(cmd=="NOT"){
			programpos++;program.push_back((unsigned short)31);
			if(tokens.size()==2+s){
				programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
			}
			else{
				cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
				return 1;
			}
		}
		else if(cmd=="PUSH"){
					programpos++;program.push_back((unsigned short)35);
					if(tokens.size()==2+s){
						programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="PUSHV"){
					programpos++;program.push_back((unsigned short)36);
					if(tokens.size()==2+s){
						if(tokens[s+1][0]==':'){
							labelpointers[programpos]=tokens[s+1];
							programpos++;
							program.push_back(99);
						}
						else{
						programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));}
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="POP"){
					programpos++;program.push_back((unsigned short)37);
					if(tokens.size()==2+s){
						programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
					}
					else{
						cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
						return 1;
					}
				}
		else if(cmd=="IFE"){
											programpos++;program.push_back((unsigned short)32);
											if(tokens.size()==3+s){
												programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
												programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
											}
											else{
												cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
												return 1;
											}
										}
		else if(cmd=="IFG"){
											programpos++;program.push_back((unsigned short)33);
											if(tokens.size()==3+s){
												programpos++;program.push_back((unsigned short)atoi(tokens[s+1]));
												programpos++;program.push_back((unsigned short)atoi(tokens[s+2]));
											}
											else{
												cout<<"Error! Too few or too many arguments for "<<cmd<<"\n\tIt takes exactly 2 arguments\n\tCheck your syntax\n";
												return 1;
											}
										}
		else{
			cout<<"Error! Unknown command "<<cmd<<"\n\tCheck your syntax\n";
			return 1;
		}
//			for(int i=0;i<tokens.size();i++)
//				cout<<"Token: \""<<tokens[i]<<"\"\n";

		numline++;
	}
	input.close();
	cout<<"Program commands parsed.\n";
	cout<<"Parsing addresses... [feature incomplete]\n";//
	for(map<int,string>::iterator it=labelpointers.begin();it!=labelpointers.end();it++){
		if(labels.find((*it).second)==labels.end()){
			cout<<"Error! Undefined reference to "<<(*it).second<<"\n";
			return 4;
		}
		program[(*it).first]=labels[(*it).second];
		if(verbose)
			cout<<(*it).first<<" - "<<(*it).second<<" - "<<labels[(*it).second]<<"\n";
	}
	cout<<"Saving file...\n";
	ofstream out(outputfile.c_str(),ios::out|ios::trunc|ios::binary);
	unsigned short * data=new unsigned short[program.size()];
	for(int i=0;i<program.size();i++){
		data[i]=program[i];
	}
	out.write((char*)data,sizeof(unsigned short)*program.size());
	out.close();
	cout<<"Done.\n\n";
	if(verbose){
		cout<<"Summary:\n"
				<<"Program consists of "<<program.size()<<" words\n"
				<<"\tthis is "<<program.size()*2<<" bytes\n"
				<<"Program dump:\n";
		for(int i=0;i<(program.size());i++){
				cout<<i<<" - "<<data[i]<<"\n";
			}
	}
	delete [] data;
	return 0;
}
*/