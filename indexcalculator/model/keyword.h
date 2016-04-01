#ifndef KEYWORD_H
#define KEYWORD_H
#include <QString>
#include <QMap>
class Database;

class Keyword
{
    QString parola;
    QMap<double, double> frequenze;
    Database* db;
    int totf;
public:
   Keyword(const QString& word, Database* d);
   void insert(const double& freq, const double& doc);
   const QMap<double, double>* getFrequenze() const;
   int getTotf() const;
   void totfUp(const int& number);
};

#endif // KEYWORD_H
