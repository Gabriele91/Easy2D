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
#define cmpCStr(s,x) (std::strncmp(s,x,cstrlen(x))>=0)
///////////////////////
#if defined( ENABLE_SHADER ) && !defined(OPENGL_ES2) //OPENGL 2.1
//VERSION
static const char versionGLSL[]=
"#version 120            \n"
"#define GLSL            \n"
"#define OPENGL          \n"
"#define ONLY_GL(x)   x  \n"
"#define ONLY_GLES(x)    \n";
//STANDARD UNIFORM
static const char standardUniform[]=
"#define Texture sampler2D                 \n"
"#define e2dProjection gl_ProjectionMatrix \n"
"#define e2dModelView  gl_ModelViewMatrix  \n"
"uniform mat4 e2dView;            \n"
"uniform mat4 e2dModel;           \n"
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
"#define GLSL_ES					\n"
"#define OPENGL_ES					\n"
"#define ONLY_GL(x)                 \n"
"#define ONLY_GLES(x) x             \n"
"precision mediump float;           \n";
//STANDARD UNIFORM
static const char standardUniform[]=
"#define Texture sampler2D        \n"
"uniform mat4 e2dProjection;      \n"
"uniform mat4 e2dModelView;       \n"
"uniform mat4 e2dView;            \n"
"uniform mat4 e2dModel;           \n"
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

/*********************************************************/

void UniformValue::free()
{
	switch (type)
	{
	case U_VEC3: value.v3.~unique_ptr(); break;
	case U_VEC4: value.v4.~unique_ptr(); break;
	case U_MAT4: value.m4.~unique_ptr(); break;
	default: break;
	}
}

//destructor
UniformValue::~UniformValue()
{
	free();
}

//costructor
UniformValue::UniformValue()
{
	type = U_NONE;
}

UniformValue::UniformValue(const UniformValue& uv)
{
	type = U_NONE;
	switch (uv.type)
	{
		case U_INT:   set(uv.value.i); break;
		case U_FLOAT: set(uv.value.f); break;
		case U_VEC2:  set(uv.value.v2); break;
		case U_VEC3:  set(std::unique_ptr<Vec3>{new Vec3(*uv.value.v3)}); break;
		case U_VEC4:  set(std::unique_ptr<Vec4>{new Vec4(*uv.value.v4)}); break;
		case U_MAT4:  set(std::unique_ptr<Mat4>{new Mat4(*uv.value.m4)}); break;
		default: break;
	}
}

//all type costructors
UniformValue::UniformValue(int i)
{
	free();
	value.i = i;
	type = U_INT;
}

UniformValue::UniformValue(float f)
{
	free();
	value.f = f;
	type = U_FLOAT;
}

UniformValue::UniformValue(const Vec2& v2)
{
	free();
	value.v2 = v2;
	type = U_VEC2;
}

UniformValue::UniformValue(const Vec3& v3)
{
	free();
	value.v3 = std::unique_ptr<Vec3>{new Vec3(v3)};
	type = U_VEC3;
}

UniformValue::UniformValue(const Vec4& v4)
{
	free();
	value.v4 = std::unique_ptr<Vec4>{ new Vec4(v4) };
	type = U_VEC4;
}

UniformValue::UniformValue(const Mat4& m4)
{
	free();
	value.m4 = std::unique_ptr<Mat4>{ new Mat4(m4) };
	type = U_MAT4;
}

UniformValue::UniformValue(std::unique_ptr<Vec3> v3)
{
	free();
	new (&value.v3) std::unique_ptr<Vec3>(std::move(v3));
	type = U_VEC3;
}

UniformValue::UniformValue::UniformValue(std::unique_ptr<Vec4> v4)
{
	free();
	new (&value.v4) std::unique_ptr<Vec4>(std::move(v4));
	type = U_VEC4;
}

UniformValue::UniformValue(std::unique_ptr<Mat4> m4)
{
	free();
	new (&value.m4) std::unique_ptr<Mat4>(std::move(m4));
	type = U_MAT4;
}

//set
void UniformValue::set(int i)
{
	free();
	value.i = i;
	type = U_INT;
}

void UniformValue::set(float f)
{
	free();
	value.f = f;
	type = U_FLOAT;
}

void UniformValue::set(const Vec2& v2)
{
	free();
	value.v2 = v2;
	type = U_VEC2;
}

void UniformValue::set(const Vec3& v3)
{
	free();
	value.v3 = std::unique_ptr<Vec3>{ new Vec3(v3) };
	type = U_VEC3;
}

void UniformValue::set(const Vec4& v4)
{
	free();
	value.v4 = std::unique_ptr<Vec4>{ new Vec4(v4) };
	type = U_VEC4;
}

