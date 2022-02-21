#include <stdio.h>
#include <malloc.h>
#include <string.h>
//Wieviele dezimale Stellen im Programm genutzt werden
#define numlength 60
#define true 1
#define false 0

typedef struct {
    //Marker, ob die Zahl negativ ist
    _Bool neg;
    //Speichert einzelne dezimale Stellen in "verkehrter" Reihenfolge.
    char nums[numlength];
} decimal_num;

decimal_num* decimal_add(decimal_num* num1, decimal_num* num2);
decimal_num* decimal_mult(decimal_num* num1, decimal_num* num2);
decimal_num* get_new_empty_num();
decimal_num* string_to_num(char* num_str);
decimal_num* num_with_offset(int num, int offset);
_Bool first_bigger(decimal_num* num1, decimal_num* num2);
decimal_num* cube(decimal_num* num);
decimal_num* add_3(decimal_num* num1, decimal_num* num2, decimal_num* num3);
void print_num(decimal_num* num);
void copy(decimal_num* to, decimal_num* from);

//Globale Zähler
int adds = 0;
int mults = 0;

int main() {
    decimal_num* x = string_to_num("-80538738812075974");
    decimal_num* y = string_to_num("80435758145817515");
    decimal_num* z = string_to_num("12602123297335631");
    decimal_num* ergebnis;
    ergebnis = add_3(cube(x), cube(y), cube(z));
    printf("\nErgebnis: ");
    print_num(ergebnis);
    printf("\nAdditionen: %d\nMultiplikationen: %d\n", adds, mults);
}

decimal_num* decimal_add(decimal_num* num1, decimal_num* num2) {
    //Addiert zwei Zahlen. Unterscheidet zwischen gleichen und verschiedenen Vorzeichen.
    //Bildet die schriftliche Addition / Subtraktion nach.

    decimal_num* ergebnis = get_new_empty_num();

    if(num1->neg == num2->neg) {
        ergebnis->neg = num1->neg;
        for (int i = 0; i < numlength - 1; i++) {
            int erg = num1->nums[i] + num2->nums[i] + ergebnis->nums[i];
            ergebnis->nums[i] = erg % 10;
            ergebnis->nums[i + 1] = erg / 10;
        }
    } else {
        //Bestimmen, welcher Wert (absolut) größer ist für spätere Berechnung.
        decimal_num *gr;
        decimal_num *kl;
        if (first_bigger(num1, num2)) {gr = num1; kl = num2;}
        else {gr = num2; kl = num1;}
        ergebnis->neg = gr->neg;

        for (int i = 0; i < numlength - 1; i++) {
            int erg = gr->nums[i] - kl->nums[i] + ergebnis->nums[i];
            if (erg >= 0) ergebnis->nums[i] = erg;
            else {
                ergebnis->nums[i] = erg + 10;
                ergebnis->nums[i + 1] = -1;
            }
        }
    }
    adds++;
    return ergebnis;
}

decimal_num* decimal_mult(decimal_num* num1, decimal_num* num2){
    //Multipliziert zwei Zahlen.
    //Bildet die schriftliche Multiplikation nach.

    decimal_num* ergebnis = get_new_empty_num();

    for(int i = 0; i < numlength; i++){
        for (int j = 0; j < numlength - i - 1; j++) {
            decimal_num* temp = num_with_offset(num1->nums[i] * num2->nums[j], i + j);
            ergebnis = decimal_add(ergebnis, temp);
            //free(temp);
        }
    }
    if (num1->neg == num2->neg) ergebnis->neg = false; else ergebnis->neg = true;

    mults++;
    return ergebnis;
}

decimal_num* get_new_empty_num() {
    decimal_num* num = (decimal_num*) malloc(sizeof(decimal_num));
    num->neg = false;
    for(int i = 0; i < numlength; i++) num->nums[i] = 0;
    return num;
}

decimal_num* string_to_num(char* num_str) {
    decimal_num* ergebnis = get_new_empty_num();
    if(num_str[0] == '-'){
        ergebnis->neg = true;
        num_str++;
    }
    int str_length = strlen(num_str);
    for(int i = 0; i < str_length; i++){
        ergebnis->nums[i] = num_str[str_length - i - 1] - 48;
        if(ergebnis->nums[i] > 9){
            printf("Die eingegebene Zahl beinhaltet nicht nur Ziffern!");
            exit(1);
        }
    }
    return ergebnis;
}

decimal_num* num_with_offset(int num, int offset){
    //Only works for num < 100 and at positions in bounds
    decimal_num* ergebnis = get_new_empty_num();
    ergebnis->nums[offset] = num % 10;
    ergebnis->nums[offset + 1] = num / 10;
    return ergebnis;
}

_Bool first_bigger(decimal_num* num1, decimal_num* num2){
    //Only looks at absolutes.
    for (int i = 0; i < numlength; ++i) {
        if (num1->nums[numlength-1-i] > num2->nums[numlength-1-i]) return true;
        else if (num1->nums[numlength-1-i] < num2->nums[numlength-1-i]) return false;
    }
    return false;
}

decimal_num* cube(decimal_num* num){
    return decimal_mult(decimal_mult(num, num), num);
}

decimal_num* add_3(decimal_num* num1, decimal_num* num2, decimal_num* num3){
    return decimal_add(decimal_add(num1, num2), num3);
}

void print_num(decimal_num* num){
    _Bool nonnullreached = false;
    if(num->neg) printf("%s", "-");
    for(int i = 0; i < numlength; i++){
        if(!nonnullreached & (num->nums[numlength - 1 - i] != 0)) nonnullreached = true;
        if(nonnullreached) printf("%d", num->nums[numlength - 1 - i]);
    }
}

void copy(decimal_num* to, decimal_num* from){
    to->neg = from->neg;
    for(int i = 0; i < numlength; i++){
        to->nums[i] = from->nums[i];
    }
}
