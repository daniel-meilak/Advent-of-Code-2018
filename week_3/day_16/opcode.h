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

void setr(const int &a, const int&, const int &c, std::vector<int> &reg){
   reg[c] = reg[a];
}

void seti(const int &a, const int&, const int &c, std::vector<int> &reg){
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

// vector of all opcode functions that take 3 inputs and change memory
std::vector<std::function<void(const int&, const int&, const int&,std::vector<int>&)>> functions =
   {&addr, &addi,
   &mulr, &muli,
   &banr, &bani,
   &borr, &bori,
   &setr, &seti,
   &gtir, &gtri, &gtrr,
   &eqir, &eqri, &eqrr,};

#endif /* OPCODE_H */