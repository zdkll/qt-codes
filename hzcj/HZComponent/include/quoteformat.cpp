#include "quoteformat.h"


namespace HzQuoteFormat
{

QHash<HzQuoteFormat::UnitType,QString> unitHash={{SigUnit,QStringLiteral("元")}
                                                 ,{TenThousands,QStringLiteral("万元")}
                                                 ,{Hundmillion,QStringLiteral("亿元")}};

QHash<HzQuoteFormat::UnitType,long long>  unitMagnitudeHash = {
    {SigUnit,1}
    ,{TenThousands,1e4}
    ,{Hundmillion,1e8}
};

QString unitString(UnitType type)
{
    return unitHash.value(type);
}

long long unitMagnitude(UnitType type)
{
    return unitMagnitudeHash.value(type,1);
}

int compareFloat(double fa, double fb, double epsinon)
{
    int ret = 0;
    double val = fa-fb;
    if ((val >= - epsinon) && (val <= epsinon))
    {
        ret = 0;
    }else if(val > 0)
    {
        ret = 1;
    }else if(val < 0)
    {
        ret = -1;
    }
    return ret;
}

UnitType HzQuoteFormat::calUnitByValue(long long value)
{
    if(value < 1e4)
        return SigUnit;
    else if(1e4<= value && value<1e8)
        return TenThousands;
    else if(1e8<= value && value<1e12)
        return Hundmillion;
    else return Hundmillion;
    return SigUnit;
}

QString HzQuoteFormat::formatByUnitType(long long value,int unitType,int dot_num)
{
    switch (unitType) {
    case SigUnit:
        return QString::number(value,'f',dot_num);
        break;
    case TenThousands:
        return QString::number(double(value)/1.0e4f,'f',dot_num);
        break;
    case Hundmillion:
        return QString::number(double(value)/1.0e8f,'f',dot_num);
        break;
    default:
        break;
    }
}

QString StringUnitFormat::toFormatString(const qulonglong &in, int pointNum)
{
    if(in == INVALID_UINT64)
        return DEFAULT_STR_PRICE;
    double out = in;

    QString suffix;
    if(in < 1e4)
        return QString::number(in);
    else if(1e4<= in && in<1e8)
    {
        out = double(in)/1e4;
        suffix = QStringLiteral("万");
    }
    else if(1e8<= in && in<1e12)
    {
        out = double(in)/1e8;
        suffix = QStringLiteral("亿");
    }else if(1e12<= in)
    {
        out = double(in)/1e12;
        suffix = QStringLiteral("万亿");
    }
    return QString::number(out,'f',pointNum)+suffix;
}

QString StringUnitFormat::toFormatString(const qlonglong &in, int pointNum)
{
    if(in == INVALID_INT64)
        return DEFAULT_STR_PRICE;
    double out = in;

    QString suffix;
    if(abs(in) < 1e4)
        return QString::number(in);
    else if(1e4<= abs(in) && abs(in)<1e8)
    {
        out = double(in)/1e4;
        suffix = QStringLiteral("万");
    }
    else if(1e8<= abs(in) && abs(in)<1e12)
    {
        out = double(in)/1e8;
        suffix = QStringLiteral("亿");
    }else if(1e12<= abs(in))
    {
        out = double(in)/1e12;
        suffix = QStringLiteral("万亿");
    }
    return QString::number(out,'f',pointNum)+suffix;
}

QString StringUnitFormat::toFormatString(const double &in, int pointNum)
{
    if(in == INVALID_DOUBLE)
        return DEFAULT_STR_PRICE;
    double input = abs(in);
    double out = in;

    QString suffix;
    if(1e4<= input && input<1e8)
    {
        out = out/1e4f;
        suffix = QStringLiteral("万");
    }else if(1e8<= input && input<1e12)
    {
        out = out/1e8f;
        suffix = QStringLiteral("亿");
    }else if(1e12<= input)
    {
        out = out/1e12f;
        suffix = QStringLiteral("万亿");
    }
    return QString::number(out,'f',pointNum)+suffix;
}

QString StringUnitFormat::toFormatString(const UINT32 &in, int pointNum)
{
    if(in == INVALID_DOUBLE)
        return DEFAULT_STR_PRICE;
    double input = qAbs(in);
    double out = in;

    QString suffix;
    if(1e4<= input && input<1e8)
    {
        out = out/1e4f;
        suffix = QStringLiteral("万");
    }else if(1e8<= input && input<1e12)
    {
        out = out/1e8f;
        suffix = QStringLiteral("亿");
    }else if(1e12<= input)
    {
        out = out/1e12f;
        suffix = QStringLiteral("万亿");
    }
    return QString::number(out,'f',pointNum)+suffix;
}

}
