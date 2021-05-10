/*
Вариант 7. ОТЯП.

Конструкция 7 - условный оператор.

if <логическое выражение> then
<операторы>
[ elseif <логическое выражение> then <операторы> ]
end


<логическое выражение> → <выражение сравнения> |
<унарная логическая операция><выражение сравнения> |
<логическое выражение><бинарная логическая операция><выражение сравнения>
<выражение сравнения> → <операнд> | <операнд><операция сравнения><операнд>
<операция сравнения> → < | > | = | <>
<унарная логическая операция> → not
<бинарная логическая операция> → and | or
<операнд> → <идентификатор> | <константа>
<операторы> → <операторы>; <оператор> | <оператор>
<оператор> → <идентификатор> = <арифметическое выражение>
<арифметическое выражение> → <операнд> |
<арифметическое выражение><арифметическая операция><операнд>
<арифметическая операция> → + | - | / | *



Ключевое слово [kw]: not, and, or, if, then, elseif, end
Специальный символ [co]: <, >, <>
Специальный символ [eq]: =
Специальный символ [ao]: +, -, /, *
Специальный символ [sc]: ;
Константы [vl]:
Идентификаторы [id]:
Мусор [wl]:
*/
/*
Уровень 1
<логическое выражение> → <операнд> | <операнд><операция сравнения><операнд>
<операция сравнения> → < | > | <= | >= | = | <>
<операнд> → <идентификатор>|<константа>
<операторы> → <идентификатор> = <арифметическое выражение>
<арифметическое выражение> → <операнд> |
<арифметическое выражение><арифметическая операция><операнд>
<арифметическая операция> → + | –


Уровень 2
<логическое выражение> → <выражение сравнения> |
<логическое выражение><логическая операция><выражение сравнения>
<выражение сравнения> → <операнд> | <операнд><операция сравнения><операнд>
<операция сравнения> → < | > | = | <>
<логическая операция> → and | or
<операнд> → <идентификатор> | <константа>
<операторы> → <идентификатор> = <арифметическое выражение>
<арифметическое выражение> → <операнд> |
<арифметическое выражение><арифметическая операция><операнд>
<арифметическая операция> → + | – | / | *
*/
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <ctype.h>

using namespace std;
enum M { co, eq, ao, sc, vl, id, wl, nt, lo, fi, th, ei, en };
const char* types[] = { "co","eq","ao","sc","vl","id","wl", "nt", "lo", "if", "th", "ei", "en" };
enum states {S, A, B, H, E, C, D, F, G, eA, eB, eH, eE};
states matrix[9][5]
{
    //  S,   A,  B,  H,  E, 
        A,   A,  E, eH,  E,   // буквы
        B,   A,  B, eH,  E,   // цифры
        C,  eA, eB, eH, eE,   // /*+-
        D,  eA, eB, eH, eE,   // =
        F,  eA, eB, eH, eE,   // ;
        G,  eA, eB,  G, eE,   // >
        H,  eA, eB, eH, eE,   // <
        S,  eA, eB, eH, eE,   // \n \0 \r \t ' '
        E,   E,  E, eH,  E,   // other
};
/*
хуйня -> fi 
<логическое выражение> L
<выражение сравнения> C
<операнд> O
<операторы> OPS
<оператор> OP
<арифметическое выражение> AV

<операция сравнения> ao
<идентификатор> id
<константа> vl
<арифметическая операция> ao
<унарная логическая операция> nt
<бинарная логическая операция> lo

1) L   →  C | nt C | C T1
    1')  T1 → lo C T1 | e
2) C   →  O | С ao O
    2')   C →  O T2
    2'')  T2 → ao O T2 | e
3) O   →  id | vl
4) OPS →  OPS sc OP | OP
    4')   OPS → OP T3
    4'')  T3 → sc OP T3 | e
5) OP  →  id eq AV
6) AV  →  O | AV ao O
    6')   AV → O T4
    6'')  T4 → ao O T4 | e

ИТОГО:
1)   S   →  fi L th OPS en | fi L th OPS ei L th OPS en
2)   L   →  C | nt C | C T1
3)   T1  →  lo C T1 | e
4)   C   →  O T2
5)   T2  →  ao O T2 | e
6)   O   →  id | vl
7)   OPS →  OP T3
8)   T3  →  sc OP T3 | e
9)   OP  →  id eq AV
10)  AV  →  O T4
11)  T4  →  ao O T4 | e

*/

