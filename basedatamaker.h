#ifndef BASEDATAMAKER_H
#define BASEDATAMAKER_H

#include <QByteArray>
#include "metadatamaker.h"

class BaseData;
class MetaData;

enum SensorType { Invalid=-1, XRF, EMS, LIBS};

// Responsible for hiding the coupling between metadata and the actual data during the object creation process.
//
class BaseDataMaker
{
public:    
    // Given a buffer from the socket, create a data object.
    //
    static BaseData* MakeData(SensorType type);

    // Based on data, generate buffer to send to the server.
    //
    static bool GenerateBufferForSending(BaseData* bd, QByteArray& qArray);
private:
    BaseDataMaker();

    static MetaData* XRFMetaData;
    static MetaData* EMSMetaData;
    static MetaData* LIBSMetaData;
};

#endif // BASEDATAMAKER_H
