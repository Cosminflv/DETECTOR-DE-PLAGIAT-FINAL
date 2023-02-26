#include "Porter.h"

static int k0, k;
static int parameter;
int TABLE_SIZE = 10000;

/*Function to check individually for consonants and vowels.
Vowels here are: a,e,i,o,u and y preceded by a consonant.
k0 here is the first element.
*/



int cons(string a, int i)
{
	switch (a[i])
	{
	case 'a': case 'e': case 'i': case 'o': case 'u': return 0;
	case 'y': {if (i == k0) return 1; else !cons(a, i - 1); }
	default: return 1;
	}
}


/*
If C- consonant sequence (c,cc,ccc,ccc,..)
V- vowel sequence (v, vv, vvv,...)
This function m() gives the count of (VC) formed.
Example
m=0 TR, EE, TREE, Y, BY.
m=1 TROUBLE, OATS, TREES, IVY.
m=2 TROUBLES, PRIVATE, OATEN, ORRERY.
*/

int m(string a, int j, int k)
{
	int n = 0;
	int r = k0;
	int flag;
	int c = 0;
	int v = 0;
	int value1 = 0;
	int value2 = 0;
	int value = 0;
	int loop = 0;
	while (1)
	{

		while (r <= k)
		{
			{

				if (r > j) break;
				if (!cons(a, r)) { c++; value1 = 1; break; } /*checks for vowel*/
				r++;
			}
			if (value1 == 1) break;
		}
		value1 = 0;
		r++;


		while (r <= k)
		{
			{

				if (r > j) break;
				if (cons(a, r)) { v++; value2 = 1; break; } /*checks for consonant*/
				r++;
			}
			if (value2 == 1) break;
		}
		value2 = 0;
		r++;
		if ((c == v) && (r <= k + 1))
		{

			value = value + 1; c = 0; v = 0; /*counts the number of VC sequences*/
		}
		if (r > k) break;
	}

	return value; /*returns the value of VC count found out*/

}

/*checks in a word, if it has a vowel*/

int vowelinstem(string a, int j)
{

	int last_w = j;
	int i;
	for (i = k0; i <= last_w; i++) if (!cons(a, i)) return 1;
	return 0;
}


/*checks for double consonants. Checks for the last position and the last but one-th position. Of the form say -ss */

int doublec(string a, int j)
{
	int last_w = j;
	if (j < k0 + 1) return 0;
	if (a[j] != a[j - 1]) return 0;
	return cons(a, j);
}

/*Checks for the form CVC*/

int cvc(int i, string a, int j)
{
	if (i <= k0 + 2)
	{
		if (!cons(a, i) || cons(a, i - 1) || !cons(a, i - 2))
		{
			int ch = a[i];
			if (ch == 'w' || ch == 'x' || ch == 'y') return 0; /* In CVC, the last C should not be w,x,y as per the algo.*/
		}
	}
	return 1;
}


/* Readjusts the string as per requirement. Adds new suffixes or changes to different forms as required.*/

void setto(int len_inc, string add, string a, int init_length, int suffix_length)
{
	for (int i = 0; i < len_inc; i++)
		a.push_back(48);
	for (int i = init_length + len_inc; i > init_length-suffix_length; i--)
		a[i] = a[i - len_inc];
	for (int i = 0; i < len_inc; i++)
		a[init_length + 1 - suffix_length + i] = add[i];
}

/*This part matches the suffix passed and checks if the suffix is present in the word*/

bool ends(string a, string s, int length, int j)
{
	for (int i = 0; i < length; i++)
		if (j + 1 - length + i>=0)
			if (a[j + 1 - length + i] != s[i])
				return 0;
	return 1;
}


/* step1ab() gets rid of plurals and -ed or -ing. e.g.

caresses -> caress
ponies -> poni
ties -> ti
caress -> caress
cats -> cat

feed -> feed
agreed -> agree
disabled -> disable

matting -> mat
mating -> mate
meeting -> meet
milling -> mill
messing -> mess

meetings -> meet

*/


