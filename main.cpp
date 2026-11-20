#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string keywordArr[16]     = {"int", "float", "double", "char", "bool", "void", "string",
                             "if", "else", "while", "for", "do", "return", "class",
                             "main", "\"Hello world\""};
string operatorArr[10]    = {"=", "+", "-", "*", "/", "<<", ">>", "==", "<", ">"};
string punctuationArr[8]  = {"(", ")", "{", "}", "[", "]", ";", ","};
string libraryArr[3]      = {"cout", "cin", "endl"};

string seen[500];
int seenCount = 0;
int lineNo = 0;
bool headerShown = false;

string prevWord = "";
bool inString = false;
string text = "";

bool found(string word, string arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == word)
            return true;
    }
    return false;
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isLetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\r';
}

bool isNumber(string w)
{
    for (int i = 0; i < w.length(); i++)
    {
        if (!isDigit(w[i]) && w[i] != '.')
            return false;
    }
    return isDigit(w[0]);
}

bool isIdentifier(string w)
{
    if (!isLetter(w[0]))
        return false;
    for (int i = 1; i < w.length(); i++)
    {
        if (!isLetter(w[i]) && !isDigit(w[i]))
            return false;
    }
    return true;
}

bool isString(string w)
{
    return w.length() >= 2 && w[0] == '"' && w[w.length() - 1] == '"';
}

void classify(string w, char nextChar, string prev)
{
    if (found(w, seen, seenCount))
        return;
    seen[seenCount] = w;
    seenCount++;

    if (headerShown == false)
    {
        cout << "Line " << lineNo << ":" << endl;
        headerShown = true;
    }

    if (found(w, keywordArr, 16))
        cout << "   " << w << "  -> KEYWORD" << endl;
    else if (found(w, operatorArr, 10))
        cout << "   " << w << "  -> OPERATOR" << endl;
    else if (found(w, punctuationArr, 8))
        cout << "   " << w << "  -> PUNCTUATION" << endl;
    else if (isNumber(w))
        cout << "   " << w << "  -> CONSTANT " << endl;
    else if (isString(w))
        cout << "   " << w << "  -> STRING" << endl;
    else if (isIdentifier(w))
    {
        if (found(w, libraryArr, 3))
            cout << "   " << w << "  -> IDENTIFIER " << endl;
        else if (prev == "class")
            cout << "   " << w << "  -> IDENTIFIER " << endl;
        else if (nextChar == '(')
            cout << "   " << w << "  -> IDENTIFIER " << endl;
        else
            cout << "   " << w << "  -> IDENTIFIER " << endl;
    }
    else
        cout << "   " << w << "  -> UNKNOWN" << endl;
}


void tokenize(string line)
{
    string word = "";
    int len = line.length();
    int i = 0;

    do
    {
        char ch;
        if (i < len)
            ch = line[i];
        else
            ch = ' ';

        if (!isSpace(ch))
        {
            word = word + ch;
        }
        else if (word != "")
        {
            int j = i;
            while (j < len && isSpace(line[j]))
                j++;
            char nextChar = '\0';
            if (j < len)
                nextChar = line[j];

            if (word == "\"")
            {
                if (inString == false)
                {
                    inString = true;
                    text = "";
                }
                else
                {
                    inString = false;
                    string full = "\"" + text + "\"";
                    classify(full, nextChar, prevWord);
                    prevWord = full;
                }
            }
            else if (inString == true)
            {
                if (text != "")
                    text = text + " ";
                text = text + word;
            }
            else
            {
                classify(word, nextChar, prevWord);
                prevWord = word;
            }
            word = "";
        }
        i++;
    } while (i <= len);
}


void readFile(string fileName)
{
    ifstream file(fileName);
    if (!file)
    {
        cout << "Could not open file" << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        lineNo++;
        headerShown = false;
        tokenize(line);
    }

    file.close();
}

int main()
{
    readFile("Tokenisation.txt");
    return 0;
}
