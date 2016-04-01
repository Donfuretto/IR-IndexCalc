#include "keyword.h"
#include <iostream>
#include "Model/database.h"
Keyword::Keyword(const QString& word, Database* d):parola(word), db(d), totf(0)
{}

void Keyword::insert(const double &freq, const double &doc){
    frequenze[doc]=freq;
    std::cout<<"FREQUENZA-----------"<<doc<<" "<<freq<<std::endl;
}

const QMap<double, double>* Keyword::getFrequenze() const{
    return &frequenze;
}

int Keyword::getTotf() const{
    return totf;
}

void Keyword::totfUp(const int& number){
    totf+=number;
}
