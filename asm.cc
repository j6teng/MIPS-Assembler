#include "scanner.h"
#include <iostream>
using std::vector;
using std::string;
using CS241::Token;

/**
 * C++ Starter code for CS241 A3
 * Written by Sean Harrap in Winter 2018
 * All code requires C++14, so if you're getting
 * compile errors make sure to use -std=c++14.
 *
 * This file contains the main function of your program.
 * You will want to begin by modifying the assemble() function,
 * and are welcome to add more functions and files as needed.
 */

// You can throw this class to signal exceptions encountered while assembling
class AssemblingFailure {
  std::string message;
  public:
    AssemblingFailure(std::string message):message(message) {}

    std::string getMessage() const { return message; }
};

void putit(long long n) {
	putchar(n >> 24);
	putchar(n >> 16);
	putchar(n >> 8);
	putchar(n);
}

int checkposi(vector<string> vs, string s) {
	int i = 0;
	while (i<vs.size()) {
		if (vs[i] == s) {
			return i;
		}
		i++;
	}
	return -1;
}

void assemble(vector<vector<Token>> &tokenLines) {
	int ind = 0;
	vector<int> address;
	vector<string> labelname;
	int er;
	//run asm for the first time, only focus on the label's address;
	for (int j = 0; j < tokenLines.size(); j++) {
		vector<Token> line = tokenLines[j];
		if (line.size() != 0) {
			if (line[0].getKind() == "LABEL") {
				string s = line[0].getLexeme();
				er = checkposi(labelname, s.substr(0, s.length() - 1));
				if (er != -1) {
					std::cerr << "ERROR: same name label" << std::endl;
					return;
				}
		//		std::cerr << "should go line61" << std::endl;
				labelname.push_back(s.substr(0, s.length() - 1));
				address.push_back(ind * 4);
				int posi = 1;
				while (line.size() > posi && line[posi].getKind() == "LABEL") {
		//			std::cerr << "shouldnot go line66" << std::endl;
					string s = line[posi].getLexeme();
					er = checkposi(labelname, s.substr(0, s.length() - 1));
					if (er != -1) {
						std::cerr << "ERROR: same name label" << std::endl;
						return;
					}
					labelname.push_back(s.substr(0, s.length() - 1));
					address.push_back(ind * 4);
					posi++;
				}
				if (line.size() == posi+2 && line[posi].getKind() == "WORD") {
					std::cerr << "shouldnot go line78" << std::endl;
					if (line.size() > posi+1 &&
						(line[posi+1].getKind() == "HEXINT"
							|| line[posi+1].getKind() == "INT"
							|| line[posi+1].getKind() == "ID")) {
						ind++;
					}
					else {
						std::cerr << "ERROR" << std::endl;
						return;
					}
				}


				else if (line.size() == posi + 7 && line[posi].getKind() == "ID") {
					if (line[posi].getLexeme() == "sw" || line[posi].getLexeme() == "lw") {
						if (line[posi+1].getKind() == "REG"
							&&line[posi+2].getKind() == "COMMA"
							&& (line[posi+3].getKind() == "INT" || line[4].getKind() == "HEXHINT")
							&& line[posi+4].getKind() == "LPAREN"
							&&line[posi+5].getKind() == "REG"
							&&line[posi+6].getKind() == "RPAREN")
						{

							if (line[posi+3].getKind() == "INT" && (line[posi+3].toLong() > 32767 || line[posi+3].toLong() < -32768)) {
								std::cerr << "ERROR :range" << std::endl;
								return;
							}
							else if (line[posi+3].getKind() == "HEXINT" && (line[posi+3].toLong() > 0xffff || line[posi+3].toLong() < 0)) {
								std::cerr << "ERROR:range" << std::endl;
								return;
							}
							ind++;

						}
						else {
							std::cerr << "ERROR::: not follow 7" << std::endl;
						}
					}
					else {
						std::cerr << "ERROR::: 7 not sw lw" << std::endl;
					}
				}

				else if (line.size() == posi + 4 && line[posi].getKind() == "ID") {
					if (line[posi].getLexeme() == "div" || line[posi].getLexeme() == "divu" || line[posi].getLexeme() == "mult" || line[posi].getLexeme() == "multu") {
						if (line.size() == posi+4
							&& line[posi+1].getKind() == "REG"
							&&line[posi+2].getKind() == "COMMA"
							&&line[posi+3].getKind() == "REG"
							) {
							ind++;
						}
						else {
							std::cerr << "ERROR:DMDM" << std::endl;
							return;
						}
					}
					else {
						std::cerr << "ERROR N4" << std::endl;
						return;
					}
				}
				else if (line.size() == posi +2 &&line[posi].getKind() == "ID") {
		//			std::cerr << "shouldnot go line 90 "<< std::endl;
					if (line[posi].getLexeme() == "jr" || line[posi].getLexeme() == "jalr") {
						if (line[posi+1].getKind() == "REG") {
							ind++;
						}
						else {
							std::cerr << "ERROR" << std::endl;
							return;
						}
					}
					else if (line[posi].getLexeme() == "lis" || line[posi].getLexeme() == "mfhi" || line[posi].getLexeme() == "mflo") {
						if (line[posi+1].getKind() == "REG") {
							ind++;
						}
						else {
							std::cerr << "ERROR" << std::endl;
							return;
						}

					}
					else {
						std::cerr << "ERROR" << std::endl;
						return;
					}

				}
				else if (line.size() == posi + 6 && line[posi].getKind() == "ID") 
				{
			//		std::cerr << "shouldnot go line107" << std::endl;
					if (line[posi].getLexeme() == "add" ||
						line[posi].getLexeme() == "sub" ||
						line[posi].getLexeme() == "slt" ||
						line[posi].getLexeme() == "sltu")
						
					{//sub add slt sltu
						if (line[posi+ 1].getKind() == "REG"
							&&line[posi + 2].getKind() == "COMMA"
							&& line[posi + 3].getKind() == "REG"
							&& line[posi + 4].getKind() == "COMMA"
							&& line[posi + 5].getKind() == "REG") {
							ind++;
						}
						else {
							std::cerr << "ERROR" << std::endl;
							return;
						}

					}
					else if (line[posi].getLexeme() == "bne" || line[posi].getLexeme() == "beq")
					{
						if (line[posi + 1].getKind() == "REG"
							&&line[posi + 2].getKind() == "COMMA"
							&& line[posi + 3].getKind() == "REG"
							&& line[posi + 4].getKind() == "COMMA"
							&& (line[posi + 5].getKind() == "HEXINT"||line[posi+5].getKind() == "INT"||line[posi+5].getKind() == "ID")) {
							if (line[posi + 5].getKind() == "INT" && (line[posi + 5].toInt() > 32767 || line[posi + 5].toInt() < -32768)) {
								std::cerr << "ERROR :range" << std::endl;
								return;
							}
							else if (line[posi + 5].getKind() == "HEXINT" && (line[posi + 5].toInt() > 0xffff || line[posi + 5].toInt() < 0)) {
								std::cerr << "ERROR:range" << std::endl;
								return;
							}
							ind++;
						}
						else {
							std::cerr << "ERROR" << std::endl;
							return;
						}
					}
					else {
						std::cerr << "ERROR" << std::endl;
						return;
					}


				}
				else if(line.size()>1){
					std::cerr << "ERROR:undefine after label" << std::endl;
					return;
				}

			}
			else if (line[0].getKind() == "WORD") {
				if (line.size() == 2 &&
					(line[1].getKind() == "HEXINT"
						|| line[1].getKind() == "INT"
						|| line[1].getKind() == "ID")) {
					ind++;
				}else{
					std::cerr << "ERROR" << std::endl;
					return;
				}
			}//checking for jr/jalr
			else if (line[0].getKind() == "ID") {
				if (line[0].getLexeme() == "jr" || line[0].getLexeme() == "jalr") {
					if (line.size() == 2 && line[1].getKind() == "REG") {
						ind++;
					}
					else {
						std::cerr << "ERROR" << std::endl;
						return;
					}
				}
				else if (line[0].getLexeme() == "add" ||
					line[0].getLexeme() == "sub" ||
					line[0].getLexeme() == "slt" ||
					line[0].getLexeme() == "sltu"
					)
				{//sub add slt sltu
					if (line.size() == 6 && line[1].getKind() == "REG"
						&& line[3].getKind() == "REG"
						&& line[5].getKind() == "REG"
						&&line[2].getKind() == "COMMA"
						&&line[4].getKind() == "COMMA") {
						ind++;
					}
					else {
						std::cerr << "ERROR" << std::endl;
						return;
					}

				}
				else if (line[0].getLexeme() == "lis" || line[0].getLexeme() == "mfhi" || line[0].getLexeme() == "mflo") {
					if (line.size() == 2 && line[1].getKind() == "REG") {
						ind++;
					}
					else {
						std::cerr << "ERROR" << std::endl;
						return;
					}

				}
				else if (line[0].getLexeme() == "sw" || line[0].getLexeme() == "lw") {
					if (line.size() == 7
						&& line[1].getKind() == "REG"
						&&line[2].getKind() == "COMMA"
						&& (line[3].getKind() == "INT" || line[3].getKind() == "HEXINT")
						&& line[4].getKind() == "LPAREN"
						&&line[5].getKind() == "REG"
						&&line[6].getKind() == "RPAREN"
						)
					{
						if (line[3].getKind() == "INT" && (line[3].toLong() > 32767 || line[3].toLong() < -32768)) {
							std::cerr << "ERROR :range" << std::endl;
							return;
						}
						else if (line[3].getKind() == "HEXINT" && (line[3].toLong() > 0xffff || line[3].toLong() < 0)) {
							std::cerr << "ERROR:range" << std::endl;
							return;
						}
						ind++;

					}else{
						std::cerr << "ERROR:::" << std::endl;
						return;
					}

				}
				else if (line[0].getLexeme() == "div" || line[0].getLexeme() == "divu" || line[0].getLexeme() == "mult" || line[0].getLexeme() == "multu") {
					if (line.size() == 4
						&& line[1].getKind() == "REG"
						&&line[2].getKind() == "COMMA"
						&&line[3].getKind() == "REG"
						) {
						ind++;
					}
					else {
						std::cerr << "ERROR:DMDM" << std::endl;
						return;
					}
			
				}

				else if (line[0].getLexeme() == "bne" || line[0].getLexeme() == "beq")
				{
			//		std::cerr << "should goes here" << std::endl;
			
					if (line.size() == 6
						&&line[1].getKind() == "REG"
						&&line[2].getKind() == "COMMA"
						&& line[3].getKind() == "REG"
						&& line[4].getKind() == "COMMA"
						&& (line[5].getKind() == "HEXINT" || line[5].getKind() == "INT"||line[5].getKind() == "ID" )) {

						if (line[5].getKind() == "INT" && (line[5].toLong() > 32767 || line[5].toLong() < -32768)) {
							std::cerr << "ERROR :range" << std::endl;
							return;
						}
						else if (line[5].getKind() == "HEXINT" && (line[5].toLong() > 0xffff || line[5].toLong() < 0)) {
							std::cerr << "ERROR:range" << std::endl;
							return;
						}

						ind++;
					//		std::cerr << "should goes here" << std::endl;
					}
					else {
						std::cerr << "ERROR" << std::endl;
						return;
					}
				}
				else {
					std::cerr << "ERROR" << std::endl;
					return;
				}
			}
			else {
				std::cerr << "ERROR" << std::endl;
				return;
			}
		}
	}	
	//now start to putchar 
	int codeline = 0;
	for (int j = 0; j < tokenLines.size(); j++) {
		vector<Token> line = tokenLines[j];
		if (line.size() != 0) {
			for (int i = 0; i < line.size(); i++) {
				if (line[i].getKind() == "WORD") {
					codeline += 4;
					for (int k = i + 1; k < line.size(); k++) {
						if (line[k].getKind() == "HEXINT" ||
							line[k].getKind() == "INT") {
							long long n = line[k].toLong();
							putit(n);
						}
						else if (line[k].getKind() == "ID") {
							int p = checkposi(labelname, line[k].getLexeme());
							if (p < 0) {
								std::cerr << "ERROR:undefined label" << std::endl;
								return;
							}
							int value = address[p];
							long long lval = (long long)value;
							putit(lval);
						}
						else {
							std::cerr << "ERROR:invalid input" << std::endl;
							return;
						}
					}
					i = line.size();
				}//adding jr/jalr
				else if (line[i].getKind() == "ID" &&
					(line[i].getLexeme() == "jr" || line[i].getLexeme() == "jalr")) {
					codeline += 4;
					int sssss = line[i + 1].toInt();
					if (line[i].getLexeme() == "jr") {
						int jr = 0x8;
						jr = jr | (sssss << 21);
						putit(jr);
					}
					else {
						int jalr = 0x9;
						jalr = jalr | (sssss << 21);
						putit(jalr);
					}
					i = line.size();
				}
				else if (line[i].getKind() == "ID" &&
					(line[i].getLexeme() == "sub" || line[i].getLexeme() == "add"
						|| line[i].getLexeme() == "slt" ||
						line[i].getLexeme() == "sltu")
					) {
					codeline += 4;
					int d = line[i + 1].toInt();
					int s = line[i + 3].toInt();
					int t = line[i + 5].toInt();
					if (line[i].getLexeme() == "sub") {
						int sub = 0x22;
						sub = sub | (s << 21);
						sub = sub | (t << 16);
						sub = sub | (d << 11);
						putit(sub);
					}
					else if (line[i].getLexeme() == "add") {
						int add = 0x20;
						add = add | (s << 21);
						add = add | (t << 16);
						add = add | (d << 11);
						putit(add);
					}
					else if (line[i].getLexeme() == "slt") {
						int slt = 0x2A;
						slt = slt | (s << 21);
						slt = slt | (t << 16);
						slt = slt | (d << 11);
						putit(slt);
					}
					else {
						int sltu = 0x2B;
						sltu = sltu | (s << 21);
						sltu = sltu | (t << 16);
						sltu = sltu | (d << 11);
						putit(sltu);
					}
					i = line.size();
				}
				else if (line[i].getKind() == "ID" &&
					(line[i].getLexeme() == "beq" || line[i].getLexeme() == "bne")) {
					if (line[i + 5].getKind() == "ID") {
						int p = checkposi(labelname, line[i + 5].getLexeme());
						if (p < 0) {
							std::cerr << "ERROR:undefined label" << std::endl;
							return;
						}
						codeline += 4;
						int add = address[p];
						int ii = (add - codeline) / 4;
						//		std::cerr << ii << std::endl;
						//		std::cerr << p << std::endl;
						int s = line[i + 1].toInt();
						int t = line[i + 3].toInt();
						if (line[i].getLexeme() == "beq") {
							int beq = 0x10000000;
							beq = beq | (s << 21);
							beq = beq | (t << 16);
							beq = beq | (ii & 0xFFFF);
							putit(beq);
						}
						else {
							int bne = 0x14000000;
							bne = bne | (s << 21);
							bne = bne | (t << 16);
							bne = bne | (ii & 0xFFFF);
							putit(bne);
						}

					}
					else {
						int s = line[i + 1].toInt();
						int t = line[i + 3].toInt();
						long ii = line[i + 5].toLong();
						if (line[i].getLexeme() == "beq") {
							int beq = 0x10000000;
							beq = beq | (s << 21);
							beq = beq | (t << 16);
							beq = beq | (ii & 0xFFFF);
							putit(beq);
						}
						else {
							int bne = 0x14000000;
							bne = bne | (s << 21);
							bne = bne | (t << 16);
							bne = bne | (ii & 0xFFFF);
							putit(bne);
						}
					}
					i = line.size();

				}
				else if (line[i].getKind() == "ID" &&
					(line[i].getLexeme() == "mult" || line[i].getLexeme() == "multu" || line[i].getLexeme() == "div" || line[i].getLexeme() == "divu")) {
					codeline += 4;
					int s = line[i + 1].toInt();
					int t = line[i + 3].toInt();
					if (line[i].getLexeme() == "mult") {
						int mult = 0x18;
						mult = mult | (s << 21);
						mult = mult | (t << 16);
						putit(mult);
					}
					else if (line[i].getLexeme() == "multu") {
						int multu = 0x19;
						multu = multu | (s << 21);
						multu = multu | (t << 16);
						putit(multu);
					}
					else if (line[i].getLexeme() == "div") {
						//			if (t == 0) {
							//		std::cerr << "ERROR:fenzi no zero" << std::endl;
								//	}
						int div = 0x1A;
						div = div | (s << 21);
						div = div | (t << 16);
						putit(div);
					}
					else {
						//		if (t == 0) {
							//	std::cerr << "ERROR:fenzi no zero uuuuu" << std::endl;
								//}
						int divu = 0x1B;
						divu = divu | (s << 21);
						divu = divu | (t << 16);
						putit(divu);
					}


				}
				else if (line[i].getKind() == "ID" &&
					(line[i].getLexeme() == "mfhi" || line[i].getLexeme() == "lis" || line[i].getLexeme() == "mflo")) {
					codeline += 4;
					int d = line[i + 1].toInt();
					if (line[i].getLexeme() == "lis") {
						int lis = 0x14;
						lis = lis | (d << 11);
						putit(lis);
					}
					else if (line[i].getLexeme() == "mfhi") {
						int mfhi = 0x10;
						mfhi = mfhi | (d << 11);
						putit(mfhi);
					}
					else {
						int mflo = 0x12;
						mflo = mflo | (d << 11);
						putit(mflo);
					}

				}
				else if (line[i].getKind() == "ID" &&
					(line[i].getLexeme() == "sw" || line[i].getLexeme() == "lw")) {
					codeline += 4;
					int t = line[i + 1].toInt();
					int ii = line[i + 3].toLong();
					int s = line[i + 5].toInt();
					if (line[i].getLexeme() == "sw") {
						int sw = 0xAC000000;
						sw = sw | (s << 21);
						sw = sw | (t << 16);
						sw = sw | (ii & 0xFFFF);
						putit(sw);
					}
					else {
						int lw = 0x8C000000;
						lw = lw | (s << 21);
						lw = lw | (t << 16);
						lw = lw | (ii & 0xFFFF);
						putit(lw);
					}
				}
			}
		}
	}
}
							


									

// Convert the input into a sequence of tokens.
// This should not need to be modified, but you can if you want to.
int main() {
  CS241::AsmDFA theDFA;
  vector<vector<Token>> tokenLines;
  string line;

  try {
    while (getline(std::cin, line)) {
      tokenLines.push_back(theDFA.scan(line));
    }
  } catch (CS241::ScanningFailure &f) {
    std::cerr << f.getMessage() << std::endl;

    return 1;
  }

  try {
    assemble(tokenLines);
  } catch (AssemblingFailure &f) {
    std::cerr << f.getMessage() << std::endl;

    return 1;
  }

  return 0;
}

