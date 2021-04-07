#include <iostream>
#include <math.h>

using namespace std;

class Polynomial;

class Term {
friend Polynomial;
friend istream &operator>>(istream &input, Polynomial &p);
friend ostream &operator<<(ostream &output, const Polynomial &p);
private:
    float coef;  // coefficient
    int exp;     // exponent
};

class Polynomial {
friend istream &operator>>(istream &input, Polynomial &p);
friend ostream &operator<<(ostream &output, const Polynomial &p);
public:
    Polynomial() : terms{0}, capacity{terms}, termArray{new Term[capacity]} {};
    Polynomial Add(const Polynomial &p);
    Polynomial Mult(const Polynomial &p);
    float Eval(const float &f);
    void NewTerm(const float &theCoeff, const int &theExp);
private:
    Term *termArray;
    int capacity;
    int terms;
};

Polynomial Polynomial::Add(const Polynomial &p) {
    Polynomial sum;
    int Pos = 0, pPos = 0;
    while ((Pos < terms) && (pPos < p.terms)) {
      if (termArray[Pos].exp == p.termArray[pPos].exp) {
          float Addcoef = termArray[Pos].coef + p.termArray[pPos].coef;
          if (Addcoef) sum.NewTerm(Addcoef, termArray[Pos].exp);
          Pos++;
          pPos++;
      } else if (termArray[Pos].exp < p.termArray[pPos].exp) {
          sum.NewTerm(p.termArray[pPos].coef, p.termArray[pPos].exp);
          pPos++;
      } else {
          sum.NewTerm(termArray[Pos].coef, termArray[Pos].exp);
          Pos++;
      }
    }
    for ( ; Pos < terms; Pos++) {
        sum.NewTerm(termArray[Pos].coef, termArray[Pos].exp);
    }
    for ( ; pPos < p.terms; pPos++) {
        sum.NewTerm(p.termArray[pPos].coef, p.termArray[pPos].exp);
    }
    return sum;
}

Polynomial Polynomial::Mult(const Polynomial &p) {
    Polynomial product;
    for (int i = 0; i < terms; i++) {
        Polynomial temp;
        for (int j = 0; j < p.terms; j++) {
            temp.NewTerm(termArray[i].coef * p.termArray[j].coef, termArray[i].exp + p.termArray[j].exp);
        }
        product = temp.Add(product);
    }
    return product;
}

void Polynomial::NewTerm(const float &theCoeff, const int &theExp) {
    if (terms == capacity) {
        if (capacity == 0) capacity++;
        else capacity *= 2;
        Term *temp = new Term[capacity];
        copy(termArray, termArray + terms, temp);
        delete []termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoeff;
    termArray[terms++].exp = theExp;
}

float Polynomial::Eval(const float &f) {
    float result = 0;
    for (int i = 0; i < terms; i++) result += termArray[i].coef * pow(f, termArray[i].exp);
    return result;
}

istream &operator>>(istream &input, Polynomial &p) {
    cout << "Enter how many terms: ";
    input >> p.terms;
    p.capacity = p.terms;
    p.termArray = new Term[p.capacity];
    for (int i = 0; i < p.terms; i++) {
        cout << "coefficient: ";
        input >> p.termArray[i].coef;
        cout << "exponent: ";
        input >> p.termArray[i].exp;
        if (p.termArray[i].coef == 0) throw "The coefficient cannot be 0!";
    }
    return input;
}

ostream &operator<<(ostream &output, const Polynomial &p) {
    if (p.terms > 0) {
      for (int i = 0; i < p.terms; i++) {
          if (p.termArray[i].exp == 0) output << p.termArray[i].coef;
          else if (p.termArray[i].exp == 1 && p.termArray[i].coef == 1) output << "x";
          else if (p.termArray[i].exp == 1) output << p.termArray[i].coef << "x";
          else if (p.termArray[i].coef == 1) output << "x^" << p.termArray[i].exp;
          else output << p.termArray[i].coef << "x^" << p.termArray[i].exp;
          if (i < p.terms - 1) output << " + ";
          else output << "\n";
      }
    } else output << "0\n";

    return output;

}

int main() {
    Polynomial p1, p2;
    try {
        float value;
        cin >> p1 >> p2;
        cout << "Polynomial 1: " << p1;
        cout << "Polynomial 2: " << p2;
        cout << "A value to evaluate the polynomial: ";
        cin >> value;
        cout << "The value of polynomial 1 at " << value << ": " << p1.Eval(value) << endl;
        cout << "The value of polynomial 2 at " << value << ": " << p2.Eval(value) << endl;
        Polynomial add = p1.Add(p2);
        Polynomial multi = p1.Mult(p2);
        cout << "Sum: " << add << endl;
        cout << "Product: " << multi << endl;
    }
    catch (const char* msg) {
        cout << msg << endl;
        return 0;
    }
    return 0;
}
