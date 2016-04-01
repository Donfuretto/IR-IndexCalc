#include "mainwindow.h"
#include <QApplication>
#include "model/database.h"
#include <iostream>
#include "model/query.h"
#include "model/querylist.h"
#include <vector>
#include "model/match.h"
#include "cblas.h"
#include "armadillo"

using namespace arma;

int main(int argc, char *argv[])
{
//    /*Database* db=new Database();
//    db->load();
//    db->printZipf();*/
querylist* ql=new querylist();
ql->printQueries();

//    //cout << "Armadillo version: " << arma_version::as_string() << endl;
//        //(RIGHE,COLONNE)
//     //dichiarazione matrice
//    mat A(2,2);
//     A<<2<<2<<endr
//         <<2<<2<<endr;
//     cx_vec eigval;
//     cx_mat eigvec;
//     vec v(5);
//    //-------------------------------------------------------------------
//cout<<"A: "<<A<<"A*A "<<A*A<<endl
//eig_gen(eigval, eigvec, A);
//cout<<"Valore: "<<eig_sym(A);
}
