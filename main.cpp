#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <stack>
#include <math.h>
#include <filesystem>

#include "ArgParser/ArgParser.h"

#define NO_NUMBER (size_t(-1))

std::streamsize get_flength(std::ifstream& file) {
	if(!file.is_open()) {
		return 0;
	}
	std::streampos temp_1 = file.tellg();
	file.seekg(0, std::fstream::end);
	std::streampos temp_2 = file.tellg();
	file.seekg(temp_1, std::fstream::beg);

	return temp_2;
}

std::string read(std::string path) {
	std::ifstream ifile;
	ifile.open(path, std::ios::binary);
	std::streamsize len = get_flength(ifile);
	char* dummy = new char[len];

	try {
		ifile.read(dummy, len);
	}
	catch(std::exception& err) {
		ifile.close();
        return "";
	}
	if (dummy == nullptr || strlen(dummy) == 0) {
		ifile.close();
        return "";
	}
	ifile.close();
	//dummy += '\0';
	std::string re;
	re.assign(dummy, len);

	delete[] dummy;
	dummy = nullptr;

	return re;
}

class intAndString {
    bool retString = false;
    int intVal = 0;
    std::string strVal = "";
    
public:
    void* current;  
    intAndString() {

    }
    intAndString(int i, std::string s) {
        intVal = i;
        strVal = s;
    }

    std::string ret() {
        if(retString) {
            return strVal;
        }
        else {
            return std::to_string(intVal);
        }
    }
    void set(std::string val) {
        if(retString) {
            strVal = val;
        }
        else {
            intVal = val[0];
            for(size_t i = 1; i < val.size(); ++i) {
                intVal += val[i] * std::pow(10,i);
            }
        }
    }
    void set(int val) {
        if(retString) {
            strVal = char(val);
        }
        else {
            intVal = val;
        }
    }

    void swap() {
        retString = !retString;
        if(retString) {
            current = &strVal;
        }
        else {
            current = &intVal;
        }
    }

    void operator=(std::string str) {
        strVal = str;
    }
    void operator=(int i) {
        intVal = i;
    }

    bool empty() {
        if(retString) {
            return strVal == "";
        }
        else {
            return intVal == 0;
        }
    }

    bool retint() {
        return !retString;
    }
};

bool isInt(std::string str) {
    try {
        std::stoi(str);
    }
    catch(...) {
        return false;
    }
    return true;
}
std::vector<std::vector<intAndString>> vec2D;
void parse(std::string source) {
    if(source.size() == 0) {
        LOG("EMPTY FILE")
        return;
    }
    source += "\n";
    vec2D.clear();
    vec2D.push_back(std::vector<intAndString>({intAndString(0,"")}));
    size_t index_x = 0;
    size_t index_y = 0;

    std::stack<size_t> loops;
    bool jmp_dir = false; // false = -> ; true = <-
#define UP '^' 
#define DOWN 'v'
#define RIGHT '>'
#define LEFT '<'
#define SWAP '#'
#define PRINT '.'
#define INPUT '?'
#define SET ';'
#define ADD '+'
#define SUB '-'
#define LOOP_START '{'
#define LOOP_END '}'
#define COND_START '['
#define COND_END ']'
#define NEWLINE '/'
#define NUMBERFY '~'
#define EXIT '='
#define SYSTEM '!'

#define POS vec2D[index_x][index_y]

    for(size_t i = 0; i < source.size(); ++i) {
        LOG(source[i] << "(" << index_x << "|" << index_y << ") [" << POS.ret() << "]")
        if(source[i] == UP) {
            while(index_y+1 >= vec2D[index_x].size()) {
                vec2D[index_x].push_back(intAndString());
            }
            ++index_y;
        }
        else if(source[i] == DOWN) {
            if(index_y - 1 != NO_NUMBER) {
                --index_y;
            }
        }
        else if(source[i] == RIGHT) {
            while(index_x+1 >= vec2D.size()) {
                vec2D.push_back(std::vector<intAndString>({intAndString(0,"")}));
            }
            ++index_x;
        }
        else if(source[i] == LEFT) {
            if(index_x - 1 != NO_NUMBER) {
                --index_x;
            }
        }
        else if(source[i] == SWAP) {
            vec2D[index_x][index_y].swap();
        }
        else if(source[i] == PRINT) {
            std::cout << POS.ret();
        }
        else if(source[i] == INPUT) {
            std::string in;
            std::getline(std::cin,in);
            POS.set(in);
        }
        else if(source[i] == SET) {
            if(index_y -1 != NO_NUMBER) {
                POS.set(vec2D[index_x][index_y-1].ret());
            }
            else if(index_x -1 != NO_NUMBER) {
                POS.set(vec2D[index_x-1][index_y].ret());
            }
        }
        else if(source[i] == ADD) {
            if(POS.retint()) {
                POS.set(std::stoi(POS.ret())+1);
            }
            else {
                std::string add = "";
                if(index_x-1 != NO_NUMBER) {
                    add = vec2D[index_x-1][index_y].ret();
                    POS.set(POS.ret()+add);
                }
                else if(index_y-1 != NO_NUMBER) {
                    add = vec2D[index_x][index_y-1].ret();
                    POS.set(POS.ret()+add);
                }
                else {
                    POS.set(POS.ret()+"1");
                }
            }
        }
        else if(source[i] == SUB) {
            if(POS.retint()) {
                POS.set(std::stoi(POS.ret())-1);
            }
            else {
                std::string str = POS.ret();
                str.pop_back();
                POS.set(str);
            }
        }
        else if(source[i] == LOOP_START) {
            if(POS.retint() && std::stoi(POS.ret()) != 0) {
                loops.push(i);
            }
            else if(!POS.retint() && POS.ret() != "") {
                loops.push(i);
            }
            else {
                size_t save = i;
                while(source[i] != LOOP_END) {
                    if(i+1 >= source.size()) {
                        i = save;
                        break;
                    }
                    ++i;
                }
            }

        }
        else if(source[i] == LOOP_END) {
            if(!loops.empty()) {
                if(POS.retint() && std::stoi(POS.ret()) != 0) {
                    i = loops.top();
                }
                else if(!POS.retint()) {
                    i = loops.top();
                }
                else {
                    loops.pop();
                }
            }
        }
        else if(source[i] == COND_START) {
            if(!((POS.retint() && POS.ret() == "0" ) || (!POS.retint() && POS.ret() == ""))) {
                size_t save = i;
                while(source[i] != COND_END) {
                    if(i+1 >= source.size()) {
                        i = save;
                        break;
                    }
                    ++i;
                }
            }
        }
        else if(source[i] == COND_END) {
            ;
        }
        else if(source[i] == NEWLINE) {
            std::cout << "\n";
        }
        else if(source[i] == NUMBERFY) {
            if(!POS.retint() && isInt(POS.ret())) {
                POS = std::stoi(POS.ret());
            }
        }
        else if(source[i] == EXIT) {
            std::exit(POS.retint() ? std::stoi(POS.ret()):0);
        }
        else if(source[i] == SYSTEM) {
            if(POS.retint()) {
                POS.swap();
                const char* ch = POS.ret().c_str();
                system(ch);
                POS.swap();
            }
            else {
                const char* ch = POS.ret().c_str();
                system(ch);
            }
        }
        else if(source[i] == '_') {
            jmp_dir = !jmp_dir;
        }
        else if(isInt(std::string(1,source[i])) and std::stoi(std::string(1,source[i])) > 0) {
            int jmp = std::stoi(std::string(1,source[i]));
            if(i + jmp >= source.size() && !jmp_dir) {
                std::cout << "Quak! Trying to jump to a code that doesn't exist at character " << i << "\n";
                std::exit(1);
            }
            if(i - jmp >= source.size() && jmp_dir) {
                std::cout << "Quak! Trying to jump to a code that doesn't exist at character " << i << "\n";
                std::exit(1);
            }
            i += jmp * (jmp_dir ? 1 : -1);
        }
        else if(source[i] == ' ' or source[i] == '\n') {
            ;
        }
        else if(source[i] == ':') {
            bool swb = false;
            if(!POS.retint()) {
                POS.swap();
                swb = true;
            }
            POS = std::string(1,char(std::stoi(POS.ret())));
            if(swb) {
                POS.swap();
            }
        }
        else {
            // I allow comments this way i guess

            //std::cout << "Quak! unknown instruction \"" << source[i] << "\"\n";
            //std::exit(1);
        }
    }
}

