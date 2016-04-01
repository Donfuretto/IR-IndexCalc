#ifndef QUERYLIST_H
#define QUERYLIST_H
#include <vector>
#include <QVector>
#include "model/query.h"
#include "model/database.h"
class querylist
{
    QVector<Query*> vectorq;

public:
    Database* db;
    querylist();
    ~querylist();
    const QVector<Query*>& getVector() const;
    void printQueries() const;
};

#endif // QUERYLIST_H
