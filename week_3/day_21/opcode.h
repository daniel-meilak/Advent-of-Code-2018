#ifndef OPCODE_H
#define OPCODE_H

#include<vector>
#include<functional>

void addr(const int &a, const int &b, const int &c, std::vector<int> &reg){
   reg[c] = reg[a] + reg[b];
}

void addi(const int &a, const int &b, const int &c, std::vector<int> &reg){
   reg[c] = reg[a] + b;
}

void mulr(const int &a, const int &b, const int &c, std::vector<int> &reg){
   reg[c] = reg[a] * reg[b];
}

void muli(const int &a, const int &b, const int &c, std::vector<int> &reg){
   reg[c] = reg[a] * b;
}

void banr(const int &a, const int &b, const int &c, std::vector<int> &reg){
   reg[c] = reg[a] & reg[b];
}

void bani(const int &a, const int &b, const int &c, std::vector<int> &reg){
   reg[c] = reg[a] & b;
}

void borr(const int &a, const int &b, const int &c, std::vector<int> &reg){
   reg[c] = reg[a] | reg[b];
}

void bori(const int &a, const int &b, const int &c, std::vector<int> &reg){
   reg[c] = reg[a] | b;
}

void setr(const int &a, const int &b, const int &c, std::vector<int> &reg){
   reg[c] = reg[a];
}

void seti(const int &a, const int &b, const int &c, std::vector<int> &reg){
   reg[c] = a;
}

void gtir(const int &a, const int &b, const int &c, std::vector<int> &reg){
   if (a > reg[b]){ reg[c] = 1; }
   else { reg[c] = 0; }
}

void gtri(const int &a, const int &b, const int &c, std::vector<int> &reg){
   if (reg[a] > b){ reg[c] = 1; }
   else { reg[c] = 0; }
}

void gtrr(const int &a, const int &b, const int &c, std::vector<int> &reg){
   if (reg[a] > reg[b]){ reg[c] = 1; }
   else { reg[c] = 0; }
}

void eqir(const int &a, const int &b, const int &c, std::vector<int> &reg){
   if (a == reg[b]){ reg[c] = 1; }
   else { reg[c] = 0; }
}

void eqri(const int &a, const int &b, const int &c, std::vector<int> &reg){
   if (reg[a] == b){ reg[c] = 1; }
   else { reg[c] = 0; }
}

void eqrr(const int &a, const int &b, const int &c, std::vector<int> &reg){
   if (reg[a] == reg[b]){ reg[c] = 1; }
   else { reg[c] = 0; }
}

void run_instructions(std::vector<int> reg, std::vector<std::vector<std::string>> input){

   // instruction pointer (iterator)
   auto ip = reg.begin()+1;

   // function reference
   std::function<void(const int&, const int&, const int&,std::vector<int>&)> func;

   // part 1
   int first = 0;

   // part 2
   int last  = 0;
   std::vector<int> numbers;

   while(*ip < (int)input.size()){
      
      // chose instruction pointed to by ip
      std::vector<std::string> &line = input[*ip];

      // part 1
      if (*ip == 28 && first==0){ first = reg[5]; }
      
      // part 2
      if (*ip == 28){
         if (std::find(numbers.begin(), numbers.end(),reg[5])==numbers.end()){ numbers.push_back(reg[5]); }
         else {
            last = numbers.back();
            break;
         }         
      }


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

   std::cout << "Answer (part 1): " << first << std::endl;
   std::cout << "Answer (part 2): " << last  << std::endl;
}

#endif /* OPCODE_H */