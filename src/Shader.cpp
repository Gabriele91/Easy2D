#include <stdafx.h>
#include <RenderContext.h>
#include <Shader.h>
#include <Debug.h>
#include <Table.h>
#include <Config.h>
#include <Application.h>
///////////////////////
using namespace Easy2D;
///////////////////////
#define ENABLE_SHADER
#define cstrlen(s) (sizeof(s)-1)
///////////////////////
#if defined( ENABLE_SHADER ) && !defined(OPENGL_ES2) //OPENGL 2.1
//VERSION
static const char versionGLSL[]=
"#version 120 \n";
//STANDARD UNIFORM
static const char standardUniform[]=
"#define Texture sampler2D                 \n"
"#define e2dProjection gl_ProjectionMatrix \n"
"#define e2dModelView  gl_ModelViewMatrix  \n"
"uniform vec4 e2dAmbientColor;    \n"
"uniform vec4 e2dColor;           \n"
"uniform vec4 e2dViewport;        \n"
"uniform sampler2D e2dTexture0;   \n";

//VERTEX HADER
static const char vertexHeader[]=
"#define  VERTEX                            \n"
"#define  e2dVertex       gl_Vertex         \n"
"#define  e2dNormal       gl_Normal         \n"
"#define  e2dVertexColor  gl_Color          \n"
"#define  e2dTexCoord0   gl_MultiTexCoord0 \n"
"varying  vec4 e2dTextureCoord;             \n";

//FRAGMENT HEADER
static const char fragmentHeader[]=
"#define  FRAGMENT               \n"
"#define  bestp                  \n"
"#define  highp                  \n"
"#define  mediump                \n"
"#define  lowp                   \n"
"varying  vec4 e2dTextureCoord;  \n";

#elif defined( OPENGL_ES2 )

//VERSION
static const char versionGLSL[]=
"#version 100						\n"
"precision mediump float;           \n";
//STANDARD UNIFORM
static const char standardUniform[]=
"#define Texture sampler2D        \n"
"uniform mat4 e2dProjection;      \n"
"uniform mat4 e2dModelView;       \n"
"uniform vec4 e2dAmbientColor;    \n"
"uniform vec4 e2dColor;           \n"
"uniform vec4 e2dViewport;        \n"
"uniform sampler2D e2dTexture0;   \n";

//VERTEX HADER
static const char vertexHeader[]=
"#define  VERTEX                    \n"
"attribute vec4  e2dVertex;         \n"
"attribute vec4  e2dNormal;         \n"
"attribute vec4  e2dVertexColor;    \n"
"attribute vec4  e2dTexCoord0;      \n"
"varying  vec4 e2dTextureCoord;     \n"
"vec4 ftransform()                            \n"
"{                                            \n"
"return e2dProjection*e2dModelView*e2dVertex; \n"
"}                                            \n";


//FRAGMENT HEADER
static const char fragmentHeader[]=
"#define  FRAGMENT               \n"
"varying  vec4 e2dTextureCoord;  \n";

#else
    #error "shader not supported"
#endif

//////////////////////////////
// VERTEX SHADER
static const char vertexMain[]=
"void main()\n"
"{\n"
"  e2dTextureCoord=e2dTexCoord0;\n"
"  #ifdef __DEFAULT_VERTEXT__    \n"
"     gl_Position=ftransform();  \n"
"  #else                         \n"
"     gl_Position=vertex();      \n"
"  #endif                        \n"
"}\n";

static const char fragmentMain[]=
"void main()\n"
"{\n"
"  gl_FragColor=fragment(e2dTexture0,e2dTextureCoord);\n"
"}\n";




Shader::Shader(ResourcesManager<Shader> *rsmr,
               const String& pathfile)
