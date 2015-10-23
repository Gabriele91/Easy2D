
#ifndef SHADER_H
#define SHADER_H

#include <Config.h>
#include <Math3D.h>
#include <EString.h>
#include <Resource.h>

namespace Easy2D
{
  
class UniformValue
{
public:

	enum UniformType
	{
		U_NONE,
		U_INT,
		U_FLOAT,
		U_VEC2,
		U_VEC3,
		U_VEC4,
		U_MAT4
	};

	//destructor
	virtual ~UniformValue();

	//costructor
	UniformValue();
	UniformValue(const UniformValue& uv);

	//all type costructors
	UniformValue(int i);
	UniformValue(float f);
	UniformValue(const Vec2& v2);
	UniformValue(const Vec3& v3);
	UniformValue(const Vec4& v4);
	UniformValue(const Mat4& m4);
	UniformValue(std::unique_ptr<Vec3> v3);
	UniformValue(std::unique_ptr<Vec4> v4);
	UniformValue(std::unique_ptr<Mat4> m4);
	//set
	void set(int i);
	void set(float f);
	void set(const Vec2& v2);
	void set(const Vec3& v3);
	void set(const Vec4& v4);
	void set(const Mat4& m4);
	void set(std::unique_ptr<Vec3> v3);
	void set(std::unique_ptr<Vec4> v4);
	void set(std::unique_ptr<Mat4> m4);
	//uniform value
	void uniform(int uid);
	//get type
	UniformType getType();
	//cast
	operator int();
	operator float();
	operator Vec2();
	operator Vec3();
	operator Vec4();
	operator Mat4();

protected:
	//uniform type
	UniformType type;
	//uniform value
	union UValue
	{
		int   i;
		float f;
		Vec2 v2;
		std::unique_ptr<Vec3> v3;
		std::unique_ptr<Vec4> v4;
		std::unique_ptr<Mat4> m4;
		//init
		UValue()
		{
			new (&v3) std::unique_ptr<Vec3>();
		}
		//delete
		~UValue()
		{

		}
	}
	value;
	//free value
	void free();
};

class Shader : public Resource<Shader>
{
    //uniform map
    std::unordered_map<String, uint> uMap;
	std::unordered_map<uint, UniformValue> uDefaultMap;
    
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
	//default uniform
	template<class T>
	inline void set(const String& name, const T& value)
	{
		set( uniformID(name), value );
	}
	template<class T>
	void set(int uid, const T& value)
	{
		auto it = uDefaultMap.find((uint)uid);
		if (it == uDefaultMap.end())
			uDefaultMap.insert({ (uint)uid, UniformValue{ value } });
		else 
			it->second.set( value );
	}
	inline UniformValue get(const String& name)
	{
		return get(uniformID(name));
	}
	UniformValue get(int uid);
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
