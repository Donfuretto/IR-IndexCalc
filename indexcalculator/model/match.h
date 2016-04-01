#ifndef MATCH_H
#define MATCH_H
#include "model/documento.h"
#include "model/keyword.h"
#include "model/database.h"
class Match
{
private:
    Documento* doc;
    int numero;
    Database* db;
    double indice;
    int posizione;
public:
    Match(const int &dc, Database* d);
    Documento* getDoc() const;
    int getNumero() const;
    void upNumero();
    void upIndice(const double &up);
    double getIndice() const;
    void setPosizione(const int& pos);
    int getPosizione() const;
    bool operator==(const Match& m) const;
    bool operator>(const Match& m) const;
    bool operator<(const Match& m) const;
    void setIndice(const double& nind);
    const QList<int>* getCitations() const;
    void upRNumero(const double a);
};

#endif // MATCH_H
