#include "query.h"
#include <QRegularExpression>
#include <iostream>
#include <stdlib.h>     /* qsort */
#include "math.h"
#include <QFile>
#include <QTextStream>
#include "QVector"
#include "armadillo"
#include <vector>
using namespace arma;

Query::Query(const QString& q, Database* d, const int& ind):db(d), index(ind){
    keys=q.split(QRegularExpression(" "));
}

void Query::exeQ(){
    QStringList::const_iterator it=keys.begin ();
    for(;it!=keys.end();++it){
        QMap<int,Documento*>::const_iterator dit=db->getDocumenti().begin ();
        for(;dit!=db->getDocumenti().end();++dit){
            if(dit.value()->contains(*it)){
                if(!matches.contains (dit.value()->getDocId ()))
                    matches[dit.value()->getDocId()]=new Match(dit.value()->getDocId(), db);
                /*QVector<double>::const_iterator zz=dit.value()->getProb()->begin();
                int i=0;
                for(;zz!=dit.value()->getProb()->end();++zz){
                    std::cout<<"valore prob "<<i<<" "<<*zz<<std::endl;
                    ++i;
                }*/
                std::cout<<"il valore che aspettiamo è "<<dit.value()->getWeights()->value(*it)<<"*"<<(dit.value()->getProb()->value(dit.value()->getProb()->size()-1))<<std::endl;
                matches[dit.value ()->getDocId()]->upIndice(dit.value()->getWeights()->value(*it)*(dit.value()->getProb()->value(dit.value()->getProb()->length()-1)));
                matches[dit.value ()->getDocId()]->upNumero();
            }
        }
    }
    order();
    std::cout<<"inizio espansione"<<std::endl;
    //db->setExpansionsize (50);
    //expandblind();
    //unorder();
    //expand();
    //hitsVector();
    //matrixCit();
    //matrixWord();
    vec a(100);
    //a=Hits();

    std::cout<<"fine espansione"<<std::endl;

    it=keys.begin ();
    if(!a.is_empty ()){
        int i=0;
        for(;it!=keys.end();++it){
            QMap<int,Documento*>::const_iterator dit=db->getDocumenti().begin ();
            for(;dit!=db->getDocumenti().end();++dit,i++){
                if(i<99)
                    cout<<a(i);
                if(dit.value()->contains(*it)){
                    if(!matches.contains (dit.value()->getDocId ()))
                        matches[dit.value()->getDocId()]=new Match(dit.value()->getDocId(), db);
                    if(db->getExpansionsize()->value(index)){
                    matches[dit.value ()->getDocId()]->upIndice(log((dit.value ()->getWeights()->value(*it)*(dit.value()->getProb()->value(dit.value()->getProb()->length()-1)))/(db->getExpansionsize()->value(index))));
                    if(i<99)
                     matches[dit.value ()->getDocId()]->upIndice(a(i));
                    }
                }
                /*  QList<Match*>::const_iterator dit=result.begin ();
            for(;dit!=result.end();++dit){
                if((*dit)->getDoc()->contains(*it)){
                    if(!matches.contains ((*dit)->getDoc()->getDocId()))
                        matches[(*dit)->getDoc()->getDocId()]=new Match((*dit)->getDoc()->getDocId(),db);
                    matches[(*dit)->getDoc()->getDocId()]->upIndice((*dit)->getDoc()->getWeights()->value(*it));
                    matches[(*dit)->getDoc()->getDocId()]->upNumero();
                }*/
            }
        }
    }
    else{
        for(;it!=keys.end();++it){
            QMap<int,Documento*>::const_iterator dit=db->getDocumenti().begin ();
            for(;dit!=db->getDocumenti().end();++dit){
                if(dit.value()->contains(*it)){
                    if(!matches.contains (dit.value()->getDocId ()))
                        matches[dit.value()->getDocId()]=new Match(dit.value()->getDocId(), db);
                    if(db->getExpansionsize()->value(index)){
                    matches[dit.value ()->getDocId()]->upIndice(log((dit.value ()->getWeights()->value(*it)*(dit.value()->getProb()->value(dit.value()->getProb()->length()-1)))/(db->getExpansionsize()->value(index))));
                    }
                }
                /*  QList<Match*>::const_iterator dit=result.begin ();
            for(;dit!=result.end();++dit){
                if((*dit)->getDoc()->contains(*it)){
                    if(!matches.contains ((*dit)->getDoc()->getDocId()))
                        matches[(*dit)->getDoc()->getDocId()]=new Match((*dit)->getDoc()->getDocId(),db);
                    matches[(*dit)->getDoc()->getDocId()]->upIndice((*dit)->getDoc()->getWeights()->value(*it));
                    matches[(*dit)->getDoc()->getDocId()]->upNumero();
                }*/
            }
        }
    }

    /*matches.clear();
    it=keys.begin();
    for(;it!=keys.end();++it){
        std::cout<<"ok"<<std::endl;
        std::cout<<"key "<<(*it).toStdString()<<std::endl;
        std::cout<<"ko"<<std::endl;
        QMap<double, double>::const_iterator fit=db->getParole().value(*it)->getFrequenze()->begin();
        for(;fit!=db->getParole().value(*it)->getFrequenze()->end();++fit){
            std::cout<<"chiave "<<fit.key()<<std::endl;
            if(!matches.contains(fit.key()))
                matches[fit.key()]=new Match(fit.key(), db);
            matches[fit.key()]->upIndice(db->getDocumenti().value(fit.key())->getWeights()->value(*it)/keys.size());
            matches[fit.key()]->upNumero();
        }

    }*/

    orderafter();
    compare();
}