void step1ab(string a, int j)
{ //cout<<"Enter 1AB";
	int last_w = j;
	int h = 0;
	if (a[k] == 's')
	{
		if (ends(a, "sses", 4, last_w) == 1) { k -= 2; } // devine ss
		else
			if (ends(a, "ies", 3, last_w) == 1) { k -= 3; setto(1, "i", a, last_w, last_w-k); } //se sterge es
			else
				if (a[k - 1] != 's') { k--; return; } //se sterge un singur s
	}
	else
		if (ends(a, "eed", 3, last_w) == 1) { if (m(a, last_w, k) > 1)k--; } //daca se termina in eed si in stem avem minim doua grupuri VC VC stergem d agreed -> agree
		else
			if ((ends(a, "ed", 2, last_w) == 1) || (ends(a, "ing", 3, last_w) == 1))
			{
				if ((ends(a, "ed", 2, last_w) == 1) && vowelinstem(a, last_w)) { k -= 2; }
				if ((ends(a, "ing", 3, last_w) == 1) && vowelinstem(a, last_w)) { k -= 3; }
				j = k;

				if (ends(a, "at", 2, j) == 1) { k -= 2; setto(3, "ate", a, last_w, last_w - k); }
				else
					if (ends(a, "bl", 2, j) == 1) { k -= 2; setto(3, "ble", a, last_w, last_w - k); }
					else
						if (ends(a, "iz", 2, j) == 1) { k -= 2; setto(3, "ize", a, last_w, last_w - k); }
						else
							if (doublec(a, k)) { k--; {int ch = a[k]; if (ch == 'l' || ch == 's' || ch == 'z') k++; } }
			}
	if ((ends(a, "at", 2, j) == 1) && (ends(a, "bl", 2, j) == 1) && (ends(a, "iz", 2, j) == 1))
	{
		if (m(a, j, k) == 1 && cvc(k, a, 0)) { parameter = 1; setto(1, "e", a, last_w, last_w-k); }
	}
	return;
}

/*turns terminal y to i when there is another vowel in the stem. */

void step1c(string& a, int j) {
	//cout<<"Enter 1C";
	//cout<<k;

	if (ends(a, "y", 1, j) && vowelinstem(a, j))
		a[j] = 'i';
}


/* step2() maps double suffices to single ones. so -ization ( = -ize plus
-ation) maps to -ize etc. note that the string before the suffix must give
m() > 0. */

void step2(string a, int j) {

	if ((m(a, j, k) > 0)) {
		switch (a[k - 1])
		{

		case 'a': if (ends(a, "ational", 7, k)) { k -= 7; parameter = 1; cout << "The value is " << " " << k; setto(3, "ate", a, j, j - k); break; }
				if (ends(a, "tional", 6, k)) { k -= 6; setto(4, "tion", a, j, j - k); break; }
				break;
		case 'c': if (ends(a, "enci", 4, k)) { k -= 4; parameter = 1; setto(4, "ence", a, j, j - k); break; }
				else
			if (ends(a, "anci", 4, k)) { k -= 4; parameter = 1; setto(4, "ance", a, j, j - k); break; }
				break;
		case 'e': if (ends(a, "izer", 4, k)) { {k -= 4; parameter = 1; setto(3, "ize", a, j, j - k); break; } }
				break;
		case 'l': if (ends(a, "bli", 3, k)) { k -= 3; parameter = 1; setto(3, "ble", a, j, j - k); break; }
				if (ends(a, "eli", 3, k)) { k -= 3; setto(1, "e", a, j, j - k); break; }
				if (ends(a, "alli", 4, k)) { k -= 4; parameter = 1; setto(2, "al", a, j, j - k); break; }
				if (ends(a, "entli", 5, k)) { k -= 5; parameter = 1; setto(3, "ent", a, j, j - k); break; }
				if (ends(a, "ousli", 5, k)) { k -= 5; parameter = 1; setto(3, "ous", a, j, j - k); break; }
				break;
		case 'o': if (ends(a, "ator", 4, k)) { k -= 4; parameter = 1; setto(3, "ate", a, j, j - k); break; }
				if (ends(a, "ization", 7, k)) { k -= 7; parameter = 1; setto(3, "ize", a, j, j - k); break; }
				if (ends(a, "ation", 5, k)) { k -= 5; parameter = 1; setto(3, "ate", a, j, j - k); break; }
				break;
		case 's': if (ends(a, "alism", 5, k)) { k -= 5; parameter = 1; setto(2, "al", a, j, j - k); break; }
				if (ends(a, "iveness", 7, k)) { k -= 7; parameter = 1; setto(3, "ive", a, j, j - k); break; }
				if (ends(a, "fulness", 7, k)) { k -= 7; parameter = 1; setto(3, "ful", a, j, j - k); break; }
				if (ends(a, "ousness", 7, k)) { k -= 7; parameter = 1; setto(3, "ous", a, j, j - k); break; }
				break;
		case 't': if (ends(a, "aliti", 5, k)) { k -= 5; parameter = 1; setto(2, "al", a, j, j - k); break; }
				if (ends(a, "iviti", 5, k)) { k -= 5; parameter = 1; setto(3, "ive", a, j, j - k); break; }
				if (ends(a, "biliti", 6, k)) { k -= 6; parameter = 1; setto(3, "ble", a, j, j - k); break; }
				break;
		case 'g': if (ends(a, "logi", 4, k)) { k -= 4; parameter = 1; setto(3, "log", a, j, j - k); break; }

		}
	}
}

