#include "database.h"
#include <QFile>
#include <QDomDocument>
#include <iostream>
#include <QStringList>
#include <QRegularExpression>
#include <QTextStream>
#include "math.h"
#include <cmath>
#include <limits>
#include <armadillo>
using namespace arma;

Database::Database(){}

void Database::load(){
    QFile file("docid.documento.xml");

    QDomDocument xml;
    if(!file.open(QIODevice::ReadOnly))
    {
        std::cout<<"Errore file documento.xml"<<std::endl;
    }
    xml.setContent(&file);
    QDomElement root=xml.documentElement();
    file.close();

    //Reading from the file
    QDomElement doc=root.firstChild().toElement();
    bool found=false;
    // Loop while there is a child
    while(!doc.isNull()&&!found)
    {
        // Check if the child tag name is utente
        if (doc.tagName()=="DOC")
        {
            // Get the first child of the component
            QDomElement child=doc.firstChild().toElement();
            double docId;
            QString title;
            QString where;
            QString authors;
            QString references;
            // Read each child of the utente node
            while (!child.isNull())
            {
                // Read Name and value
                if (child.tagName()=="DOCID") docId=child.firstChild().toText().data().toInt();
                // Next child
                if (child.tagName()=="TITLE"){
                    title=child.firstChild().toText().data().simplified();
                }
                if (child.tagName()=="WHERE"){
                    where=child.firstChild().toText().data().simplified();
                }
                if (child.tagName()=="AUTHORS"){
                    authors=child.firstChild().toText().data().simplified();
                }
                if (child.tagName()=="REFERENCES"){
                    references=child.firstChild().toText().data().simplified();
                }
                documenti[docId]=new Documento(docId,title,authors,references,this);
                child = child.nextSibling().toElement();
            }//end while !child.isNull()
        }//end if tag==doc
        // Next lemma
        doc = doc.nextSibling().toElement();
    }
    loadCitations();
    loadkeywords();
    pesi();
    loadPesiFromText();
    loadExpansionset();
    //std::cout<<"Page rank disattivato"<<std::endl;
    //pageRank();
    //std::cout<<"-caricamento eseguito-"<<std::endl;
}

void Database::loadCitations(){
    QFile file("citation.list.txt");
    std::cout<<"sono entrato nelle citazioni"<<std::endl;
    if(!file.open(QIODevice::ReadOnly)){
        std::cout<<"Errore Citazioni file"<<std::endl;}
    //... (open the file for reading, etc.)
    QTextStream textStream(&file);
    QString line = textStream.readLine();
    while (!line.isNull())
    {
        QStringList temp=line.split(QRegularExpression(" "));
        for(int i=2; i<temp.size();++i){
            documenti.value(temp[0].toInt())->insertCitation(temp[i].toInt());

        }
        if (documenti.value(temp[0].toInt())->getCitations()->size()==0){
            for(int i=0; i<3204;++i){
                documenti.value(temp[0].toInt())->insertCitation(0);
            }
        }
       /* QList<int>::const_iterator it=documenti.value(temp[0].toInt())->getCitations()->begin();
                for(; it!=documenti.value(temp[0].toInt())->getCitations()->end();++it){
            std::cout<<"citazione: "<<temp[0].toInt()<<" "<<(*it);
        }*/
        //std::cout<<"inserito "<<temp[2].toStdString()<<" in "<<temp[1].toInt()<<std::endl;

    line = textStream.readLine();
    }
    file.close();
}

void Database::loadkeywords(){
    int max=0;
    QFile textFile("freq.docid.stem.txt");
    if(!textFile.open(QIODevice::ReadOnly)){
        std::cout<<"errore loadKey"<<std::endl;}
    //... (open the file for reading, etc.)
    QTextStream textStream(&textFile);
    QString line = textStream.readLine();
    while (!line.isNull())
    {
        QStringList temp=line.split(QRegularExpression(" "));
        if (!parole.contains(temp[2]))
            parole[temp[2]]=new Keyword(temp[2], this);
        parole.value(temp[2])->insert(temp[0].toInt(),temp[1].toInt());
        parole.value(temp[2])->totfUp(temp[0].toInt());
        if (temp[0].toInt()>max)
            max=temp[0].toInt();
        if(documenti.contains(temp[1].toInt())){
            documenti[temp[1].toInt()]->setWeight(
                        temp[2],0);
            std::cout<<"inserito "<<temp[2].toStdString()<<" in "<<temp[1].toInt()<<std::endl;
        }
        line = textStream.readLine();
    }
    maxf=max+1;
    textFile.close();
    std::cout<<"Cit completate"<<std::endl;
}