:Resource(rsmr,pathfile)
{
    program  = 0 ;
    vertex   = 0 ;
    fragment = 0 ;
    uProjection = 0 ;
    uModelView = 0 ;
    uAmbientColor = 0 ;
    uColor = 0 ;
    uViewport = 0 ;
    uTex0 = 0 ;
    uCallback=[](Shader&){};
    unSaveUniform=true;
    //is not loaded
    loaded=false;
}
Shader::Shader()
{
    program  = 0 ;
    vertex   = 0 ;
    fragment = 0 ;
    uProjection = 0 ;
    uModelView = 0 ;
    uAmbientColor = 0 ;
    uColor = 0 ;
    uViewport = 0 ;
    uTex0 = 0 ;
    uCallback=[](Shader&){};
    unSaveUniform=true;
    //is not loaded
    loaded=false;
}
Shader::~Shader()
{
    //release resource
    release();
}
//compile utilities
bool Shader::linking()
{
    GLint programOk;
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    
#if defined(OPENGL_ES2)
    //attributes
    glBindAttribLocation(program,ATTRIBUTE_POSITION,"e2dVertex");
    glBindAttribLocation(program,ATTRIBUTE_NORMAL,"e2dNormal");
    glBindAttribLocation(program,ATTRIBUTE_COLOR,"e2dVertexColor");
    glBindAttribLocation(program,ATTRIBUTE_TEXTCOOR,"e2dTexCoord0");
#endif
    
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &programOk);
    if (!programOk)
    {
        //max length
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        //The maxLength includes the NULL character
        String errorLog(' ',maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);
        //print error
        DEBUG_ASSERT_MSG(0,errorLog);
        //delete
        glDeleteProgram(program);
        return false;
    }
    return true;
    
}
bool Shader::compiler(uint shader)
{
    //compiler
    glCompileShader(shader);
    //sttatus compile
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    //get status
    if(isCompiled == GL_FALSE)
    {
        //max length
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        
        //The maxLength includes the NULL character
        String errorLog(' ',maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        
        //print error
        DEBUG_ASSERT_MSG(0,errorLog);
        
        //Provide the infolog in whatever manor you deem best.
        //Exit with failure.
        glDeleteShader(shader); //Don't leak the shader.
        //reset shader id
        shader=0;
        //return false
        return false;
    }
    return true;

}
//uniform callback
void Shader::setUniformCallback(DFUNCTION<void(Shader&)> callback)
{
    uCallback=callback;
}
//load
void Shader::load(const String& ps)
{
    std::vector<String> defines;
    load("#define __DEFAULT_VERTEXT__ \n",ps,defines);
}

void Shader::load(const String& ps,
                  const std::vector<String>& defines)
{
    load("#define __DEFAULT_VERTEXT__ \n",ps,defines);
}

void Shader::load(const String& vs,
                  const String& ps)
{
    std::vector<String> defines;
    load(vs,ps,defines);
}

void Shader::load(const String& vs,
                  const String& ps,
                  const std::vector<String>& defines)
{
    //var declaretion
    String alldefines;
    //define list
    if(defines.size()) //vs11
    for(const String& define:defines)
    {
        alldefines+="#define "+define+"\n";
    };
    //create a vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    {
        const char* sources[]=
        {
            versionGLSL,
            alldefines.c_str(),
            standardUniform,
            vertexHeader,
            "#line 0\n",
            vs.c_str(),
            vertexMain
        };
        GLint lengths[]=
        {
            cstrlen(versionGLSL),
            (GLint)alldefines.size(),
            //uniform before define header in OGL
            cstrlen(standardUniform),
            cstrlen(vertexHeader),
            cstrlen("#line 0\n"),
            (GLint)vs.size(),
            cstrlen(vertexMain)
        };
        //const source code
		const GLchar** constSources=(const GLchar**)sources;
		//add code
        glShaderSource((GLint)vertex,7,constSources,lengths);
    }
    //compiler
    compiler(vertex);
    
    //create fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    {
        const char* sources[]=
        {
            versionGLSL,
            alldefines.c_str(),
            fragmentHeader,
            standardUniform,
            "#line 0\n",
            ps.c_str(),
            fragmentMain
        };
        GLint lengths[]=
        {
            cstrlen(versionGLSL),
            (GLint)alldefines.size(),
            //define precision float before uniforms in OGLES
            cstrlen(fragmentHeader),
            cstrlen(standardUniform),
            cstrlen("#line 0\n"),
            (GLint)ps.size(),
            cstrlen(fragmentMain)
        };
        //const source code
		const GLchar** constSources=(const GLchar**)sources;
        //add code
        glShaderSource((GLint)fragment,7,constSources,lengths);
    }
    //compiler
    compiler(fragment);
    
    //linking
    linking();

}
/************* *************
        PARSER EFFECT
 ************* *************/

