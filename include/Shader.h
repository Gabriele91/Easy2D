
#ifndef SHADER_H
#define SHADER_H

#include <Config.h>
#include <Math3D.h>
#include <EString.h>
#include <Resource.h>

namespace Easy2D
{
   
class Shader : public Resource<Shader>
{
    //uniform map
    std::unordered_map<String,uint> uMap;
    
    //sshader program
    uint program;
    uint vertex;
    uint fragment;
    
    //default uniform
    int uProjection  { -1 };
    int uModelView   { -1 };
    int uView        { -1 };
    int uModel       { -1 };
    int uAmbientColor{ -1 };
    int uColor       { -1 };
    int uViewport    { -1 };
    int uTex0        { -1 };
    
    //compile shader program
    void compileProgram();
    uint compileShader(const String& shader);
    void createFragment(const String& shader);
    void createVertex(const String& shader);
    
    //uniform map
    DFUNCTION<void(Shader&)> uCallback{ nullptr };
    
    //compile utilities
    bool linking();
    bool compiler(uint shader);
    
    //get all uniform
    void buildUniform();
    void saveUniform(uint uniform,char* const name);
    
    //friend class
    friend class RenderContext;
    
public:
    
    //ettribute enum
    enum ATTRIBUTES
    {
        ATTRIBUTE_POSITION = 1,
        ATTRIBUTE_NORMAL   = 2,
        ATTRIBUTE_COLOR    = 3,
        ATTRIBUTE_TEXTCOOR = 4
    };
    
    //init shader
    Shader(ResourcesManager<Shader> *rsmr,
           const String& pathfile="");
    Shader();
    virtual ~Shader();
    //load shader
    void load(const String& ps);
    void load(const String& ps,
              const std::vector<String>& defines);
    void load(const String& vs,
              const String& ps);
    void load(const String& vs,
              const String& ps,
              const std::vector<String>& defines);
    //load methods
    virtual bool load();
    virtual bool unload();
    //set uniform callback
    void setUniformCallback(DFUNCTION<void(Shader&)> callback);
    //bind buffer
    void bind();
    //info shader
    uint programID() const;
    void updateUniform();
    void updateStandardUniform();
    //uniform name
    void uniform(const String& name,int v);
    void uniform(const String& name,float v);
    void uniform(const String& name,const Vector2D& v);
    void uniform(const String& name,const Vector3D& v);
    void uniform(const String& name,const Vector4D& v);
    void uniform(const String& name,const Matrix4x4& v);
    void uniform(const String& name,const std::vector<int>& v);
    void uniform(const String& name,const std::vector<float>& v);
    void uniform(const String& name,const std::vector<Vec2>& v);
    void uniform(const String& name,const std::vector<Vec3>& v);
    void uniform(const String& name,const std::vector<Vec4>& v);
    void uniform(const String& name,const std::vector<Mat4>& v);
    void uniformTexture(const String& name,const uint v);
    //uniform id
    int uniformID(const String& name) const;
    void uniform(int uid,int v);
    void uniform(int uid,float v);
    void uniform(int uid,const Vector2D& v);
    void uniform(int uid,const Vector3D& v);
    void uniform(int uid,const Vector4D& v);
    void uniform(int uid,const Matrix4x4& v);
    void uniform(int uid,const std::vector<int>& v);
    void uniform(int uid,const std::vector<float>& v);
    void uniform(int uid,const std::vector<Vec2>& v);
    void uniform(int uid,const std::vector<Vec3>& v);
    void uniform(int uid,const std::vector<Vec4>& v);
    void uniform(int uid,const std::vector<Mat4>& v);
    void uniformTexture(int uid,const uint v);
    
};
    
};

#endif