void Database::loadPesiFromText(){//Inserimento matrice
    QFile file("risutati_pesatura_gruppo14.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        std::cout<<"Errore file pesatura"<<std::endl;
    }
    QMap<int, Documento*>::const_iterator it=documenti.begin();
    QTextStream textStream(&file);
    QString line = textStream.readLine();
    for(;it!=documenti.end() && !line.isNull();++it){
        QMap<QString, double>::const_iterator wit=it.value()->getWeights()->begin();
        for(; wit!=it.value()->getWeights()->end()&&(!line.isNull());++wit){
            QStringList list=line.split(QRegularExpression(" "));
            it.value()->setWeight(list[0],list[2].toDouble());
            std::cout<<"insert "<<line.toStdString()<<std::endl;
            line = textStream.readLine();
        }
    }
    it=documenti.begin();
    for(; it!=documenti.end();++it){
        it.value()->setMaxweights();

    }
    file.close();
}
const QMap<int, Documento*>& Database::getDocumenti() const{
    return documenti;
}

const QMap<QString, Keyword*>& Database::getParole() const{
    return parole;
}

int Database::getMaxf() const{
    return maxf;
}

void Database::pesi(){
    QMap<int, Documento*>::const_iterator it=documenti.begin();
    for(; it!=documenti.end();++it){

        QMap<QString, double>::const_iterator kit=it.value()->getWeights()->begin();
        for(; kit!=it.value()->getWeights()->end();++kit){

            double zipf=it.value()->zipfIndex(kit.key());
            double epsilon = std::numeric_limits<double>::min();
            if (std::abs(zipf) < epsilon) {
                zipf=0;
            }
            double ven=it.value()->venturelli(zipf, parole.value(kit.key())->getFrequenze()->size());
            if (std::abs(ven) < epsilon) {
                ven=0;
            }
            double kt=it.value()->ktIndex(3204, parole.value(kit.key())->getFrequenze()->size());
            double part;
            if (std::abs(kt) < epsilon) {
                kt=0;
                part=ven;
            }
            else
                part=ven*kt;
            if (std::abs(part) < epsilon) {
                part=0;
            }
            double parziale=part+it.value()->checkTitle(kit.key());
            QMap<double, double>::const_iterator dit=parole.value(kit.key())->getFrequenze()->begin();
            double count=0;
            for(; dit!=parole.value(kit.key())->getFrequenze()->end();++dit){
                if(documenti.value(dit.key())->contains(kit.key())&&documenti.value(dit.value())->quotes(it.key())){
                    ++count;
                }
            }
            double par;
            if (count==0)
                par=0;
            else
                par=log(count);
            if (std::abs(par) < epsilon) {
                par=0;
            }
            double result=parziale+par;
            it.value()->setWeight(kit.key(),result);
        }
    }
    it=documenti.begin();
    for(; it!=documenti.end();++it){
        it.value()->setMaxweights();
    }
}

void Database::printZipf() const{
    QFile file("risutati_pesatura_gruppo14.txt");
    if(!file.open(QIODevice::ReadWrite))
    {
        std::cout<<"Errore file pesatura"<<std::endl;
    }
    file.resize(0);
    QTextStream out(&file);
    QString s(QString(""));
    QMap<int, Documento*>::const_iterator dit=documenti.begin();
    for(;dit!=documenti.end();++dit)
    {
        QMap<QString, double>::const_iterator wit=dit.value()->getWeights()->begin();
        for(;wit!=dit.value()->getWeights()->end();++wit){
            s=s+wit.key()+" "+QString::number(dit.value()->getDocId())+" "+QString::number(wit.value())+"\n";
            std::cout<<wit.key().toStdString()<<" "<<dit.value()->getDocId()<<" "<<wit.value()<<std::endl;
        }
    }
    out<<s;
    file.close();
}