static inline bool isAlphaChar(const char* source)
{
    return  (*(source))==' ' ||
    (*(source))=='\t'||
    (*(source))=='\r'||
    (*(source))=='\n';
}
static inline bool isStartLComment(const char* source)
{
    return (*source)=='/' && (*(source+1)=='/');
}
static inline bool isStartMLComment(const char* source)
{
    return (*source)=='/' && (*(source+1)=='*');
}
static inline bool isEndMLComment(const char* source)
{
    return (*source)=='*' && (*(source+1)=='/');
}
static inline void skipSpaceAndComments(char*& source,int line)
{
    do
    {
        //jump space
        if(isAlphaChar(source))
        {
            //count line
            if((*(source))=='\n')
                ++line;
            //skip char
            ++source;
        }
        //jump line comment
        else if(isStartLComment(source))
        {
            while (true)
            {
                //count line
                if((*(source))=='\n')
                {
                    //count
                    ++line;
                    //skip char
                    ++source;
                    //exit
                    break;
                }
                else if((*(source))=='\0')
                {
                    break;
                }
                //skip char
                ++source;
            }
        }
        //jump multi line comment
        else if(isStartMLComment(source))
        {
            while (true)
            {
                //count line
                if((*(source))=='\n')
                {
                    ++line;
                }
                else if(isEndMLComment(source))
                {
                    //skip chars * and /
                    source+=2;
                    //exit
                    break;
                }
                else if((*(source))=='\0')
                {
                    break;
                }
                //skip char
                ++source;
            }
        }
        else
        {
            return;
        }
    }while(true);
    
}
static inline void skipGLSLAndComments(char*& source,int line)
{
    do
    {
        //jump line comment
        if(isStartLComment(source))
        {
            while (true)
            {
                //count line
                if((*(source))=='\n')
                {
                    //count
                    ++line;
                    //skip char
                    ++source;
                    //exit
                    break;
                }
                else if((*(source))=='\0')
                {
                    break;
                }
                //skip char
                ++source;
            }
        }
        //jump multi line comment
        else if(isStartMLComment(source))
        {
            while (true)
            {
                //count line
                if((*(source))=='\n')
                {
                    ++line;
                }
                else if(isEndMLComment(source))
                {
                    //skip chars * and /
                    source+=2;
                    //exit
                    break;
                }
                else if((*(source))=='\0')
                {
                    break;
                }
                //skip char
                ++source;
            }
        }
        //jump GLSL
        else if((*source)!='@')
        {
            //count line
            if((*(source))=='\n')
                ++line;
            //skip char
            ++source;
        }
        else
        {
            return;
        }
    }while((*source)!='\0');
    
}
//add shader source
static bool addSourceFromEffect(char*& source,String& shader,int& line)
{
    if(shader.size()) return false;
    //start line
    shader+="#line "+String::toString(line+1)+"\n";
    //parser
    const char* start=source;
    skipGLSLAndComments(source,line);
    //save char
    char tmpchar;
    //swap
    tmpchar=*source;
    (*source)='\0';
    //add into string
    shader+=String(start);
    //reset char
    (*source)=tmpchar;
    //true
    return true;
}