/* step3() deals with -ic-, -full, -ness etc. similar strategy to step2. */

void step3(string a, int j) {
	if (m(a, j, k) > 0) {
		switch (a[k])
		{

		case 'e': if (ends(a, "icate", 5, k)) { k -= 5; parameter = 1; setto(2, "ic", a, j, j - k); break; }
				if (ends(a, "ative", 5, k)) { k -= 5; parameter = 1; setto(0, " ", a, j, j - k); break; }
				if (ends(a, "alize", 5, k)) { k -= 5; parameter = 1; setto(2, "al", a, j, j - k); break; }
				break;
		case 'i': if (ends(a, "iciti", 5, k)) { k -= 5; parameter = 1; setto(2, "ic", a, j, j - k); break; }
				break;
		case 'l': if (ends(a, "ical", 4, k)) { k -= 4; parameter = 1; setto(2, "ic", a, j, j - k); break; }
				if (ends(a, "ful", 3, k)) { k -= 3; parameter = 1; setto(0, " ", a, j, j - k); break; }
				break;
		case 's': if (ends(a, "ness", 4, k)) { k -= 4; parameter = 1; setto(0, "", a, j, j - k); break; }
				break;
		}
	}
}


/* step4() takes off -ant, -ence etc., in context <c>vcvc<v>,i.e. m()>1. */

