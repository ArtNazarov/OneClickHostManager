#include "transformurls.h"
#include <QString>
#include <QFile>
#include <QTextStream>

void transformUrls(const QString& inputFile, const QString& outputFile){

    QString prefix = "   ";

    QFile input(inputFile);
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QFile output(outputFile);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text)) {
        input.close();
        return;
    }

    QTextStream in(&input);
    QTextStream out(&output);

    while (!in.atEnd()) {
        QString line = in.readLine();
        out << prefix << line << "\n";
    }

    input.close();
    output.close();
}