/************* *************/

//parser effect
bool parserEffect(char* source,
                  String& errors,
                  String& vertex,
                  String& fragment)
{
    //start count line
    int line=0;
    //init values
    errors="";
    vertex="";
    fragment="";
    //skip space
    skipSpaceAndComments(source,line);
    //is a...
    #define isA(x) strncmp(source,x,sizeof(x))>=0
    //get vertex
    while((*source)!='\0')
    {
        if(isA("@VERTEX"))
        {
            source+=sizeof("@VERTEX");
            if(!addSourceFromEffect(source,vertex,line))
            {
                errors+="Parsing Effect, vertex shader olready added\n";
                return false;
            }
        }
        else if(isA("@FRAGMENT"))
        {
            source+=sizeof("@FRAGMENT");
            if(!addSourceFromEffect(source,fragment,line))
            {
                errors+="Parsing Effect, fragment shader olready added\n";
                return false;
            }
        }
    }
    //invalid effect
    if(!fragment.size())
    {
        errors+="Parsing Effect, fragment shader is requaired\n";
        return false;
    }
    //return
    return  true;
}

/************* *************/

//load methods
bool Shader::load()
{
    //DEBUG_ASSERT(!program); force reload?
    //load shader
    if(rpath.getExtension()=="e2d")
    {
        //load table
        Table texInfo(NULL,rpath);
        texInfo.load();
        //shaders
        String vertex;
        String fragment;
        //load source
        vertex=texInfo.getString("vertex");
        fragment=texInfo.getString("fragment");
        //compile
        if(vertex.size())
            load(vertex, fragment);
        else
            load(fragment);
    }
    else if(rpath.getLeftExtension()=="fx.glsl")
    {
        //load data
        size_t len=0;
        void*  ptr=nullptr;
        Application::instance()->loadData(rpath, ptr, len);
        //parsing
        String errors,vertex,fragment;
        //parsing
        bool result=parserEffect((char*)ptr,errors,vertex,fragment);
        //free
        free(ptr);
        //errors
        DEBUG_ASSERT_MSG(result,errors);
        //compile
        if(vertex.size())
            load(vertex, fragment);
        else
            load(fragment);
    }
    else
    {
        //is not loaded
        loaded=false;
        return false;
    }
    //is loaded
    loaded=true;
    //return
    return loaded;
}
bool Shader::unload()
{
    DEBUG_ASSERT(program);
    glDeleteProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    //is not loaded
    loaded=false;
    //return
    return !loaded;
}
//get default uniforms
void Shader::getDefaultUniform()
{
    //standard uniforms
    if(unSaveUniform)
    {
        
#if defined(OPENGL_ES2)
        //gles extra uniform
        uProjection =  uniformID("e2dProjection");
        uModelView  =  uniformID("e2dModelView");
#endif
        uAmbientColor = uniformID("e2dAmbientColor");
        uColor        = uniformID("e2dColor") ;
        uViewport     = uniformID("e2dViewport") ;
        uTex0         = uniformID("e2dTexture0") ;
        //flag
        unSaveUniform=false;
    }
}
//bind buffer
void Shader::bind()
{
    //enable program
    RenderContext::enableProgram(program);
    //default uniforms
    getDefaultUniform();
    //update uniform
    updateUniform();
}
//update uniforms
void Shader::updateUniform()
{
    //bind parameters
    updateStandardUniform();
    //bind constom paramers
    uCallback(*this);
}
//update standard uniforms
void Shader::updateStandardUniform()
{
    //bind parameters
#if defined(OPENGL_ES2)
    if(uProjection>=0)
        uniform(uProjection,RenderContext::getGlobalProjection());//global projection
    if(uModelView>=0)
        uniform(uModelView,RenderContext::getModelView());
#endif
    if(uAmbientColor>=0)
        uniform(uAmbientColor,RenderContext::getAmbientColor().toNormalize());
    if(uColor>=0)
        uniform(uColor,RenderContext::getColor().toNormalize());
    if(uViewport>=0)
        uniform(uViewport,RenderContext::getViewport());
    uniformTexture(uTex0,/*RenderContext::currentTexture()*/ 0);
}
//uniform name
void Shader::uniform(const String& name,int v){ glUniform1i(glGetUniformLocation(program, name), v); }
void Shader::uniform(const String& name,float v){ glUniform1f(glGetUniformLocation(program, name), v); }
void Shader::uniform(const String& name,const Vector2D& v){ glUniform2fv(glGetUniformLocation(program, name), 1, &v.x); }
void Shader::uniform(const String& name,const Vector3D& v){ glUniform3fv(glGetUniformLocation(program, name), 1, &v.x); }
void Shader::uniform(const String& name,const Vector4D& v){ glUniform4fv(glGetUniformLocation(program, name), 1, &v.x); }
void Shader::uniform(const String& name,const Matrix4x4& v){ glUniformMatrix4fv(glGetUniformLocation(program, name), 1, false, v.entries); }

