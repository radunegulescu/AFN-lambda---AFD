#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;
ifstream f("Text.txt");
struct tranzitie {			// Strcutura ce reprezinta trazitiile
	int vec;				// Starea in care se duce tranzitia 
	char c;					// Litera tranzitiei
};												
struct noduri {										// Strcutura ce reprezinta o stare din AFN lambda
	tranzitie* tranz = new tranzitie[100];			// Tranzitiile care pleaca stare
	int* Inc = new int[100];						// Inchiderile unei stari
	int nrinchideri = 0;
	int nrtranz = 0;
	bool fin = false;
	int lcl[100][100] = { 0 };						// Matrice ce reprezinta linia unei stari din tabelul lambda* litera lamda* 
	int dim[100];									// Lungimea vectorului ce reprezinta starea corespunzatoare unei stari din AFN
													// din tabelul lambda* litera lamda* pentru o anumita litera
};
struct stareafd										// Strcutura ce reprezinta un nod din AFD
{
	int* vect= new int[100];						// Vectorul ce reprezinta starea din AFD
	tranzitie* tranz = new tranzitie[100];			// Tranzitiile starii din AFD
	int nr = 0;										// Dimensiunea vectorului de mai sus
	bool fin = false;
}s[100];
void inchidere(int init, int curent, int *viz, noduri *v) {
	for (int i = 0; i < v[curent].nrtranz; i++) {
		if (v[curent].tranz[i].c == '@' && viz[v[curent].tranz[i].vec] == 0) {
			viz[v[curent].tranz[i].vec] = 1;
			v[init].Inc[v[init].nrinchideri++] = v[curent].tranz[i].vec;
			inchidere(init, v[curent].tranz[i].vec, viz, v);
		}
	}
}
bool egal(int* v1, int* v2, int n1, int n2) {
	if (n1 != n2) {
		return false;
	}
	else {
		for (int i = 0; i < n1; i++) {
			if (v1[i] != v2[i]) {
				return false;
			}
		}
	}
	return true;
}
int i, j, k, l, nrstari, nrlitere, init, nrfin, x, a, b, y;
char c, lit;
int main() {
	f >> nrlitere;								// Numarul de litere din alfabet
	char* litere = new char[nrlitere];	
	for (i = 0; i < nrlitere; i++) {
		f >> litere[i];							// Citire alfabet 
	}
	f >> nrstari;								// Numarul de stari
	noduri* v = new noduri[nrstari];
	f >> init;									// Starea initiala
	f >> nrfin;									// Numarul de stari finale
	for (i = 0; i < nrfin; i++) {
		f >> x;									//  Citire stari finale
		v[x].fin = true;
	}
	while (f >> a >> b >> lit) {				// Citire tranzitii
		v[a].tranz[v[a].nrtranz].vec = b;
		v[a].tranz[v[a].nrtranz].c = lit;
		++v[a].nrtranz;
	}
	cout << "AFN lambda \n";
	cout << "\nInitial: q" << init << "\n\n";
	for (i = 0; i < nrstari; i++) {				// Afisare AFN lambda
		cout << "q" << i << ":  \ntranzitii:\n";
		for (j = 0; j < v[i].nrtranz; j++) {
			cout << v[i].tranz[j].c << " " << v[i].tranz[j].vec << '\n';
		}
		cout << "final: ";
		if (v[i].fin) {
			cout << "DA";
		}
		else {
			cout << "NU";
		}
		cout << "\n\n";
	}
	int* viz = new int[nrstari];
	for (i = 0; i < nrstari; i++) {				// Formare inchideri lambda in vectorii Inc din structurile noduri
		for (j = 0; j < nrstari; j++) {
			viz[j] = 0;
		}
		v[i].Inc[v[i].nrinchideri++] = i;
		viz[i] = 1;
		inchidere(i, i, viz, v);
	}
	for (i = 0; i < nrstari; i++) {						// Sortare inchideri lambda
		sort(v[i].Inc, v[i].Inc + v[i].nrinchideri);
	}
	cout << "\n Inchideri:\n";
	for (i = 0; i < nrstari; i++) {						// Afisare inchideri lambda
		cout << endl << i << ":  ";
		for (j = 0; j < v[i].nrinchideri; j++) {
			cout << v[i].Inc[j] << " ";
		}
	}
	cout<<endl;
	int* v2;
	// Formare tabel: stari / lambda* litera lambda*
	for (i = 0; i < nrlitere; i++) {			// Luam fiecare litera din alfabet		
		char ch = litere[i];
		for (j = 0; j < nrstari; j++) {			// Luam fiecare stare din automat
			v2 = new int[nrstari];
			int ind2 = 0;
			for (k = 0; k < v[j].nrinchideri; k++) {	// Luam ficare stare din inchiderea starii j
				x = v[j].Inc[k];
				for (l = 0; l < v[x].nrtranz; l++) {	// Cautam prin tranzitiile starii x din inchidere
					if (v[x].tranz[l].c == ch) {	// Cautam starile de care se leaga in mod direct starea x din inchidere prin litera ch 
						v2[ind2++] = v[x].tranz[l].vec;		// Adaugam starea in vectorul v2
					}	
				}
			}
			for (int o = 0; o < nrstari; o++) {		// Vectorul viz folosit pt a marca starile care se afla in inchiderile starilor din v2 
				viz[o] = 0;                            
			}
			for (int u = 0; u < ind2; u++) {			// Marcarea
				for (int o = 0; o < v[v2[u]].nrinchideri; o++) {
					viz[v[v2[u]].Inc[o]] = 1;
				}
			}
			int dim = 0;
			for (int o = 0; o < nrstari; o++) {		// Formarea tabelului in matricea lcl pe linii aflandu-se indicii literele 
													// iar pe coloane aflandu-se starile corespunzatoare pentru fiecare litera
				if (viz[o]) {
					v[j].lcl[i][dim++] = o;
				}
			}
			v[j].dim[i] = dim;
			delete[]v2;
		}
	}
	cout << "\nTabel stari / lambda* litera lambda* \n\n";
	for (i = 0; i < nrstari; i++) {					// Afisarea tabelului stari / lambda* litera lambda* 
		cout << "q" << i << ": " << endl;
		for (j = 0; j < nrlitere; j++) {
			cout << "lambda* " << litere[j] << " lambda*" << ": ";
			cout << "q";
			for (k = 0; k < v[i].dim[j] ; k++) {
				cout << v[i].lcl[j][k];
			}
			cout << endl << endl;
		}
	}
	s[0].nr = v[init].nrinchideri;				// Introducerea starii initiale in AFD reprezentata prin inchiderea starii initiale din AFN lambda
	for (i = 0; i < v[init].nrinchideri; i++) {
		s[0].vect[i] = v[init].Inc[i];
	}
	int st = 0, dr = 0, OK = 1;
	int* V;
	while (OK||st<=dr) {						// Formarea AFD-ului se opreste cand nu mai gasim nicio stare noua,
												// aceasta continuandu-se pana cand se formeaza tranzitiile tuturor starilor
		OK = 0;
		for (j = 0; j < nrlitere; j++) {		// Parcurgerea literelor din alfabet
			V = new int[100];
			int len = 0;
			for (i = 0; i < nrstari; i++) {		// Vectorul viz folosit pt a marca starile din coloana 
												// lambda* litera[j] lambda* pentru nodul x din tabelul format mai sus
				viz[i] = 0;
			}
			for (i = 0; i < s[st].nr; i++) {	// Parcurgerea starilor AFN lambda ce formeaza starea din AFD curenta
				x = s[st].vect[i];
				for (k = 0; k < v[x].dim[j]; k++) {
					viz[v[x].lcl[j][k]] = 1;	// Marcarea definita mai sus
				}
			}
			for (i = 0; i < nrstari; i++) {
				if (viz[i]) {
					V[len++] = i;		// Formarea vectorului V care contine starile din AFN lambda marcate
										// Vectorul acestor stari constituie o stare din AFD
				}
			}
			int OK2 = 1;
			for (i = 0; i <= dr; i++) {
				if (egal(V, s[i].vect, len, s[i].nr)) {	// Verificarea daca starea AFD obtinuta se afla deja in AFD
					OK2 = 0;
					break;
				}
			}
			if (OK2) {		// Stare noua
				OK = 1;
				dr++;
				s[dr].nr = len;		// Adaugarea lungimii vectorului de stari din AFN lambda care reprezinta starea din AFD 
				for (i = 0; i < len; i++) {
					s[dr].vect[i] = V[i];	// Formarea starii din AFD
				}
				s[st].tranz[j].c = litere[j];	// Formarea tranzitiei cu litera[j] a starii curente cu starea nou adaugata
				s[st].tranz[j].vec = dr;		// Adaugarea indicelui starii noi din AFD cu care se leaga starea curenta prin litera[j]
			}
			else {								// Stare care se afla deja in AFD
				s[st].tranz[j].c = litere[j];	// Formarea tranzitiei cu litera[j] a starii curente cu starea gasita
				s[st].tranz[j].vec = i;			// Adaugarea indicelui starii gasite cu care se leaga starea curenta prin litera[j]
			}
		}
		st++;		// Trecerea la urmatoarea stare obtinuta in AFD
	}
	// Afisare AFD
	cout << "AFD: \n\n";
	cout << "Initial: q";
	for (j = 0; j < s[0].nr; j++) {
		cout << s[0].vect[j];
	}
	cout << "\n\n";
	for (i = 0; i <= dr; i++) {
		cout << "q";
		for (j = 0; j < s[i].nr; j++) {
			if (v[s[i].vect[j]].fin == true) {
				s[i].fin = true;		// Marcarea nodurilor finale din AFD
			}
			cout << s[i].vect[j];	// Afisarea starilor din AFD
		}
		cout << endl;
		cout << "Final: ";
		if (s[i].fin) {			// Afisarea daca starea e finala sau nu
			cout << "DA";
		}
		else {
			cout << "NU";
		}
		cout << "\ntranz: \n";
		for (j = 0; j < nrlitere; j++) {
			cout << s[i].tranz[j].c << " " << s[i].tranz[j].vec << endl;	// Afisarea tranzitiilor din AFD
		}
		cout << endl << endl;
	}
}