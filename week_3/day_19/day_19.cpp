#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<functional>
#include"opcode.h"
#include"../../Utils/utils.h"

// forward function declarations
int run_instructions(std::vector<int> reg, std::vector<std::vector<std::string>> input);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" "};
    std::vector<std::vector<std::string>> input = read_input_2D("input_19", delimiters);

    // memory 
    std::vector<int> reg(6,0);
    
    // part 1
    std::cout << "Answer (part 1): " << run_instructions(reg,input) << std::endl;

    // part 2
    // From analyzing the opcode, find the sum of divisors of 10551340
    int sum = 0;
    int n   = 10551340;
    for (int i=1; i<=n; i++){
        if (n % i == 0){ sum += i; }
    }
    std::cout << "Answer (part 2): " << sum << std::endl;

    return 0;
}

int run_instructions(std::vector<int> reg, std::vector<std::vector<std::string>> input){

    // instruction pointer (iterator)
    auto ip = reg.begin()+1;

    // function reference
    std::function<void(const int&, const int&, const int&,std::vector<int>&)> func;

    while(*ip < (int)input.size()){
        
        // chose instruction pointed to by ip
        std::vector<std::string> &line = input[*ip];

        // run approproate function
        if      (line[0]=="addr"){ func = &addr; }
        else if (line[0]=="addi"){ func = &addi; }
        else if (line[0]=="mulr"){ func = &mulr; }
        else if (line[0]=="muli"){ func = &muli; }
        else if (line[0]=="banr"){ func = &banr; }
        else if (line[0]=="bani"){ func = &bani; }
        else if (line[0]=="borr"){ func = &borr; }
        else if (line[0]=="bori"){ func = &bori; }
        else if (line[0]=="setr"){ func = &setr; }
        else if (line[0]=="seti"){ func = &seti; }
        else if (line[0]=="gtir"){ func = &gtir; }
        else if (line[0]=="gtri"){ func = &gtri; }
        else if (line[0]=="gtrr"){ func = &gtrr; }
        else if (line[0]=="eqir"){ func = &eqir; }
        else if (line[0]=="eqri"){ func = &eqri; }
        else if (line[0]=="eqrr"){ func = &eqrr; }
        else if (line[0]=="#ip" ){
            ip = reg.begin() + std::stoi(line[1]);
            input.erase(std::remove(input.begin(), input.end(), line), input.end());
            continue; 
        }

        // use function with arguments
        func(std::stoi(line[1]), std::stoi(line[2]), std::stoi(line[3]), reg);

        // increment value of ip
        (*ip)++;
    }

    return reg[0];
}
