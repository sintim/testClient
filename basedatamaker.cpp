#include <string.h>
#include <iostream>
#include <sstream>
#include <ctime>
#include "basedatamaker.h"
#include "basedata.h"
#include "xrfdata.h"
#include "emsdata.h"
#include "libsdata.h"
#include "metadata.h"

MetaData* BaseDataMaker::XRFMetaData = 0;
MetaData* BaseDataMaker::EMSMetaData = 0;
MetaData* BaseDataMaker::LIBSMetaData = 0;

const double doubleVal = 1.345;
const int intVal = 50;

const int intSize = 4;
const int doubleSize = 8;

char intBuf[intSize] = {0};
char doubleBuf[doubleSize] = {0};

std::string ConvertIntToString(int value)
{
    std::stringstream ss;
    ss << value;
    std::string str = ss.str();
    return str;
}

std::string ConvertDoubleToString(double value)
{
    std::stringstream ss;
    ss << value;
    std::string str = ss.str();
    return str;
}

// Write supplementary data to object.
//
bool WriteSupplementaryData(MetaData* md, BaseData* bd, int index)
{
    std::string resultingString;

    // Looking at the data definition type of the supplementary data.
    //
    if (md->GetSupplementaryDataDefinition(index).type == Double)
    {
        resultingString = ConvertDoubleToString(doubleVal);
    }
    else if (md->GetSupplementaryDataDefinition(index).type == Integer)
    {
        resultingString = ConvertIntToString(intVal);
    }
    else
    {
        return false;
    }

    bd->SetSupplementaryData(index, resultingString);
    return true;
}

// Write data to object.
//
bool WriteData(MetaData* md, BaseData* bd, int row, int column)
{
    std::string resultingString;

    // Looking at data definition.
    //
    if (md->GetDataDefinition(row, column).type == Double)
    {
        resultingString = ConvertDoubleToString(doubleVal);
    }
    else if (md->GetDataDefinition(row, column).type == Integer)
    {
        resultingString = ConvertIntToString(intVal);
    }
    else
    {
        return false;
    }

    bd->SetData(row, column, resultingString);
    return true;
}

// Copy an int to exactly sizeof(int) bytes.
//
void CopyInt(int value, char* buffer)
{
  memcpy(buffer, (void*)&value, sizeof(int));
}

// Copy a double to exactly sizeof(double) bytes.
//
void CopyDouble(double value, char* buffer)
{
  memcpy(buffer, (void*)&value, sizeof(double));
}

// Reading supplementary data from object, convert to approprite type based
// on definition, and append to buffer.
//
bool WriteSupplementaryDataToBuffer(MetaData* md, BaseData* bd, int index, QByteArray& qArray)
{
    // Looking at the data definition type of the supplementary data.
    //
    if (md->GetSupplementaryDataDefinition(index).type == Double)
    {
        double value = atof(bd->GetSupplementaryData(index).c_str());
        CopyDouble(value, doubleBuf);
        qArray.append(doubleBuf, doubleSize);
    }
    else if (md->GetSupplementaryDataDefinition(index).type == Integer)
    {
        int value = atoi(bd->GetSupplementaryData(index).c_str());
        CopyInt(value, intBuf);
        qArray.append(intBuf, intSize);
    }
    else
    {
        return false;
    }

    return true;
}

// Reading data from object, convert to approprite type based
// on definition, and append to buffer.
//
bool WriteDataToBuffer(MetaData* md, BaseData* bd, int row, int column, QByteArray& qArray)
{
    // Looking at data definition.
    //
    if (md->GetDataDefinition(row, column).type == Double)
    {
        double value = atof(bd->GetData(row, column).c_str());
        CopyDouble(value, doubleBuf);
        qArray.append(doubleBuf, doubleSize);
    }
    else if (md->GetDataDefinition(row, column).type == Integer)
    {
        int value = atoi(bd->GetData(row, column).c_str());
        CopyInt(value, intBuf);
        qArray.append(intBuf, intSize);
    }
    else
    {
        return false;
    }

    return true;
}



std::string intToString(int value)
{
    std::ostringstream convert;   // stream used for the conversion
    convert << value;      // insert the textual representation of 'Number'in the characters in the stream
    std::string stringVal = convert.str(); // set 'Result' to the contents of the stream

    if (stringVal.length() == 1)
        return "0" + stringVal;
    else
        return stringVal;
}