struct Lex
{
    int type;
    size_t index;
    char* lex;
};


char* inputFromFile()  // Получение данных из файла "input.txt"
{
    ifstream finp("input.txt", ios::binary);
    if (!finp.is_open()) throw("I went through the files, looking for the right one, but I didn't find it...\n");
    finp.seekg(0, ios::end);    // Смещение = 0 от конца буфера потока
    streamoff size = finp.tellg();
    finp.seekg(0, ios::beg);    // Смещение = 0 от начала буфера потока
    char* text = new char[(size_t)size + 1];
    finp.read(&text[0], size);
    text[size] = '\0';
    finp.close();
    return text;
}

int iskw(const char* const str) // Проверка str на ключевое слово. Возвращает номер ключевого слова, иначе -1
{
    // Ключевое слово [kw]: not, and, or, if, then, elseif, end
    if (!strcmp(str, "not")) return nt;
    if (!strcmp(str, "and")) return lo;
    if (!strcmp(str, "or")) return lo;
    if (!strcmp(str, "if")) return fi;
    if (!strcmp(str, "then")) return th;
    if (!strcmp(str, "elseif")) return ei;
    if (!strcmp(str, "end")) return en;
    return -1;
}

void add(vector<Lex>& v, const char* const text, int firstPos, int lastPos, int type, size_t index)  // Создаёт слово с firstPos включительно по lastPos не включительно и добавляет его в вектор с проверками на ключевые слова и длину
{
    Lex word;  // Создаём лексему
    word.index = index;  // даём ей индекс
    word.lex = new char[lastPos - firstPos + 1];
    for (size_t i = firstPos; i < (size_t)lastPos; ++i) word.lex[i - firstPos] = text[i]; // WARNING
    word.lex[lastPos - firstPos] = '\0';
    if (type == id)  // Потенциальный id
    {
        word.type = iskw(word.lex); // Проверка на ключевое слово
        if (word.type == -1) // Если НЕ ключевое слово
            if (lastPos - firstPos > 5) word.type = wl;  //длина больше 5, то wl
            else word.type = id;
    }
    else if (type == vl)
    {
        (atoi(word.lex) > 32768) ? word.type = wl : word.type = vl;  // Если id больше диапазона, то wl
    } 
    else word.type = type;
    v.push_back(word);
}
size_t whatLine(char c)  // Функция возвращает индекс строки в матрице состояний (отсчёт с нуля)
{
    if (isalpha(c)) return 0;
    if (isdigit(c)) return 1;
    if (c == '*' || c == '/' || c == '+' || c == '-') return 2;
    if (c == '=') return 3;
    if (c == ';') return 4;
    if (c == '>') return 5;
    if (c == '<') return 6;
    if (isspace(c) || iscntrl(c)) return 7;
    return 8;
}

vector<Lex> lexAnalysis(const char* const text)
{
    vector<Lex> result;
    int position = 0;    // Индекс текущего символа из текста
    states state = S;  // Установлено начальное состояние S
    int firstPos = 0;  // Индекс начала палиндрома
    size_t index = 0;
    do {
        state = matrix[whatLine(text[position])][state];  
        switch (state) {
        case (S):
            firstPos = position + 1;
            break;
        case (eA): // Закончился идентификатор
            add(result, text, firstPos, position, id, index);
            state = S;
            ++index;
            firstPos = position;
            --position;
            break;
        case (eB): // Закончилось число
            add(result, text, firstPos, position, vl, index);
            state = S;
            ++index;
            firstPos = position;
            --position;
            break;
        case (C): // Закончилась арифметическая операция
            add(result, text, firstPos, position + 1, ao, index);
            state = S;
            firstPos = position + 1;
            ++index;
            break;
        case (D): // Символ =
            add(result, text, firstPos, position + 1, eq, index);
            state = S;
            firstPos = position + 1;
            ++index;
            break;
        case (F): // Символ ;
            add(result, text, firstPos, position + 1, sc, index);
            state = S;
            firstPos = position + 1;
            ++index;
            break;
        case (G): // Символ >
            add(result, text, firstPos, position + 1, co, index);
            state = S;
            firstPos = position + 1;
            ++index;
            break;
        case (eH): // Символ <
            add(result, text, firstPos, position, co, index);
            firstPos = position;
            --position;
            state = S;
            ++index;
            break;
        case (eE): // Закончилась некорректная лексема
            add(result, text, firstPos, position, wl, index);
            --position;
            firstPos = position;
            state = S;
            break;
        }
    } while (text[position++] != '\0');
    return result;
}

