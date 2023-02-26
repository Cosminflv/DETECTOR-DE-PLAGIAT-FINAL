#include "Porter.h"

using namespace std;

int main()
{
    int n;
    vector <int> vector_dimensiuni;
    vector <cuvantt> hash_table1;
    vector <vector <cuvantt>> hash_table;

    int dim1, nr_cuv, nr_semne, nr_comparatii = 0;
    float plagiat;
    vector<string> a;
    string s;
    float suma = 0;

    string numedoc;

    cout << "Introduceti numarul de documente:";
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        dim1 = 0;
        cout << "Introduceti numele documentului:";
            cin >> numedoc;
        ifstream f(numedoc);
        std::getline(f, s);
        tokenizare(a, s, dim1);
        for (int j=0;j<dim1;j++)
          step1c(a[j], a[j].length() - 1); 
        initializare_hash_table(hash_table1);
        memorare_in_hash_table(a, hash_table1, dim1);
        vector_dimensiuni.push_back(dim1);
        hash_table.push_back(hash_table1);
        hash_table1.clear();
        a.clear();
    }
  
    for(int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
        {
            nr_cuv = verificare_documente(hash_table[i], hash_table[j], i, j);
            plagiat = (float)(2 * nr_cuv) / (vector_dimensiuni[i] + vector_dimensiuni[j]) * 100;
            suma += plagiat;
            nr_comparatii++;
        }
    suma = (float)suma / nr_comparatii;
    cout << fixed << setprecision(2) << suma << "%";
}