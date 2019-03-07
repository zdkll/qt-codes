#include "delegatefunctions.h"


QString  DelegateFunctions::toFormatString(const qulonglong &in)
{
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

    int  eval = log10(out);
    eval = 2-eval<0?0:2-eval;
    return QString::number(out,'f',eval)+suffix;
}

QString  DelegateFunctions::toFormatString(const qlonglong &in)
{
    double out = in;

    qlonglong val = llabs(in);

    QString suffix;
    if(val < 1e4)
        return QString::number(in);
    else if(1e4<= val && val<1e8)
    {
        out = double(in)/1e4;
        suffix = QStringLiteral("万");
    }
    else if(1e8<= val && val<1e12)
    {
        out = double(in)/1e8;
        suffix = QStringLiteral("亿");
    }else if(1e12<= val)
    {
        out = double(in)/1e12;
        suffix = QStringLiteral("万亿");
    }

    int  eval = log10(llabs(out));
    eval = 2-eval<0?0:2-eval;

    return QString::number(out,'f',eval)+suffix;
}

QString  DelegateFunctions::toFormatString(const double &in)
{
    double out = in;

    double val = abs(in);

    QString suffix;
    if(1e4<= val && val<1e8)
    {
        out = out/1e4f;
        suffix = QStringLiteral("万");
    }else if(1e8<= val && val<1e12)
    {
        out = out/1e8f;
        suffix = QStringLiteral("亿");
    }else if(1e12<= val)
    {
        out = out/1e12f;
        suffix = QStringLiteral("万亿");
    }

    int  eval = log10(abs(out));
    eval = 2-eval<0?0:2-eval;
    return QString::number(out,'f',eval)+suffix;
}


QString  DelegateFunctions::stockToFormatString(const qulonglong &in)
{
    double out = double(in)/100.f;

    QString suffix;
    if(out < 1e4)
        return QString::number(out,'f',0);
    else if(1e4<= out && out<1e8)
    {
        out = double(out)/1e4;
        suffix = QStringLiteral("万");
    }
    else if(1e8<= out && out<1e12)
    {
        out = double(out)/1e8;
        suffix = QStringLiteral("亿");
    }else if(1e12<= out)
    {
        out = double(out)/1e12;
        suffix = QStringLiteral("万亿");
    }

    int  eval = log10(out);
    eval = 2-eval<0?0:2-eval;
    return QString::number(out,'f',eval)+suffix;
}

QString  DelegateFunctions::stockToFormatString(const qlonglong &in)
{
    double out = double(in)/100.f;

    double val =  abs(out);

    QString suffix;
    if(val < 1e4)
        return QString::number(out,'f',0);
    else if(1e4<= val && val<1e8)
    {
        out = double(out)/1e4;
        suffix = QStringLiteral("万");
    }
    else if(1e8<= val && val<1e12)
    {
        out = double(out)/1e8;
        suffix = QStringLiteral("亿");
    }else if(1e12<= val)
    {
        out = double(out)/1e12;
        suffix = QStringLiteral("万亿");
    }

    int  eval = log10(labs(out));
    eval = 2-eval<0?0:2-eval;
    return QString::number(out,'f',eval)+suffix;
}