int value_sort(Match* pa, Match* pb)
{
    return *pa > *pb;
}

double weights_sort(const QPair<QString, double>& pa, const QPair<QString, double>& pb);

void Query::expand(){
    QMap<QString, double>temp;

    QList<QStringList>::const_iterator dit=db->getExpansionset()->begin();
    bool get1=false;

    for(;dit!=db->getExpansionset()->end()&&!get1;++dit){
        if((*dit).value(0).toInt()==index){
            QMap<QString, double>::const_iterator w=db->getDocumenti().value((*dit).value(2).toInt())->getWeights()->begin();
            for(; w!=db->getDocumenti().value((*dit).value(2).toInt())->getWeights()->end();++w){
                std::cout<<"Weights: "<<(*dit).value(2).toStdString()<<" = "<<w.key().toStdString()<<" "<<w.value()<<std::endl;
                QMap<QString, double>::iterator temit=temp.begin();
                bool trovato=false;
                for(;temit!= temp.end()||!trovato;++temit){
                    if (temit.key()==w.key()){
                        (temp)[w.key()]+=w.value();
                        trovato=true;
                    }
                }
                if(!temp.contains(w.key()))
                    temp[w.key()]=w.value();
            }

            QMap<QString, double>::iterator temit=temp.begin();
            for(;temit!=temp.end() ;++temit){
                if(!keys.contains(temit.key()))
                    keys.append(temit.key());
            }

            QList<QPair<QString, double> >::const_iterator tp=totparole.begin();
            for(;tp!=totparole.end();++tp){
                if(!keys.contains((*tp).first))
                    keys.push_back((*tp).first);
            }
        }
        else if((*dit).value(0).toInt()>index)
            get1=true;
    }
}

void Query::expandblind(){

    QList<QPair<QString, double> >temp;
    QList<Match*>::const_iterator dit=result.begin();

    for(;dit!=result.end();++dit){
        QList<QPair<QString, double> >t=*(*dit)->getDoc()->getMaxweights();
        if((*dit)->getPosizione()>1){
            QList<QPair<QString, double> >::iterator pairit=t.begin();
            for(;pairit!=t.end();++pairit){
                (*pairit).second=((*pairit).second/(log((*dit)->getPosizione())/2));
            }
        }
        QList<QPair<QString, double> >::const_iterator mw=(*dit)->getDoc()->getMaxweights()->begin();
        for(; mw!=(*dit)->getDoc()->getMaxweights()->end();++mw){
            QList<QPair<QString, double> >::iterator temit=temp.begin();
            bool trovato=false;
            for(;temit!= temp.end() &&!trovato;++temit){
                int i=0;
                if ((*temit).first==(*mw).first)
                {
                    if((*mw).second>(*temit).second)
                        (temp).replace(i, *mw);
                    trovato=true;
                }
                ++i;
            }
            if(!trovato)
                temp.push_back(*mw);
        }
        std::sort(temp.begin(), temp.end(), weights_sort);
        QList<QPair<QString, double> >::const_iterator tp=totparole.begin();
        for(;tp!=totparole.end();++tp){
            if(!keys.contains((*tp).first))
                keys.push_back((*tp).first);
        }
    }
}

const QMap<int, Match*>& Query::getMatches() const{
    return matches;
}

void Query::order(){
    result = matches.values();
    std::sort(result.begin(), result.end(), value_sort);
    QVector<Match*> v=result.toVector();
    if(v.size()>50)
        v.resize(50);
    result=v.toList();
    QList<Match*>::const_iterator lit=result.begin();
    int i=1;
    for(; lit!=result.end();++lit){
        (*lit)->setPosizione(i);
        ++i;
    }
}

void Query::orderafter(){
    resultafter = matches.values();
    std::sort(resultafter.begin(), resultafter.end(), value_sort);
    QVector<Match*> v=resultafter.toVector();
    if(v.size()>50)
        v.resize(50);
    resultafter=v.toList();
    QList<Match*>::const_iterator lit=resultafter.begin();
    int i=1;
    for(; lit!=resultafter.end();++lit){
        (*lit)->setPosizione(i);
        ++i;
    }
}

void Query::orderfinal(){
    result = matches.values();
    std::sort(result.begin(), result.end(), value_sort);
    QVector<Match*> v=result.toVector();
    if(v.size()>50)
        v.resize(50);
    result=v.toList();
    QList<Match*>::const_iterator lit=result.begin();
    int i=1;
    for(; lit!=result.end();++lit){
        (*lit)->setPosizione(i);
        ++i;
    }
}

int Query::getIndex() const{
    return index;
}

const QList<Match*>& Query::getResult() const{
    return result;
}