int main(int argc, char** argv) {
    logging::file = "Deduck.log";
    CLEAR_LOG

    ParsedArgs pargs = ArgParser()
        .addArg("--help",ARG_TAG,{"--h"})
        .addArg("--no-log",ARG_TAG,{"--nl"})
        .addArg("-visualise",ARG_SET,{"-v","-vis"})
        .addArg("-file",ARG_SET,{"-f"})
        .parse(argv, argc);

    if(pargs["--no-log"]) {
        logging::file = "";
    }
    if(pargs["--help"]) {
        std::cout << "## Brainduck ##" << "\n"
                  << "Another brainfuck-like language but even worse" << "\n\n"
                  << "Usage: brainfuck [options] -file <file>" << "\n\n"
                  << "Options:" << "\n"
                  << "  --help          : prints this and exits" << "\n"
                  << "  --no-log        : disables the logging" << "\n"
                  //<< "  -visualise <f>  : writes after finishing the parsing process the final cells into a file" << "\n"
                  << "  -file <.bd>     : the file to parse" << "\n\n"
                  << "By SirWolf (2021)" << "\n";
        exit(0);
    }
    if(pargs("-file") == "") {
        std::cout << "Quak! No file given!\n";
        std::exit(1);
    }
    if(!std::filesystem::exists(pargs("-file"))) {
        std::cout << "Quak! File doesn't exist!\n";
        std::exit(1);
    }
    if(std::filesystem::path(pargs("-file")).extension() != ".bd" and std::filesystem::path(pargs("-file")).extension() != ".brainduck") {
        std::cout << "Quak! Duck doesn't like non- .bd files!\n";
        std::exit(1);
    }
    if(read(pargs("-file")) == "") {
        std::cout << "Quak! Empty file!\n";
        std::exit(1);
    }

    parse(read(pargs("-file")));
    /*
    if(pargs("-visualise") != "") {
        std::ofstream f;
        LOG(pargs.size() << " " << vec2D.size() << " " << pargs("-visualise"))
        f.open(pargs("-visualise"),std::ios::app | std::ios::trunc);
        
        for(size_t i = 0; i < vec2D.size(); ++i) {
            f << "\n|";
            for(size_t j = 0; j < vec2D[i].size(); ++j) {
                LOG(vec2D[i][j].ret())
                if(vec2D[i][j].retint()) {
                    f << "\t" << vec2D[i][j].ret() << "\t|";
                }
                else { //ret string
                    f << "\t\"" << vec2D[i][j].ret() << "\"\t|";
                }
            }  
        }
        f.close();
    }
    */
}