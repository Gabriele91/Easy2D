#include <stdafx.h>
#include <Table.h>
#include <Debug.h>
#include <Application.h>
#include <ResourcesManager.h>
///////////////////////
using namespace Easy2D;
///////////////////////
//statics
Table Table::NONE;
Table::Binary Table::Binary::NONE;

//Table class
Table::Table(ResourcesManager<Table> *rsmr,
             const String& pathfile)
    :Resource(rsmr,pathfile)
    ,index(0)
{
    reloadable=true;
}

Table::Table(const Table& cptable)
    :Resource(cptable.ptrResources,
              cptable.rpath)
    ,index(cptable.index)
{
    //resource copy
    reloadable=cptable.reloadable;
    loaded=cptable.loaded;
    //table copy
    for(auto it:cptable)
        table[it.first]=it.second->clone();
    //
}

Table::Table()
    :Resource(NULL,"")
    ,index(0)
{
    reloadable=false;
}
Table::~Table()
{
    //clear all table
    index=0;
    for(auto& value : *this)
        delete value.second;
    table.clear();
    //release resource
    release();
}
bool Table::load()
{
    //can't load this resource
    DEBUG_ASSERT(isReloadable());
    //
    if(isReloadable())
    {
        //get raw file
        void *data=NULL;
        size_t len=0;
        Application::instance()->loadData(rpath,data,len);
        //deserialize
        deserialize(String((const char*)data));
        //get errors
        if(getDeserializeErros().size()>0)
        {
            DEBUG_ASSERT_MSG(0,"Table error:"
                             "invalid parsing:\n"+
                             getDeserializeErros());
        }
        //free memory
        free(data);
        //
        loaded=true;
    }
    return true;
}
bool Table::unload()
{
    //clear all
    index=0;
    for(auto& value : *this)
        delete value.second;
    table.clear();
    //
    loaded=false;
    //
    return true;
}

/* ENUM TOKEN */
enum Token
{
    TK_NAME,
    TK_STRING,
    TK_NUMBER,

    TK_TABLE_START,
    TK_TABLE_END,

    TK_VECTOR_START,
    TK_VECTOR_END,

    TK_BINARY_START,
    TK_BINARY_MIDDLE,
    TK_BINARY_END,