void Query::compare(){
    QList<Match*> temp;
    QList<Match*>::const_iterator it=resultafter.begin();
    for(;it!=resultafter.end();++it){
        bool trovato=false;
        QList<Match*>::iterator rit=result.begin();
        int i=0;
        (*it)->setIndice((*it)->getIndice()/50);
        for(;rit!=result.end()&&!trovato;++rit){
            if((*rit)->getDoc()->getDocId()==(*it)->getDoc()->getDocId()){
                (*rit)->upIndice((*it)->getIndice());
                //(*rit)->upIndice(2);
                trovato=true;
            }
        }
        if(!trovato)
            temp.push_back(*it);
        ++i;
    }
    result.append(temp);
    orderfinal();
}

void Query::hitsVector(){
    QList<Match*>::const_iterator wit=resultafter.begin();
    int x=0;
    vec bestVec(50);
    vec::iterator i= bestVec.begin();
    for(;i<bestVec.end()&&wit<resultafter.end ();++i,++wit)
      {
       *i=(*wit)->getDoc()->getDocId ();
       cout <<x<<" "<< *i << endl;
        ++x;
       }
    bestVec.print ("bestVec: ");
    cout<<bestVec;
    bestVec.save("bestVec.txt",arma_ascii);
}

void Query::matrixCit(){
    //QMap<int, Documento*>::iterator it=documenti.begin();
    QList<Match*>::const_iterator it=resultafter.begin();
    QList<Match*>::const_iterator cit=resultafter.begin();
    mat matCo(100,100,fill::zeros);
    int trovata=0;//numero citazioni;
    for(int x=0;x<100&&it!=resultafter.end();x++,++it){
        cit=resultafter.begin();
        trovata=0;
        for(int y=0;y<100;y++){
            if((*cit)->getDoc()->getCitations()->back()!=0)
                {matCo(x,y)=1.0f; trovata++;}
       }
    }
    //matCo.print ("Citmat: ");
    //cout<<matCo;
    matCo.save ("Citazmatrix.txt",arma_ascii);
}

void Query::matrixWord(){
    //QMap<int, Documento*>::iterator it=documenti.begin();
    int max=0;
    QList<QString> word;
    QFile textFile("bestWord.txt");
    if(!textFile.open(QIODevice::ReadOnly)){
        std::cout<<"errore loadKey"<<std::endl;}
    //... (open the file for reading, etc.)
    QTextStream textStream(&textFile);
    QString line = textStream.readLine();
    while (!line.isNull())
    {
        QStringList temp=line.split(QRegularExpression(" "));
        word.append (temp[0]);
        std::cout<<"PAROLA"<<word.last ().toStdString ();
        line = textStream.readLine();
    }
    textFile.close();
    std::cout<<"Word Caricate"<<std::endl;
    std::cout<<"Creo Matrice"<<std::endl;

    QList<Match*>::const_iterator it=resultafter.begin ();
    QMap<QString,Keyword*>::const_iterator dp=db->getParole ().begin ();
    QList<QString>::iterator pr=word.begin ();

    mat matCo(100,50,fill::zeros);
    for(int x=0;x<100&&it!=resultafter.end();x++,++it){
        for(int y=0;y<50;y++,pr++){
                if(dp.value ()->getFrequenze ()->contains (x))
                {matCo(x,y)=1.0f;}
       }
        pr=word.begin();
    }
    //matCo.print ("Citmat: ");
    //cout<<matCo;
    matCo.save ("WordDoc.txt",arma_ascii);
}

vec Query::Hits(){
 mat Ai(100,100,fill::ones);
 mat Hi(100,100,fill::ones);
 mat Cit(100,100);
 int temposcelto=50;
 Cit.load ("Citazmatrix.txt",arma_ascii);

 int t;
 for(int i=0;i<temposcelto;i++){
    t=0;
    Ai(i,t)=Hi(i,t)=1/14;
    bool converge=false;
    while (!converge&&t<temposcelto){
        t++;
        //autorevolezza
        for(int k=0;k<100;k++){
            for(int j=0;j<100;j++){
                if(Cit(k,j))//se k link j, guardo in matrice
                    Ai(k,t)=Ai(k,t-1)+Hi(j,t-1);
            }
        }
        //centralità
        for(int k=0;k<100;k++){
            for(int j=0;j<100;j++){
                if(Cit(j,k))//se j link k, guardo in matrice
                    Ai(j,t)=Ai(j,t-1)+Hi(k,t-1);
            }
        }

    double x=0.014;//limite Convergenza
    if((Ai(i,t)-Ai(i,t-1))<x && Hi(i,t)-Hi(i,t-1)<x)
        converge=true;
    }
 }
    vec Potenziamento(100);
   return Potenziamento=Ai.col(temposcelto);
//    Potenziamento.save("test1.txt",arma_ascii);
//    Hi.save("test2.txt",arma_ascii);

//    QList<Match*>::const_iterator lit=result.begin();
//    int i=1;
//    for(; lit!=result.end() && i<100;++lit){
//        (*lit)->upRNumero(Potenziamento(i));
//        ++i;
//  }
}