void Shader::uniform(const String& name,const std::vector<int>& v){ glUniform1iv(glGetUniformLocation(program, name), v.size(), &v[0]); }
void Shader::uniform(const String& name,const std::vector<float>& v){ glUniform1fv(glGetUniformLocation(program, name), v.size(), &v[0]); }
void Shader::uniform(const String& name,const std::vector<Vec2>& v){ glUniform2fv(glGetUniformLocation(program, name), v.size(), &v[0].x); }
void Shader::uniform(const String& name,const std::vector<Vec3>& v){ glUniform3fv(glGetUniformLocation(program, name), v.size(), &v[0].x); }
void Shader::uniform(const String& name,const std::vector<Vec4>& v){ glUniform4fv(glGetUniformLocation(program, name), v.size(), &v[0].x); }
void Shader::uniform(const String& name,const std::vector<Mat4>& v){ glUniformMatrix4fv(glGetUniformLocation(program, name), v.size(), false, v[0].entries); }

void Shader::uniformTexture(const String& name,const uint v){ glUniform1i(glGetUniformLocation(program, name), v); }

//uniform id
int Shader::uniformID(const String& name){ return glGetUniformLocation(program, name); }
void Shader::uniform(int uid,int v){ glUniform1i(uid, v); }
void Shader::uniform(int uid,float v){ glUniform1f(uid, v); }
void Shader::uniform(int uid,const Vector2D& v){ glUniform2fv(uid, 1, &v.x); }
void Shader::uniform(int uid,const Vector3D& v){ glUniform3fv(uid, 1, &v.x); }
void Shader::uniform(int uid,const Vector4D& v){ glUniform4fv(uid, 1, &v.x); }
void Shader::uniform(int uid,const Matrix4x4& v){ glUniformMatrix4fv(uid, 1, false, v.entries); }

void Shader::uniform(int uid,const std::vector<int>& v){ glUniform1iv(uid, v.size(), &v[0]); }
void Shader::uniform(int uid,const std::vector<float>& v){ glUniform1fv(uid, v.size(), &v[0]); }
void Shader::uniform(int uid,const std::vector<Vec2>& v){ glUniform2fv(uid, v.size(), &v[0].x); }
void Shader::uniform(int uid,const std::vector<Vec3>& v){ glUniform3fv(uid, v.size(), &v[0].x); }
void Shader::uniform(int uid,const std::vector<Vec4>& v){ glUniform4fv(uid, v.size(), &v[0].x); }
void Shader::uniform(int uid,const std::vector<Mat4>& v){ glUniformMatrix4fv(uid, v.size(), false, v[0].entries); }

void Shader::uniformTexture(int uid,const uint v){ glUniform1i(uid, v); }





