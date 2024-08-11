#include "deletefiles.h"
#include <QString>
#include <QFile>
void deleteFileByName(const QString& fileName){
    // Create a QFile object
    QFile file(fileName);
    file.remove();
}