    TK_EQUALS,
    TK_SPACE,
    TK_INCLUDE_START,
    TK_INCLUDE_END,
    TK_NONE
};
/* PARSE FUNCTIONS */
DFORCEINLINE bool isStartName(char c)
{
    return 	 (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
DFORCEINLINE bool isCharName(char c)
{
    return 	 (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c=='_' ;
}
DFORCEINLINE bool isStartNumber(char c)
{
    return 	 (c >= '0' && c <= '9') || (c == '-') || (c == '.');
}
DFORCEINLINE bool isStartTable(char c)
{
    return (c == '{');
}
DFORCEINLINE bool isEndTable(char c)
{
    return (c == '}');
}
DFORCEINLINE bool isStartBinary(char c)
{
    return (c == '@');
}
DFORCEINLINE bool isMiddleBinary(char c)
{
    return (c == '[');
}
DFORCEINLINE bool isEndBinary(char c)
{
    return (c == ']');
}
DFORCEINLINE bool isStartVector(char c)
{
    return (c == '(');
}
DFORCEINLINE bool isEndVector(char c)
{
    return (c == ')');
}
DFORCEINLINE bool isEquals(char c)
{
    return (c == '=');
}
DFORCEINLINE bool isStartString(char c)
{
    return 	 (c == '"');
}
DFORCEINLINE bool isSpace(char c)
{
    return 	 c == ' ' || c == '\t' || c == '\r' || c == '\n';
}
DFORCEINLINE bool isLineComment(const char* c)
{
    return (*c)=='/' && (*(c+1))=='/';
}
DFORCEINLINE bool isStartMultyLineComment(const char* c)
{
    return (*c)=='/' && (*(c+1))=='*';
}
DFORCEINLINE bool isEndMultyLineComment(const char* c)
{
    return (*c)=='*' && (*(c+1))=='/';
}
DFORCEINLINE bool isIncludeStart(char c)
{
    return (c=='<');
}
DFORCEINLINE bool isIncludeEnd(char c)
{
    return (c=='>');
}
DFORCEINLINE Token getToken(char c)
{

    if(isStartName(c)) return TK_NAME;
    if(isStartString(c)) return TK_STRING;
    if(isStartNumber(c)) return TK_NUMBER;

    if(isStartTable(c)) return TK_TABLE_START;
    if(isEndTable(c)) return TK_TABLE_END;

    if(isStartVector(c)) return TK_VECTOR_START;
    if(isEndVector(c)) return TK_VECTOR_END;

    if(isStartBinary(c)) return TK_BINARY_START;
    if(isMiddleBinary(c)) return TK_BINARY_MIDDLE;
    if(isEndBinary(c)) return TK_BINARY_END;

    if(isEquals(c)) return TK_EQUALS;
    if(isSpace(c)) return TK_SPACE;
    if(isIncludeStart(c)) return TK_INCLUDE_START;
    if(isIncludeEnd(c)) return 	TK_INCLUDE_END;

    return TK_NONE;
}

DFORCEINLINE bool parseNumber(const char* in,float& out,const char** cout=NULL)
{
    const char* tmp=in;
    if(!isStartNumber(*in)) return false;

    out=(float)std::strtod(in,(char**)&(in));
    (*cout)=in;

    return tmp!=in;
}
DFORCEINLINE bool parseCString(int& countln,const char* in,String& out,const char** cout=NULL)
{
    const char *tmp=in;
    out="";
    //if(jumpSpace(in,&tmp)==FIND_GOOD){//[  "...."]
    if((*tmp)=='\"')  //["...."]
    {
        ++tmp;  //[...."]
        while((*tmp)!='\"'&&(*tmp)!='\n')
        {
            if((*tmp)=='\\') //[\.]
            {
                ++tmp;  //[.]
                switch(*tmp)
                {
                case 'n':
                    out+='\n';
                    break;
                case 't':
                    out+='\t';
                    break;
                case 'b':
                    out+='\b';
                    break;
                case 'r':
                    out+='\r';
                    break;
                case 'f':
                    out+='\f';
                    break;
                case 'a':
                    out+='\a';
                    break;
                case '\\':
                    out+='\\';
                    break;
                case '?':
                    out+='\?';
                    break;
                case '\'':
                    out+='\'';
                    break;
                case '\"':
                    out+='\"';
                    break;
                case '\n': /* jump unix */
                    ++countln;
                    break;
                case '\r': /* jump mac */
                    ++countln;
                    if((*(tmp+1))=='\n') ++tmp; /* jump window (\r\n)*/
                    break;
                default:
                    return true;
                    break;
                }
            }
            else
            {
                if((*tmp)!='\0') out+=(*tmp);
                else return false;
            }
            ++tmp;//next char
        }
        if((*tmp)=='\n') return false;
        if(cout) (*cout)=tmp+1;
        return true;
    }
    //}
    return false;
}
DFORCEINLINE bool parseName(const char* in,String& out,const char** cout=NULL)
{
    if (!isStartName(*in)) return false;
    out+=*in;
    ++in;
    while(isCharName(*in))
    {
        out+=*in;
        ++in;
    }
    (*cout)=in;
    return true;
}

DFORCEINLINE void skeepLineComment(int& cntN,const char** inout)
{
    if(isLineComment(*inout))
    {
        while(*(*inout)!=EOF &&
                *(*inout)!='\0'&&
                *(*inout)!='\n')
            ++(*inout);
    }
}
DFORCEINLINE void skeepMultyLineComment(int& cntN,const char** inout)
{
    if(isStartMultyLineComment(*inout))
    {
        while(*(*inout)!=EOF &&
                *(*inout)!='\0'&&
                !isEndMultyLineComment(*inout))
        {
            cntN+=(*(*inout))=='\n';
            ++(*inout);
        }
        if((*(*inout))=='*') ++(*inout);
    }
}
DFORCEINLINE void skeepSpaceAndComment(int& cntN,const char** inout)
{
    while(isSpace(*(*inout))||
            isLineComment(*inout)||
            isStartMultyLineComment(*inout))
    {
        skeepLineComment(cntN,inout);
        skeepMultyLineComment(cntN,inout);
        cntN+=(*(*inout))=='\n';
        ++(*inout);
    }
}

DFORCEINLINE bool parseInclude(int& countln,const char* in,String& out,const char** cout=NULL)
{
    if (!isIncludeStart(*in)) return false;
    ++in;//jmp <
    skeepSpaceAndComment(countln,&in);//jump space
    while(!isIncludeEnd(*in)&&(*in)!='\0'&&(*in)!=EOF)
    {
        countln+=(*in)=='\n';
        out+=*in;
        ++in;
    }
    (*cout)=in;
    return isIncludeEnd(*in);
}

int Table::deserialize(const String& intextfile)
{
    return __deserialize(intextfile);
}
String Table::serialize(int countspace) const
{
    return __serialize(countspace);
}
int Table::__deserialize(const String& intextfile,int* lenRead,unsigned int* stline)
{
    //
    const char *prtC=intextfile.c_str();
    int countKey=-1;
    int cntEL=(int)( stline!=NULL ? *stline : 1 );
    //get {
    skeepSpaceAndComment(cntEL,&prtC);
    bool thiIsStarted=isStartTable(*prtC);
    if(!thiIsStarted)
    {
        dErrors.push(cntEL,*prtC,"table: token '{' not found");
        return false;
    }
    else ++prtC; //jmp {

    //tmp values
    bool jmpNewKey=false;
    Table::KeyTable key(0);
    //start parse...
    while(*prtC != EOF && *prtC!='\0')
    {
        /* temp values */
        if(!jmpNewKey)
        {
            ++countKey;
            key=Table::KeyTable(countKey);
        }
        else
        {
            jmpNewKey=false;
        }
        //Tmps
        String str;
        String path;
        float fl;
        float vectorsTmp[16];
        int i=0;
        void *binary;
        Table *tmp;
        /* switch-case */
        switch (getToken(*prtC))
        {
        case TK_NAME:
            if(!parseName(prtC,str,&prtC))
            {
                dErrors.push(cntEL,*prtC,"name attribute: not valid");
                return false;
            }
            //set key
            key=Table::KeyTable(str);
            jmpNewKey=true;
            //
            //search =
            skeepSpaceAndComment(cntEL,&prtC);
            if(!isEquals(*prtC))
            {
                dErrors.push(cntEL,*prtC,"name attribute: token '=' not found");
                return false;
            }
            else ++prtC; //jmp =

            break;
        case TK_STRING:
            //parse string
            if(!parseCString(cntEL,prtC,str,&prtC))
            {
                dErrors.push(cntEL,*prtC,"string: not valid");
                return false;
            }
            //search =
            skeepSpaceAndComment(cntEL,&prtC);
            if(isEquals(*prtC))  //if found this is a key
            {
                //set key
                key=Table::KeyTable(str);
                jmpNewKey=true;
                //
                ++prtC; //jmp =
            }
            else
                setPt(key,str);

            break;
        case TK_NUMBER:
            if(!parseNumber(prtC,fl,&prtC))
            {
                dErrors.push(cntEL,*prtC,"float number: not valid");
                return false;
            }
            setPt(key,fl);
            break;
        case TK_TABLE_START:
            //create table
            tmp=&createTablePt(key);
            //deserialize
            fl=(float)(tmp->__deserialize(prtC,&i,(unsigned int*)(&cntEL)));
            //errors?
            if(tmp->dErrors.hasErrors())
            {
                dErrors.push(cntEL,*prtC,"error sub table: \n"+tmp->getDeserializeErros());
                return false;
            }
            //set this path
            tmp->rpath=rpath;
            //set name
            if(key.isString())
                tmp->name=key.string();
            //save lines count
            cntEL=(int)fl;
            prtC+=i;
            break;
        case TK_TABLE_END:
            ++prtC; //jmp }
            if(lenRead)
                (*lenRead)=prtC-intextfile.c_str();
            return cntEL;
            /* end parse */
            break;
        case TK_VECTOR_START:
            ++prtC; //jmp (
            //parse values
            skeepSpaceAndComment(cntEL,&prtC);
            while(parseNumber(prtC,fl,&prtC))
            {
                vectorsTmp[i]=fl;
                ++i;
                skeepSpaceAndComment(cntEL,&prtC);
            }
            if(i==2) setPt(key,Vec2(vectorsTmp[0],vectorsTmp[1]));
            else if(i==3) setPt(key,Vec3(vectorsTmp[0],vectorsTmp[1],vectorsTmp[2]));
            else if(i==4) setPt(key,Vec4(vectorsTmp[0],vectorsTmp[1],vectorsTmp[2],vectorsTmp[3]));
            else if(i==16) setPt(key,Matrix4x4(vectorsTmp));
            else
            {
                dErrors.push(cntEL,*prtC, "vector: number parameter are not valid");
                return false;
            }

            skeepSpaceAndComment(cntEL,&prtC);
            if(!isEndVector(*prtC))
            {
                dErrors.push(cntEL,*prtC,"vector: token ')' not found");
                return false;
            }
            else ++prtC; //jmp )

            break;
        case TK_INCLUDE_START:
            //<
            if(!parseInclude(cntEL,prtC,str,&prtC))
            {
                dErrors.push(cntEL,*prtC,"include: token '>' not found");
                return false;
            }
            //create table
            tmp=&createTablePt(key);
            //set path
            str=rpath.getDirectory()+"/"+str;
            tmp->rpath=Utility::Path(str);
            //set resource group
            tmp->ptrResources=this->ptrResources;
            //is relodable
            tmp->reloadable=true;
            //load file
            tmp->load();
            //set name
            if(key.isString())
                tmp->name=key.string();
            //get errors
            if(tmp->dErrors.hasErrors())
            {
                dErrors.push(cntEL,*prtC,"error sub table("+
                             tmp->rpath.getPath()+
                             ") :\n"+
                             tmp->getDeserializeErros());
                return false;
            }
            //jmp >
            ++prtC;

            break;
        case TK_BINARY_START:
            ++prtC; //jmp @
            //parse values
            skeepSpaceAndComment(cntEL,&prtC);
            parseNumber(prtC,fl,&prtC);
            binary=(void*)malloc( (int)fl );
            //find start binary
            skeepSpaceAndComment(cntEL,&prtC);
            if(!isMiddleBinary(*prtC))
            {
                dErrors.push(cntEL,*prtC,"binary: token '[' not found");
                return false;
            }
            else ++prtC; //jmp [
            //read
            memcpy(binary,prtC,(int)fl);
            //find end binary
            skeepSpaceAndComment(cntEL,&prtC);
            if(!isEndBinary(*prtC))
            {
                dErrors.push(cntEL,*prtC,"binary: token ']' not found");
                return false;
            }
            else ++prtC; //jmp ]
            //save
            setPt(key,binary,(int)fl,true);
            break;

        case TK_SPACE:
            skeepSpaceAndComment(cntEL,&prtC);
            jmpNewKey=true;
            break;

        /* ERRORs */
        case TK_BINARY_MIDDLE:
        case TK_BINARY_END:
        case TK_VECTOR_END:
        case TK_EQUALS:
        case TK_NONE:
        default:
            dErrors.push(cntEL,*prtC,"token not valid");
            return false;
            break;
        }
    }
    dErrors.push(cntEL,*prtC,"table: token '}' not found");
    return false;
}
String Table::__serialize(int countSpace,bool havename) const
{
    //
    String lspace(' ',countSpace);
    //open table
    String outtextfile(/*( havename? String("") :lspace )+*/ " {\n");
    for(auto value :  *this )
    {

        int supSpace=1;
        outtextfile+=lspace;

        if(value.first.isString())
        {
            if(value.first.string().find(" ")==-1)
            {
                outtextfile+=value.first.string()+" = ";
                supSpace+=(value.first.string().size()+3);
            }
            else
            {
                outtextfile+='"'+value.first.string()+"\" = ";
                supSpace+=(value.first.string().size()+5);
            }
        }

        switch (value.second->type)
        {
        case Easy2D::Table::FLOAT:
            outtextfile+=String::toString(value.second->get<float>())+"\n";
            break;
        case Easy2D::Table::VECTOR2D:
            outtextfile+=value.second->get<Vec2>().toString();
            break;
        case Easy2D::Table::VECTOR3D:
            outtextfile+=value.second->get<Vec3>().toString();
            break;
        case Easy2D::Table::VECTOR4D:
            outtextfile+=value.second->get<Vec4>().toString();
            break;
        case Easy2D::Table::MATRIX4X4:
            outtextfile+=value.second->get<Matrix4x4>().toString("(",
                         " ",
                         "\n"+
                         String(' ',countSpace+supSpace),
                         ")\n");
            break;
        case Easy2D::Table::STRING:
            outtextfile+=String::toString('\"')+value.second->get<String>()+"\"\n";
            break;
        case Easy2D::Table::BINARY:
            //to do
            break;
        case Easy2D::Table::TABLE:
            outtextfile+=value.second->get<Table>().__serialize(countSpace+supSpace,value.first.isString());
            break;
        default:
            break;
        }

    }

    return outtextfile+lspace+"}\n";
}