/*
ИТОГО:
1)   S   →  fi L th OPS en | fi L th OPS ei L th OPS en
1)   L   →  C | nt C | C T1
2)   T1  →  lo C T1 | e
3)   C   →  O T2
4)   T2  →  ao O T2 | e
5)   O   →  id | vl
6)   OPS →  OP T3
7)   T3  →  sc OP T3 | e
8)   OP  →  id eq AV
9)   AV  →  O T4
10)  T4  →  ao O T4 | e


if <логическое выражение> then
<операторы>
[ elseif <логическое выражение> then <операторы> ]
end


<логическое выражение> → <выражение сравнения> |
<унарная логическая операция><выражение сравнения> |
<логическое выражение><бинарная логическая операция><выражение сравнения>
<выражение сравнения> → <операнд> | <операнд><операция сравнения><операнд>
<операция сравнения> → < | > | = | <>
<унарная логическая операция> → not
<бинарная логическая операция> → and | or
<операнд> → <идентификатор> | <константа>
<операторы> → <операторы>; <оператор> | <оператор>
<оператор> → <идентификатор> = <арифметическое выражение>
<арифметическое выражение> → <операнд> |
<арифметическое выражение><арифметическая операция><операнд>
<арифметическая операция> → + | - | / | *

1) L   →  C | nt C | L lo C
2) C   →  O | С (co или eq) O
3) O   →  id | vl
4) OPS →  OPS sc OP | OP
5) OP  →  id eq AV
6) AV  →  O | AV ao O
*/


size_t matrix2[13][5]
{
//     0, 1, 2, 3, 4, 5, 
        ,  , 3,  ,  ,  , // ,  ,  ,  ,          // co (> > <>)
        ,  , 3,  ,  ,  , // ,  ,  ,  ,          // eq (=)
        ,  ,  ,  ,  ,  , // ,  ,  ,  ,          // ao (+/-*)
        ,  ,  ,  ,  ,  , // ,  ,  ,  ,          // sc (;)
        , 2,  , 5, 2, 2, // ,  ,  ,  ,          // vl (228)
        , 2,  , 5, 2, 2, // ,  ,  ,  ,          // id (abc123)
        ,  ,  ,  ,  ,  , // ,  ,  ,  ,          // wl (Er)
        , 4,  ,  ,  , 4, // ,  ,  ,  ,          // nt (not)
        ,  ,  ,  ,  , 1, // ,  ,  ,  ,          // lo (and/or)
       1,  ,  ,  ,  ,  , // ,  ,  ,  ,          // fi (if)
        ,  , 6, 6,  , 6, // ,  ,  ,  ,          // th (then)
        ,  ,  ,  ,  ,  , // ,  ,  ,  ,          // ei (elseif)
        ,  ,  ,  ,  ,  , // ,  ,  ,  ,          // en (end)
};

void SyntaxAnalysis(const char* const text)
{
    vector<Lex> v = lexAnalysis(text);
    LexStates state = S1;
    for(int i = 0; i < v.size(); i++)
    {
        state = matrix2[v[i].type][state];

    
    
    }

}


void saveToFile(const vector<Lex>& v)
{
    ofstream foutp("output.txt");   // Открытие выходного файла
    for (size_t i = 0; i < v.size(); ++i) foutp << v[i].lex << '[' << types[v[i].type] << ']' << ' ';
    foutp << endl;
    //for (size_t i = 0; i < v.size(); ++i) if (v[i].type == 6) foutp << v[i].lex << ' ';
    //foutp << endl;
    //for (size_t i = 0; i < v.size(); ++i) if (v[i].type == 5) foutp << v[i].lex << ' ';
    foutp.close();  // Закрытие выходного файла
}

int main()
{
    char* text = inputFromFile();
    vector<Lex> v = lexAnalysis(text);
    cout << v.size();
    saveToFile(v);
    for (size_t i = 0; i < v.size(); ++i) delete[] v[i].lex;
    delete[] text;
    return 0;
}