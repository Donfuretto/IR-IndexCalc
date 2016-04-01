#include "querylist.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <iostream>
#include <string>
#include <QTime>
#include <armadillo>
using namespace arma;

querylist::querylist()
{
    db=new Database();
    db->load();
    int i=1;
    QFile file("query-keyword.txt");
    if(!file.open(QIODevice::ReadOnly))
        std::cout<<"non hai aperto"<<std::endl;
    QTextStream textStream(&file);
    QString line = textStream.readLine();
    QString s=QString("");
    while (!line.isNull()){
        QStringList l=line.split(QRegularExpression("\t"));
        if(l[0]==QString::number(i)){
            s=s+l[1]+" ";
        }
        else{
            vectorq.push_back (new Query(s, db, i));
        std::cout<<"query "<<l[0].toStdString()<<" inserita "<<s.toStdString()<<std::endl;
        s=QString("");
        ++i;
        }
        line=textStream.readLine();
    }
       line = textStream.readLine();
 }


const QVector<Query*>& querylist::getVector() const{
    return vectorq;
}

querylist::~querylist(){}

void querylist::printQueries() const{
    QFile file("G14R7.txt");
    if(!file.open(QIODevice::ReadWrite)){
        std::cout<<"Errore file risultato"<<std::endl;
    }
    file.resize(0);
    QTextStream out(&file);
    QString s(QString(""));
    QVector<Query*>::const_iterator qit=vectorq.begin();
    int i=1;
    QTime* time=new QTime();
    time->start();
    for(;qit!=vectorq.end();++qit)
    {
        (*qit)->exeQ();
        QList<Match*>::const_iterator wit=(*qit)->getResult().begin();
        for(;wit!=(*qit)->getResult().end()/* && (*wit)->getResult()<80*/;++wit){
            s=s+QString::number(i)+QString(" Q0 ")+QString::number((*wit)->getDoc()->getDocId())+QString(" ")+QString::number((*wit)->getPosizione())+QString(" ")+QString::number(((*wit)->getIndice()+((*wit)->getNumero()*10)))+QString(" G14R7")+QString("\n");
            //std::cout<<i<<" Q0 "<<(*wit)->getDoc()->getDocId()<<" "<<(*wit)->getPosizione()<<" "<<((*wit)->getIndice()+((*wit)->getNumero()*10))<<" "<<"G14R5"<<std::endl;
        }
        ++i;
    }
    std::cout<<"timer "<<time->restart()<<std::endl;
    out<<s;
    file.close();
}



