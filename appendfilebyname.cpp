#include "appendfilebyname.h"
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

void appendFileByName(const QString& fileName, const QStringList& lines){
    QFile file(fileName);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString& line : lines) {
            out << line << "\n";
        }
        file.close();
    } else {
        // Error handling
    }
}
