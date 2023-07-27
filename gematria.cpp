#include "gematria.h"


quint64 getGematria(QString str)
{
    quint64 count = 0;
    QChar c;

    str = str.toLower();

    for(int i = 0; i < str.length(); i++)
    {
        c = str[i];
        switch(c.toLatin1())
        {
        case 'a':
            count += 1;
            break;
        case 'b':
            count += 2;
            break;
        case 'c':
            count += 3;
            break;
        case 'd':
            count += 4;
            break;
        case 'e':
            count += 5;
            break;
        case 'f':
            count += 6;
            break;
        case 'g':
            count += 7;
            break;
        case 'h':
            count += 8;
            break;
        case 'i':
            count += 9;
            break;
        case 'j':
            count += 10;
            break;
        case 'k':
            count += 11;
            break;
        case 'l':
            count += 12;
            break;
        case 'm':
            count += 13;
            break;
        case 'n':
            count += 14;
            break;
        case 'o':
            count += 15;
            break;
        case 'p':
            count += 16;
            break;
        case 'q':
            count += 17;
            break;
        case 'r':
            count += 18;
            break;
        case 's':
            count += 19;
            break;
        case 't':
            count += 20;
            break;
        case 'u':
            count += 21;
            break;
        case 'v':
            count += 22;
            break;
        case 'w':
            count += 23;
            break;
        case 'x':
            count += 24;
            break;
        case 'y':
            count += 25;
            break;
        case 'z':
            count += 26;
            break;
        }
    }

    return count;
}

QString removeSpaces(QString str)
{
    QString res = "";
    for(int i = 0; i < str.length(); i++)
    {
        if(res[i].toLatin1() != ' ')
        {
            res += str[i];
        }
    }
    return res;
}
