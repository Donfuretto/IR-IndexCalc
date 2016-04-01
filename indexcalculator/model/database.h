#ifndef DATABASE_H
#define DATABASE_H
#include <QMap>
#include <QString>
#include "model/documento.h"
#include "model/keyword.h"
#include <QMap>
#include <QList>
#include <QStringList>
class Database
{
    QMap<QString, Keyword*> parole;
    QMap<int, Documento*> documenti;
    int maxf;
    QList<QStringList> expansionset;
    QMap<int, int> expansionsize;
public:
    Database();
    void load();
    const QMap<QString, Keyword*>& getParole() const;
    const QMap<int, Documento*>& getDocumenti() const;
    void loadkeywords();
    void loadCit();//Carica Citazioni file.
    int getMaxf() const;
    void pesi();
    void printZipf() const;
    void loadPesiFromText();
    const QList<QStringList>* getExpansionset() const;
    void loadExpansionset();
    const QMap<int, int>* getExpansionsize() const;
    void setExpansionsize(const int);
    void loadCitations();
    void pageRank();

};

#endif // DATABASE_H
