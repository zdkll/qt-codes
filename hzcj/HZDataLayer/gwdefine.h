#pragma once
#include <QVector>
#include <QMap>
#include <vector>
#include <list>
#include <map>
using namespace std;

//#define DECLARE_MAP(key, value, name)	\
//    typedef map<key, value>	name##MAP;	\
//    typedef map<key, value>::iterator name##MAPITR;  \
//    typedef map<key, value>::const_iterator CONST_##name##MAPITR;

//#define DECLARE_VECTOR(value, name)	\
//    typedef vector<value>  name##VEC;	\
//    typedef vector<value>::iterator name##VECITR;	\
//    typedef vector<value>::const_iterator CONST_##name##VECITR;

#define DECLARE_MAP(key, value, name)	\
    typedef QMap<key, value>	name##MAP;	\
    typedef QMap<key, value>::iterator name##MAPITR;  \
    typedef QMap<key, value>::const_iterator CONST_##name##MAPITR;

#define DECLARE_VECTOR(value, name)	\
    typedef QVector<value>  name##VEC;	\
    typedef QVector<value>::iterator name##VECITR;	\
    typedef QVector<value>::const_iterator CONST_##name##VECITR;

#define DECLARE_LIST(value, name)	\
    typedef list<value>  name##LST;	\
    typedef list<value>::iterator name##LSTITR;		\
    typedef list<value>::const_iterator CONST_##name##LSTITR;

#define VECTOR_MERGE(vectDest, vectorOrg, bClearFirt) \
    if(bClearFirt) \
{\
    vectDest.clear();\
}\
    if(FALSE == vectorOrg.empty())\
{\
    vectDest.insert(vectDest.end(), vectorOrg.begin(), vectorOrg.end());\
}

#define DELETE_PTR(p)	\
    delete p;	\
    p = NULL;
#define DELETE_PTRA(p)	\
    delete[]p;	\
    p = NULL;

#define SIMPLE_FIND_OBJECT(c, o) std::find(c.begin(), c.end(), o)