void step4(string a, int j)
{
	if ((m(a, j, k) > 1))
	{
		switch (a[k - 1])
		{
		case 'a': if (ends(a, "al", 2, k)) { k -= 2; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 'c': if (ends(a, "ance", 4, k)) { k -= 4; parameter = 1; setto(0, " ", a, j, j - k); return; }
				if (ends(a, "ence", 4, k)) { k -= 4; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 'e': if (ends(a, "er", 2, k)) { k -= 2; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 'i': if (ends(a, "ic", 2, k)) { k -= 2; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 'l': if (ends(a, "able", 4, k)) { k -= 4; parameter = 1; setto(0, " ", a, j, j - k); return; }
				if (ends(a, "ible", 4, k)) { k -= 4; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 'n': if (ends(a, "ant", 3, k)) { k -= 3; parameter = 1; setto(0, " ", a, j, j - k); return; }
				if (ends(a, "ement", 5, k)) { k -= 5; parameter = 1; setto(0, " ", a, j, j - k); return; }
				if (ends(a, "ment", 4, k)) { k -= 4; parameter = 1; setto(0, " ", a, j, j - k); return; }
				if (ends(a, "ent", 3, k)) { k -= 3; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 'o': if (ends(a, "ion", 3, k) && (a[k - 3] == 's' || a[k - 3] == 't')) { k -= 3; parameter = 1; setto(0, " ", a, j, j - k); return; }
				if (ends(a, "ou", 2, k)) { k -= 2; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 's': if (ends(a, "ism", 3, k)) { k -= 3; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 't':
			if (ends(a, "ate", 3, k)) { k -= 3; parameter = 1; setto(0, " ", a, j, j - k); return; } //========================discuss
			if (ends(a, "iti", 3, k)) { k -= 3; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 'u': if (ends(a, "ous", 3, k)) { k -= 3; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 'v': if (ends(a, "ive", 3, k)) { k -= 3; parameter = 1; setto(0, " ", a, j, j - k); return; }
		case 'z': if (ends(a, "ize", 3, k)) { k -= 3; parameter = 1; setto(0, " ", a, j, j - k); return; }
		default: return;
		}
		if (m(a, j, k) > 1) j = k;
	}
}

/* step5() removes a final -e if m() > 1, and changes -ll to -l if
m() > 1. */

void step5(string a, int j)
{
	j = k;
	if (a[k] == 'e')
	{
		int x = m(a, j, k);
		if (x > 1) { k--; }
		else
			if ((x == 1) && !cvc(k, a, 0)) { k--;}
	}
	if (m(a, j, k) > 1)
	{
		if (doublec(a, k))
		{
			if (a[k] == 'l') k--;
		}
	}
}

/*Initialisatin and calls all 5 steps. */

int stem(string a, int i, int j)
{
	k = j; k0 = i; /* copy the parameters into statics */
	if (k <= k0 + 1) return k;
	step1ab(a, j);
	step2(a, j);
	step3(a, j);
	step4(a, j);
	step5(a, j);
	return k;
}

void eliminareCaractere(string& s, int& nr_semne)
{
	int lungime = s.length();
	for (int i = 0; i < lungime; i++)
		if (s[i] == '.' || s[i] == '?' || s[i] == '!' || s[i] == ',' || s[i] == '-' || s[i] == '(' || s[i] == ')')
		{
			nr_semne++;
			for (int j = i; j < lungime - 1; j++)
				s[j] = s[j + 1];
			lungime--;
			i--;
		}
}

void eliminareSemneUltimulCuvant(vector<string>& cuvinte, int nr_semne)
{
	for (int i = 0; i < nr_semne; i++)
		cuvinte[cuvinte.size() - 1].erase(cuvinte[cuvinte.size() - 1].end() - 1); //stergem semnele de punctuatie pe care le-am mutat la sfarsitul ultimului cuvant
}

void Stopword_Removal(vector<string>& cuvinte)
{
	std::vector<std::string> cuvinte_uzuale;
	cuvinte_uzuale.push_back("and");
	cuvinte_uzuale.push_back("because");
	cuvinte_uzuale.push_back("with");
	cuvinte_uzuale.push_back("or");
	cuvinte_uzuale.push_back("not");
	cuvinte_uzuale.push_back("others");
	cuvinte_uzuale.push_back("thus");
	cuvinte_uzuale.push_back("the");
	cuvinte_uzuale.push_back("of");
	for (int i = 0; i < cuvinte.size(); i++)
		for (int j = 0; j < cuvinte_uzuale.size(); j++)
			if (cuvinte[i] == cuvinte_uzuale[j])
			{
				cuvinte.erase(cuvinte.begin() + i);
				i--;
				break;
			}
}

void tokenizare(vector<string>& a, string& s, int& dima)
{
	int nr_semne = 0;
	string cuvant;
	eliminareCaractere(s, nr_semne);
	stringstream doc(s);
	while (getline(doc, cuvant, ' '))
		a.push_back(cuvant);
	eliminareSemneUltimulCuvant(a, nr_semne);
	Stopword_Removal(a);
	dima = a.size();
}

void initializare_hash_table(vector <cuvantt>& hash_table)
{
	cuvantt element;
	element.cod = 0;
	for (int k = 0; k < TABLE_SIZE; k++)
		hash_table.push_back(element);
}

int hashing(string a)
{
	int nrCifreMari = 0, nrCifreMici = 0, p, x, y; //ana, a-1, n-2, nn - 1414, nrcifremari=2
	for (int j = 0; j < a.size(); j++)
		if (int(a[j]) - 96 > 9)
			nrCifreMari++;
	for (int j = 0; j < a.size(); j++)
		if (int(a[j]) - 96 < 10)
			nrCifreMici++;
	int j = 0, aux = 0;
	p = a.size() - 1 + nrCifreMari;
	if (a.size() == nrCifreMari)
		p--;
	else if (((int)a[0] - 96) > 9)  // daca primul e mai mare, scade puterea initiala cu o unitate
		p--;
	while (p > -1)
	{
		if (a.size() == 1)
			return (int)a[0] - 96;
		x = (int)a[j] - 96;
		if (j < a.size() - 1)
			y = (int)a[j + 1] - 96;

		if (a.size() == nrCifreMari && y >= 10)   //cazul in care toate numerele sunt mari
		{
			aux = (int)x * pow(10, p) + (int)aux;
			aux = aux % TABLE_SIZE;
			j++;
			p -= 2;
		}
		else if (a.size() == nrCifreMici)   //cazul in care toate cifrele sunt mici
		{
			aux = (int)x * pow(10, p) + (int)aux;
			aux = aux % TABLE_SIZE;
			j++;
			p--;
		}
		else if (((int)a[0] - 96) >= 10)    //cazul in care prima cifra e mare
		{
			if (y >= 10)  //apoi urmatoarea cifra e tot mare
			{
				aux = (int)x * pow(10, p) + (int)aux;
				aux = aux % TABLE_SIZE;
				j++;
				p -= 2;
			}
			else  //sau urmatoarea cifra e mica
			{
				aux = (int)x * pow(10, p) + (int)aux;
				aux = aux % TABLE_SIZE;
				j++;
				p--;
			}
		}
		else if (((int)a[0] - 96) <= 10)  // cazul in care prima cifra e mica
		{
			if (y > 9)  //apoi urm mare
			{
				aux = (int)x * pow(10, p) + (int)aux;
				aux = aux % TABLE_SIZE;
				j++;
				p -= 2;
			}
			else  //sau urm mica
			{
				aux = (int)x * pow(10, p) + (int)aux;
				aux = aux % TABLE_SIZE;
				j++;
				p--;
			}
		}
	}
	return aux;
}

void inserare_tabel(string cuvant_de_inserat, vector <cuvantt>& hash_table)
{
	cuvantt index;
	index.cod = hashing(cuvant_de_inserat);
	index.denumire = cuvant_de_inserat;

	hash_table[index.cod].cod = index.cod;
	hash_table[index.cod].denumire = cuvant_de_inserat;
}

void memorare_in_hash_table(vector<string> a, vector <cuvantt>& hash_table, int dima)
{
	int warning = 0;

	for (int i = 0; i < dima; i++)
	{
		for (int j = 0; j < a[i].length(); ++j)
		{
			if (a[i][j] == '*' || a[i][j] == '!' || a[i][j] == '@' || a[i][j] == '#' || a[i][j] == '$' || a[i][j] == '%' || a[i][j] == '^' || a[i][j] == '&' || a[i][j] == '(' || a[i][j] == ')' || a[i][j] == '{' || a[i][j] == '[' || a[i][j] == '}' || a[i][j] == '.' || a[i][j] == '/' || a[i][j] == '>' || a[i][j] == '<' || a[i][j] == ',' || a[i][j] == ':' || a[i][j] == ';') if (warning == 0) { cout << "\nWarning: Special character found.\nSpecial character remain unaltered.\n"; warning = 1; }
			a[i][j] = tolower(a[i][j]);
		}
		int iCount = a[i].length() - 1;
		int nrLitereStemming = iCount - stem(a[i], 0, iCount);
		for (int j = 0; j < nrLitereStemming; j++)
			a[i].erase(a[i].end()-1);
		inserare_tabel(a[i], hash_table);
		//cout << "Stem word:" << a[i] << " " << "avand codul hash " << hashing4(a[i]) << "\n";
	}

}

int verificare_documente(vector <cuvantt> hash_table1, vector <cuvantt> hash_table2, int nr_document1, int nr_document2)
{
	int cuvinte_similare = 0;
	for (int i = 0; i < TABLE_SIZE; i++)
		if (hash_table2[i].cod != NULL)
			if (hash_table1[hash_table2[i].cod].cod == hash_table2[i].cod)
			{
				cout << "Am gasit cuvantul " << hash_table2[i].denumire << " in documentul "<< nr_document1 + 1 << " si in documentul " << nr_document2 + 1 << " avand codul hash: " << hash_table2[i].cod << "\n";
				cout << "Se verifica daca cuvintele corespund..." << "\n";
				if (hash_table2[i].denumire == hash_table1[hash_table2[i].cod].denumire)
					cout << "Da, corespund \n", cuvinte_similare++;
				else
					cout << "Nu, nu corespund \n";
			}
	return cuvinte_similare;
}

