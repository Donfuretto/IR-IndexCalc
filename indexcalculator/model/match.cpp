#include "match.h"
#include <iostream>
#include "math.h"

Match::Match(const int& dc, Database* d): db(d), numero(0), indice(0.0), posizione(0)
{
doc=db->getDocumenti().value(dc);
}

Documento* Match::getDoc() const{
    return doc;
}
int Match::getNumero() const{
    return numero;
}

void Match::upNumero(){
    numero++;
}

void Match::upRNumero(double a){
    numero+a;
}

double Match::getIndice() const{
    return indice;
}

void Match::upIndice(const double &up){
   indice=indice+up;//AGGIORNO INDICE
   std::cout<<"nuovo indice "<<indice<<std::endl;
}

void Match::setPosizione(const int& pos){
    posizione=pos;
}

int Match::getPosizione() const{
    return posizione;
}

bool Match::operator==(const Match& m) const{
    return (numero==m.numero&&indice==m.indice);
}
bool Match::operator>(const Match& m) const{
    if(numero==m.numero)
        return indice>m.indice;
    else if(numero<m.numero)
        return false;

    else
        return true;
}
bool Match::operator<(const Match& m) const{
    return (!(*this>m)&&!(*this==m));
}

void Match::setIndice(const double& nind){
    indice=nind;
}
const QList<int>* Match::getCitations() const{
   return doc->getCitations ();
};
