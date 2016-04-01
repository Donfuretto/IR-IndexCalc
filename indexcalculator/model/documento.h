#ifndef DOCUMENTO_H
#define DOCUMENTO_H
#include <QMap>
#include "model/keyword.h"
#include <QString>
#include <QVector>
class Database;
class Documento
{
    Database* db;
    QMap<QString, Keyword*> keys;
    int docid;
    QString title;
    QString authors;
    QString references;
    QMap<QString, double> weights;
    QList<QPair<QString, double> > maxweights;
    double p;
    QVector<double> prob;
public:
    QList<int> citations;//Lascio public per Matrice cit
    Documento(const int& docId,const QString& titolo, const QString& autori, const QString& riferimenti, Database* d);
    QString getTitle() const;
    int quotes(const double& citante);
    int isQuoted(const double& citato);
    int getDocId() const;
    //QString getWhere() const;
    QString getAuthors() const;
    QString getReferences() const;
    const QList<int>* getCitations() const;
    void setWeight(const QString& word, const double& peso);
    const QMap<QString, double>* getWeights() const;
    const QMap<QString, Keyword*>* getKeys() const;
    bool checkTitle(const QString& word) const;
    double zipfIndex(const QString& word) const;
    double venturelli(const double& zipf, const double& numDoc) const;
    double ktIndex(const double& singleF, const double& totalF) const;
    bool contains(const QString& word) const;
    const QList<QPair<QString, double> >* getMaxweights() const;
    void setMaxweights();
    void insertCitation(const int& cit);
    double getP() const;
    const QVector<double>* getProb() const;
    void insertProb(const double& nprob);
    void setP(const double& np);
};

#endif // DOCUMENTO_H
