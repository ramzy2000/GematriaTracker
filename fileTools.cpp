#include "fileTools.h"

QString readFile(QString fileName)
{
    QFile file(fileName);
    QString fileContents = "";
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        fileContents = in.readAll();
        file.close();
    }
    return fileContents;
}
