#ifndef QUERY_H
#define QUERY_H
#include <QHash>
#include "model/match.h"
#include <QStringList>
#include "model/database.h"
#include <QVector>
#include <QPair>
#include <armadillo>
using namespace arma;
class Query
{
private:
   QMap<int,Match*> matches;
   QStringList keys;
   Database* db;
   int index;
   QList<Match*> result; //risultato aggiornato a ogni iterazione di query
   QList<Match*> resultafter; //risultato parziale dell'iterazione
   QList<QPair<QString, double> > totparole;
public:
    Query(const QString &q, Database* d, const int& ind=0);
    const QMap<int,Match*>& getMatches() const;
    void order();
    void orderafter();
    void exeQ();
    int getIndex() const;
    const QList<Match*>& getResult() const;
    void expand();
    void expandblind();
    void compare();
    void orderfinal();
    void hitsVector();
    void hitsMatrix();
    void matrixCit();
    void matrixWord();
    vec Hits();
};

#endif // QUERY_H