void Database::loadExpansionset(){
    QFile exfile("qrels-originale.txt");
    if(!exfile.open(QIODevice::ReadOnly))
    {
        std::cout<<"Errore file espansione"<<std::endl;
    }
    QTextStream textStream(&exfile);
    QString line=textStream.readLine();
    int i=0;
    std::cout<<"aperto"<<std::endl;
    while (!line.isNull())
    {
        QStringList temp=line.split(QRegularExpression(" "));
        if (!expansionsize.contains(temp.value(0).toInt())){
            expansionsize[temp.value(0).toInt()]=0;
        }
        expansionsize[temp.value(0).toInt()]+=expansionsize.value(temp.value(0).toInt())+1;
        expansionset.append(temp);
        std::clog<<"espansione "<<expansionset.value(i).value(0).toStdString()<<" "<<expansionset.value(i).value(1).toStdString()<<" "<<expansionset.value(i).value(2).toStdString()<<std::endl;
        line = textStream.readLine();
        ++i;
    }
    exfile.close();
}

const QList<QStringList> *Database::getExpansionset() const{
    return &expansionset;
}

void Database::setExpansionsize(const int x) {
    for(int i=0;i>=x;i++){
        expansionsize[i]=x;
    }
}

const QMap<int, int>* Database::getExpansionsize() const{
    return &expansionsize;
}

void Database::pageRank(){
    QFile file("pagerank.txt");
    if(!file.open(QIODevice::ReadWrite)){
        std::cout<<"Errore File pagerank"<<std::endl;
    }
    file.resize(0);
    QTextStream out(&file);
    QString s(QString(""));

    std::cout<<"Inizio conto Rank Page"<<std::endl;
    int numeroPassi=100;
    for (int t=1; t<numeroPassi;++t){
        //Stampo
        s=QString::number(t)+QString("\n");
        out<<s;
        std::cout<<t<<std::endl;
        //------
        QMap<int, Documento*>::const_iterator it=documenti.begin();
        double xd=(*it)->getCitations()->size();
        for(; it!=documenti.end();++it){
            if(it.value()->getCitations()->contains(0))
                it.value()->insertProb(0);
            else{
                for(int i=0; i!=(*it)->getCitations()->size();++i){
                    double epsilon = std::numeric_limits<double>::min();
                    double np=(*documenti.value((*(*it)->getCitations())[i])->getProb())[t-1];//np
                    if (np<epsilon){
                        np=0;
                        std::cout<<"epsilion alternaive";
                    }
                    (*it)->setP(np);

                    std::cout<<t<<" np "<<(*it)->getP()<<" "<<(*documenti.value((*(*it)->getCitations())[i])->getProb())[t-1]<<std::endl;
                    std::cout<<""<<epsilon<<"";
                }
                double xa=20, xb=0.005, xc=3204;
                (*it)->insertProb((((xa-xb)*(*it)->getP())+xb*(xa/xc))/xd);
            }
            s=QString::number((*it)->getDocId())+QString(" ")+QString::number((*it)->getProb()->value(t))+QString("\n");
            out<<s;
        }//for(; it!=documenti.end();++it)
    }//for (int t=1; t<3204;++t)
    //Matrice Correlazione
    /*//*----------------------------------------------------------------------------------
        QMap<int, Documento*>::iterator it=documenti.begin();
        int x=0;
        mat matCo(100,numeroPassi,fill::zeros);
        for(;(it!=documenti.end() && x<numeroPassi);++it){
           rowvec v=(*it)->getProb()->toStdVector ();
            matCo.insert_rows (x,v);
            cout <<x<<" "<<v<< endl;
            ++x;
        }
        for(int x=0;x<numeroPassi;x++){
            for(int y=0;y<100;y++){
                if(matCo(x,y)!=0)
                    matCo(x,y)=1;
            }
        }
        matCo.print ("Citmat: ");
        matCo.save("MatCit.txt",arma_ascii);
    //-----------------------------------------------------------------------------------
    */file.close();
}