const std::string GenerateTimeStamp()
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );

    std::string ts = intToString(now->tm_year + 1900) +
            intToString(now->tm_mon + 1) +
            intToString(now->tm_mday) +
            intToString(now->tm_hour) +
            intToString(now->tm_min) +
            intToString(now->tm_sec) ;

    return ts;
}

BaseDataMaker::BaseDataMaker()
{
}

BaseData* BaseDataMaker::MakeData(SensorType type)
{
    if (XRFMetaData == 0)
        XRFMetaData = MetaDataMaker::MakeMetaData(".\\XRF.txt");
    if (EMSMetaData == 0)
        EMSMetaData = MetaDataMaker::MakeMetaData(".\\EMS.txt");
    if (LIBSMetaData == 0)
        LIBSMetaData = MetaDataMaker::MakeMetaData(".\\LIBS.txt");

    BaseData* bd = 0;
    MetaData* currentMetaData = 0;
\
    if (type == XRF)
    {
        currentMetaData = XRFMetaData;
        bd = new XRFData(XRFMetaData->GetNumOfSupplementaryData(),
                         XRFMetaData->GetSizeYDataDefinition(),
                         XRFMetaData->GetSizeXDataDefinition());
    }

    else if (type == EMS)
    {
        currentMetaData = EMSMetaData;
        bd = new EMSData(EMSMetaData->GetNumOfSupplementaryData(),
                         EMSMetaData->GetSizeYDataDefinition(),
                         EMSMetaData->GetSizeXDataDefinition());
    }
    else if (type == LIBS)
    {
        currentMetaData = LIBSMetaData;
        bd = new LIBSData(LIBSMetaData->GetNumOfSupplementaryData(),
                         LIBSMetaData->GetSizeYDataDefinition(),
                         LIBSMetaData->GetSizeXDataDefinition());
    }
    else
    {
        printf ("Invalid sensor data received.\n");
        return 0;
    }

    // timestamp.
    //
    bd->SetTimestamp(GenerateTimeStamp());

    // Supplementary data.
    //
    for (int i = 0; i < bd->GetSupplementaryDataSize(); i++)
    {
        bool writeSuccessful = WriteSupplementaryData(currentMetaData, bd, i);

        if (!writeSuccessful)
        {
            delete bd;
            return 0;
        }
    }

    // data.
    //
    for (int i = 0; i < bd->GetNumRowInData(); i++)
    {
        for (int j = 0; j < bd->GetNumColumnInData(); j++)
        {
            bool writeSuccessful = WriteData(currentMetaData, bd, i, j);

            if (!writeSuccessful)
            {
                delete bd;
                return 0;
            }
        }
    }

    return bd;
}

bool BaseDataMaker::GenerateBufferForSending(BaseData* bd, QByteArray& qArray)
{
    // Determine type.
    //
    MetaData* currentMetaData = 0;
    SensorType enumVal = Invalid;

    if (0 != dynamic_cast<XRFData*>(bd))
    {
        currentMetaData = XRFMetaData;
        enumVal = XRF;
    }
    else if (0 != dynamic_cast<EMSData*>(bd))
    {
        currentMetaData = EMSMetaData;
        enumVal = EMS;
    }
    else if (0 != dynamic_cast<LIBSData*>(bd))
    {
        currentMetaData = LIBSMetaData;
        enumVal = LIBS;
    }
    else
        return false;

    // TimeStamp
    //
    qArray.append(bd->GetTimeStamp().c_str());

    // Type
    //
    CopyInt(enumVal, intBuf);
    qArray.append(intBuf, 4);

    // Supplementary data.
    //
    for (int i = 0; i < bd->GetSupplementaryDataSize(); i++)
    {
        bool writeSuccessful = WriteSupplementaryDataToBuffer(currentMetaData, bd, i, qArray);

        if (!writeSuccessful)
        {
            delete bd;
            return false;
        }
    }

    // data.
    //
    for (int i = 0; i < bd->GetNumRowInData(); i++)
    {
        for (int j = 0; j < bd->GetNumColumnInData(); j++)
        {
            bool writeSuccessful = WriteDataToBuffer(currentMetaData, bd, i, j, qArray);

            if (!writeSuccessful)
            {
                delete bd;
                return false;
            }

        }
    }

    return true;
}