void UniformValue::set(const Mat4& m4)
{
	free();
	value.m4 = std::unique_ptr<Mat4>{ new Mat4(m4) };
	type = U_MAT4;
}

void UniformValue::set(std::unique_ptr<Vec3> v3)
{
	free();
	new (&value.v3) std::unique_ptr<Vec3>(std::move(v3));
	type = U_VEC3;
}

void UniformValue::set(std::unique_ptr<Vec4> v4)
{
	free();
	new (&value.v4) std::unique_ptr<Vec4>(std::move(v4));
	type = U_VEC4;
}

void UniformValue::set(std::unique_ptr<Mat4> m4)
{
	free();
	new (&value.m4) std::unique_ptr<Mat4>(std::move(m4));
	type = U_MAT4;
}
//uniform value
void UniformValue::uniform(int uid)
{
	switch (type)
	{
		case U_INT: glUniform1i(uid, value.i); break;
		case U_FLOAT: glUniform1f(uid, value.f); break;
		case U_VEC2: glUniform2fv(uid, 1, &value.v2.x); break;
		case U_VEC3: glUniform3fv(uid, 1, &value.v3->x); break;
		case U_VEC4: glUniform4fv(uid, 1, &value.v4->x); break;
		case U_MAT4: glUniformMatrix4fv(uid, 1, false, value.m4->entries); break;
		default: break;
	}
}
//get type
UniformValue::UniformType UniformValue::getType()
{
	return type;
}
//cast
UniformValue::operator int()
{
	return value.i;
}
UniformValue::operator float()
{
	return value.i;
}
UniformValue::operator Vec2()
{
	return value.v2;
}
UniformValue::operator Vec3()
{
	if (type != U_VEC3) return Vec3();
	return *value.v3;
}
UniformValue::operator Vec4()
{
	if (type != U_VEC4) return Vec4();
	return *value.v4;
}
UniformValue::operator Mat4()
{
	if (type != U_MAT4) return Mat4();
	return *value.m4;
}
/*********************************************************/


Shader::Shader(ResourcesManager<Shader> *rsmr,
               const String& pathfile)
