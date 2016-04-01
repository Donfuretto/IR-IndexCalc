#include "documento.h"
#include "model/database.h"
#include "math.h"
#include <QVector>
#include <iostream>
#include <limits>
Documento::Documento(const int& id, const QString& titolo, const QString& autori, const QString& riferimenti, Database* d):docid(id), title(titolo), authors(autori), references(riferimenti), db(d), p(0)
{
    double a=1;
    double b=3204;
    prob.push_back(a/b);
    /*for (int i=0; i<100;++i){
        prob.push_back(0);
    }*/
}

QString Documento::getTitle() const{
return title;}

int Documento::quotes(const double& citante){
    return db->getDocumenti()[citante]->citations.contains(docid);
}


int Documento::isQuoted(const double& citato){
    //return citations.contains(citato);
        return 0;
}

int Documento::getDocId() const{
    return docid;
}

/*QString Documento::getWhere() const{
    return where;
}*/

QString Documento::getAuthors() const{
    return authors;
}

QString Documento::getReferences() const{
    return references;
}


 const QList<int>* Documento::getCitations() const{
    return (&citations);
}

void Documento::setWeight(const QString& word, const double& peso){
    weights[word]=peso;
}

const QMap<QString, double>* Documento::getWeights() const{
    return &weights;
}

const QMap<QString, Keyword*>* Documento::getKeys() const{
    return &keys;
}


bool Documento::checkTitle(const QString& word) const{
    QString reg(word);
    return title.contains(reg, Qt::CaseInsensitive);
}

double Documento::zipfIndex(const QString& word) const{
    double k=db->getMaxf();
    double r=k-db->getParole().value(word)->getFrequenze()->value(docid);
    double p=k/r;
    return p;
}

double Documento::venturelli(const double& zipf, const double& numDoc) const{
    return zipf/numDoc;
}

double Documento::ktIndex(const double& singleF, const double& totalF) const{
    //std::cout<<"singleF "<<singleF<<"totalF "<<totalF<<std::endl;
    /*if(log(singleF/totalF)<0.3){
        std::cout<<"too little"<<std::endl;
        return 0.3;
    }*/
    std::cout<<log(singleF/totalF)<<std::endl;
    return log(singleF/totalF);
}

bool Documento::contains(const QString& word) const{
    return weights.contains(word);
}

double weights_sort(const QPair<QString, double>& pa, const QPair<QString, double>& pb)
{
    return pa.second > pb.second;
}

void Documento::setMaxweights(){

   QMap<QString, double>::const_iterator it=weights.begin();
   for(;it!=weights.end();++it){
   maxweights.push_back(QPair<QString, double>(it.key(),it.value()));
   }
    std::sort(maxweights.begin(), maxweights.end(), weights_sort);
    if(maxweights.size()>20){
        QVector<QPair<QString, double> > v= maxweights.toVector();
        v.resize(20);

        maxweights=v.toList();
    }

}
const QList<QPair<QString, double> >* Documento::getMaxweights() const{
    return &maxweights;
}

void Documento::insertCitation(const int& cit){
    citations.push_back(cit);
}

double Documento::getP() const{
    return p;
}
void Documento::setP(const double& np){
    if(p+np>1)
        p+=np/std::numeric_limits<double>::max();
    else
        p+=np;
    p/=10;
}

const QVector<double>* Documento::getProb() const{
    return &prob;
}
void Documento::insertProb(const double& nprob){
    prob.push_back(nprob);
    std::cout<<prob[prob.size()-1]<<" nuovo valore di prob"<<std::endl;
}
