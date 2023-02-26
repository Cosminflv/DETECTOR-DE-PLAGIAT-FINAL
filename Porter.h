#pragma once
#include <iostream> // std::cin, std::cout
#include <fstream> // std::ifstream
#include<string.h>
#include <string>
#include<stdlib.h>
#include<ctype.h>
#include <stdio.h>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;



struct cuvantt
{
    string denumire;
    int cod;
};

int cons(string a, int i);

int m(string a, int j, int k);

int vowelinstem(string a, int j);

int doublec(string a, int j);

int cvc(int i, string a, int j);

void setto(int len_inc, string add, string a, int init_length, int suffix_length);

bool ends(string a, string s, int length, int j);

void step1ab(string a, int j);

void step1c(string& a, int j);

void step2(string a, int j);

void step3(string a, int j);

void step4(string a, int j);

void step5(string a, int j);

int stem(string a, int i, int j);

void eliminareCaractere(string& s, int& nr_semne);

void eliminareSemneUltimulCuvant(vector<string>& cuvinte, int nr_semne);

void Stopword_Removal(vector<string>& cuvinte);

void tokenizare(vector<string>& a, string& s, int& dima);

int hashing(string cuv);

void initializare_hash_table(vector <cuvantt>& hash_table);

void inserare_tabel(string cuvant_de_inserat, vector <cuvantt>& hash_table);

void memorare_in_hash_table(vector<string> a, vector <cuvantt>& hash_table, int dima);

int verificare_documente(vector <cuvantt> hash_table1, vector <cuvantt> hash_table2, int i, int j);