:Resource(rsmr,pathfile)
{
    program  = 0 ;
    vertex   = 0 ;
    fragment = 0 ;
    //is not loaded
    loaded=false;
}
Shader::Shader()
{
    program  = 0 ;
    vertex   = 0 ;
    fragment = 0 ;
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
        String errorLog(maxLength, ' ');
        glGetProgramInfoLog(program, maxLength, &maxLength, (char*)errorLog.data());
        //print error
        DEBUG_ASSERT_MSG(0,errorLog);
        //delete
        glDeleteProgram(program);
        return false;
    }
    //build uniforms
    buildUniform();
    //return true ..
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
        String errorLog(maxLength, ' ');
        glGetShaderInfoLog(shader, maxLength, &maxLength, (char*)errorLog.data());
        
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
void Shader::setUniformCallback(Function<void(Shader&)> callback)
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
            alldefines.cStr(),
            standardUniform,
            vertexHeader,
            "#line 0\n",
            vs.cStr(),
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
            alldefines.cStr(),
            fragmentHeader,
            standardUniform,
            "#line 0\n",
            ps.cStr(),
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
    return  (*(source))==' ' || (*(source))=='\t'||
            (*(source))=='\r'|| (*(source))=='\n';
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
static inline void skipSpaceAndComments(char*& source,int& line)
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
static inline void skipGLSLAndComments(char*& source,int& line)
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
//parser effect
static bool parserEffect(char* source, String& errors, String& vertex, String& fragment)
{
    //start count line
    int line=0;
    //init values
    errors="";
    vertex="";
    fragment="";
    //skip space
    skipSpaceAndComments(source,line);
    //get vertex
    while((*source)!='\0')
    {
        if(cmpCStr(source,"@VERTEX"))
        {
            source+=sizeof("@VERTEX");
            if(!addSourceFromEffect(source,vertex,line))
            {
                errors+="Parsing Effect, vertex shader olready added\n";
                return false;
            }
        }
        else if(cmpCStr(source,"@FRAGMENT"))
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
//save a uniform
void Shader::saveUniform(uint uniform, char* const cname)
{
    size_t lenCName=strlen(cname);
    //ignore built-in uniform (gl_...)
    if(lenCName >= 3
       && cname[0]=='g'
       && cname[1]=='l'
       && cname[2]=='_') return;
    //delete array syntax
    //nVidia return "array[0]", AMD return "array"
    //use the AMD style..
    for(size_t c=0;c!=lenCName;++c)
    {
        if(cname[c] == '[')
        {
            cname[c] = '\0';
            break;
        }
    }
    //c++ str
    String name(cname);
    //e2d built-in uniform
    if(name=="e2dProjection")
        uProjection=uniform;
    else if(name=="e2dModelView")
        uModelView=uniform;
    else if(name=="e2dView")
        uView=uniform;
    else if(name=="e2dModel")
        uModel=uniform;
    else if(name=="e2dView")
        uView=uniform;
    else if(name=="e2dAmbientColor")
        uAmbientColor=uniform;
    else if(name=="e2dColor")
        uColor=uniform;
    else if(name=="e2dViewport")
        uViewport=uniform;
    else if(name=="e2dTexture0")
        uTex0=uniform;
    //uniform by user
    else
        uMap[name]=uniform;
}
//get default uniforms
void Shader::buildUniform()
{
    //count uniform
    GLint nUniform{ 0 };
    GLint uStrMaxSize{ 0 };
    //get count of uniform
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &nUniform);
    //get max str size of all uniform
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uStrMaxSize);
    //alloc
    std::vector< char > buffer(uStrMaxSize+1);
    //for all uniforms
    for(GLuint index=0; index!=nUniform; ++index)
    {
        //reset data
        std::memset(buffer.data(), 0, buffer.size());
        //get name
        GLsizei  length{ 0 };
        GLsizei  size{ 0 };
        GLenum   type{ 0 };
        glGetActiveUniform(program, index, buffer.size(),  &length, &size, &type, buffer.data());
        //add uniform
        saveUniform(glGetUniformLocation(program,buffer.data()),buffer.data());
        //uniform errors
        CHECK_GPU_ERRORS();
    }
}
//get program id
uint Shader::programID() const
{
    return program;
}
//bind buffer
void Shader::bind()
{
    //enable program
    RenderContext::enableProgram(program);
    //update uniform
    updateUniform();
}
//get a standard uniform
UniformValue Shader::get(int uid)
{
	auto it = uDefaultMap.find(uid);
	if (it == uDefaultMap.end()) return UniformValue();
	return it->second;
}
//update uniforms
void Shader::updateUniform()
{
    //bind parameters
    updateStandardUniform();
	//bind default uniforms
	for (auto& it : uDefaultMap)
	{
		it.second.uniform(it.first);
	}
    //bind constom paramers
    if(uCallback) uCallback(*this);
    //uniform errors
    CHECK_GPU_ERRORS();
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
    if(uView>=0)
        uniform(uView, RenderContext::getView());
    if(uModel>=0)
        uniform(uModel,RenderContext::getModel());
    if(uAmbientColor>=0)
        uniform(uAmbientColor,RenderContext::getAmbientColor().toNormalize());
    if(uColor>=0)
        uniform(uColor,RenderContext::getColor().toNormalize());
    if(uViewport>=0)
        uniform(uViewport,RenderContext::getViewport());
    //uniform texture
    if(uTex0>=0)
        uniformTexture(uTex0,/*RenderContext::currentTexture()*/ 0);
    //uniform errors
    CHECK_GPU_ERRORS();
}
//uniform name
void Shader::uniform(const String& name,int v){ glUniform1i(uniformID(name), v); }
void Shader::uniform(const String& name,float v){ glUniform1f(uniformID(name), v); }
void Shader::uniform(const String& name,const Vector2D& v){ glUniform2fv(uniformID(name), 1, &v.x); }
void Shader::uniform(const String& name,const Vector3D& v){ glUniform3fv(uniformID(name), 1, &v.x); }
void Shader::uniform(const String& name,const Vector4D& v){ glUniform4fv(uniformID(name), 1, &v.x); }
void Shader::uniform(const String& name,const Matrix4x4& v){ glUniformMatrix4fv(uniformID(name), 1, false, v.entries); }

void Shader::uniform(const String& name,const std::vector<int>& v){ glUniform1iv(uniformID(name), v.size(), &v[0]); }
void Shader::uniform(const String& name,const std::vector<float>& v){ glUniform1fv(uniformID(name), v.size(), &v[0]); }
void Shader::uniform(const String& name,const std::vector<Vec2>& v){ glUniform2fv(uniformID(name), v.size(), &v[0].x); }
void Shader::uniform(const String& name,const std::vector<Vec3>& v){ glUniform3fv(uniformID(name), v.size(), &v[0].x); }
void Shader::uniform(const String& name,const std::vector<Vec4>& v){ glUniform4fv(uniformID(name), v.size(), &v[0].x); }
void Shader::uniform(const String& name,const std::vector<Mat4>& v){ glUniformMatrix4fv(uniformID(name), v.size(), false, v[0].entries); }

void Shader::uniformTexture(const String& name,const uint v){ glUniform1i(uniformID(name), v); }

//uniform id
int Shader::uniformID(const String& name) const
{
    //search
    auto itUid = uMap.find(name);
    //not find?
    if(uMap.end() == itUid) return -1;
    //else return uid
    return itUid->second;
}
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





