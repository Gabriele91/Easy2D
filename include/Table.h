#ifndef TABLE_H
#define TABLE_H

#include <Config.h>
#include <Math3D.h>
#include <EString.h>
#include <iostream>
#include <Resource.h>

namespace Easy2D{

	class Table : public Resource<Table> {

	public:

		enum TypeDate{
			FLOAT,
			VECTOR2D,
			VECTOR3D,
			VECTOR4D,
			MATRIX4X4,
			STRING,
			BINARY,
			TABLE
		};
		/* map key */
		class KeyTable{
			protected:
				bool isstring;
				unsigned int itr;
				String sv;
			public:
				KeyTable(const char* key):isstring(true),sv(key){}
				KeyTable(const String & key):isstring(true),sv(key){}
				KeyTable(int i):isstring(false),itr(i){}
				bool isString() const { return isstring; };
				const String& string() const{ return sv; };
				unsigned int integer() const{ return itr; };

				struct KeyHash {
					  std::size_t operator() (const KeyTable& key) const {
						size_t maxsize=-1;
						size_t hlmaxsize=maxsize/2;
						if(key.isString()){
							std::hash<std::string> hstring;
							return (hstring(key.string().c_str()))%(hlmaxsize)+hlmaxsize;
						}
						else{
							std::hash<unsigned int> huint;
							return hlmaxsize-(huint(key.integer())%hlmaxsize);
						}
					}
				};
				struct KeyEq {
					bool operator()(const KeyTable&  key1,const KeyTable&  key2) const {
						if(key1.isString() && key2.isString())
							return key1.string().compare(key2.string())==0;
						else if(!(key1.isString()||key2.isString()))
							return key1.integer()==key2.integer();
						return false;
					}
				};
				struct KeyCompare {
					bool operator()(const KeyTable&  key1,const KeyTable&  key2) const {

						if(key1.isString() && key2.isString())
							return key1.string().compare(key2.string())<0;
						else if(!(key1.isString()||key2.isString()))
							return key1.integer()<key2.integer();

						return !key1.isString();
					}
				};
		};

	private:

		/* map values */
		class Value{
		public:
			//
			TypeDate type;
			Value( TypeDate type ):type(type){}
            virtual ~Value(){}
			//
			bool asType(TypeDate tp){
				return type==tp;
			}
			//
			virtual void* getValue(){return NULL;};
			virtual Value* clone(){return NULL;};
			//return generic
			template<typename T> T& get(){
				return *((T*)getValue());
			}
		};
		template<typename T>
		class DefineValue : public Value{
		public:
			//
			T value;
			DefineValue( TypeDate type,const T& value ):Value(type),value(value){}
			//
			virtual void* getValue(){ return &value; };
			virtual Value* clone(){ return new DefineValue<T>(type,value); };
			//
		};
		/* map */
		#ifdef ENABLE_ORDERED_TABLE
		typedef std::map<KeyTable,
			             Value*,
						 KeyTable::KeyCompare> UNMAPTable;
		#else
		typedef DUNORDERED_MAP<KeyTable,
			                   Value*,
			                   KeyTable::KeyHash,
							   KeyTable::KeyEq> UNMAPTable;
		#endif

		/* binary type */
		struct Binary{
            //statics
            static Binary NONE;
            //class
			void *value;
			size_t len;
			bool destroydata;
			Binary():value(NULL),len(0){};
			Binary(void *value,size_t len):value(value),len(len){};
			~Binary(){
				if(destroydata&&value) free(value);
			};
		};

	public:

        //statics
        static Table NONE;
		/* costructor */
		Table(ResourcesManager<Table> *rsmr,
			  const String& pathfile="");
        Table(const Table& cptable);
		Table();
		/* destructor */
		virtual ~Table();

		/* resource mathods implementation */
		virtual bool load();
		virtual bool unload();

		/* c++11 for each */
		UNMAPTable::iterator begin(){ return table.begin(); }
		UNMAPTable::const_iterator begin() const { return table.cbegin(); }
		UNMAPTable::iterator end(){ return table.end(); }
		UNMAPTable::const_iterator end() const{ return table.cend(); }

		UNMAPTable::reverse_iterator rbegin(){ return table.rbegin(); }
		UNMAPTable::const_reverse_iterator crbegin() const { return table.crbegin(); }
		UNMAPTable::reverse_iterator rend(){ return table.rend(); }
		UNMAPTable::const_reverse_iterator crend() const{ return table.crend(); }
		/* find */
		UNMAPTable::iterator find(const KeyTable& key){ return table.find(key); }

		/** return table-array size  */
		unsigned int indexUnnomed() const { return index; }
		/** return table-array size  */
		unsigned int size() const { return index; }

		/** create a sub table  */
		DFORCEINLINE Table& createTable(){
			DefineValue<Table> *ptr=new DefineValue<Table>(TABLE,Table());
			table[index]=ptr;
			++index;
			return *((Table*)ptr->getValue());
		}
		/** create a sub table with a name */
		DFORCEINLINE Table& createTable(const String& key){

			if(exists(key))
				delete table[key];
			DefineValue<Table> *ptr=new DefineValue<Table>(TABLE,Table(getResourcesManager(),rpath));
			table[key]=ptr;

			return *((Table*)ptr->getValue());
		}

		/** set a floating in an associative table */
		DFORCEINLINE void set(const String& key, float value){
			set(key,FLOAT,value);
		}
		/** set a vector2D in an associative table */
		DFORCEINLINE void set(const String& key, const Vec2& value){
			set(key,VECTOR2D,value);
		}
		/** set a vector3D in an associative table */
		DFORCEINLINE void set(const String& key, const Vec3& value){
			set(key,VECTOR3D,value);
		}
		/** set a vector4D in an associative table */
		DFORCEINLINE void set(const String& key, const Vec4& value){
			set(key,VECTOR4D,value);
		}
		/** set a Matrix4x4 in an associative table */
		DFORCEINLINE void set(const String& key, const Matrix4x4& value){
			set(key,MATRIX4X4,value);
		}
		/** set a string in an associative table */
		DFORCEINLINE void set(const String& key, const String& value){
			set(key,STRING,value);
		}
		/** set a binary file in an associative table */
		DFORCEINLINE void set(const String& key, void *value, size_t len,bool destroydata=false){
			if(exists(key))
				delete table[key];
			DefineValue<Binary> *ptr=new DefineValue<Binary>(TABLE,Binary());
			((Binary*)ptr->getValue())->len=len;
			((Binary*)ptr->getValue())->value=value;
			((Binary*)ptr->getValue())->destroydata=destroydata;
			table[key]=ptr;
		}


		/** set a floating in an array */
		DFORCEINLINE void set(float value){
			set(index,FLOAT,value);
			++index;
		}
		/** set a vector2D in an array */
		DFORCEINLINE void set(const Vec2& value){
			set(index,VECTOR2D,value);
			++index;
		}
		/** set a vector3D in an array */
		DFORCEINLINE void set(const Vec3& value){
			set(index,VECTOR3D,value);
			++index;
		}
		/** set a vector4D in an array */
		DFORCEINLINE void set(const Vec4& value){
			set(index,VECTOR4D,value);
			++index;
		}
		/** set a Matrix4x4 in an array */
		DFORCEINLINE void set(const Matrix4x4& value){
			set(index,MATRIX4X4,value);
			++index;
		}
		/** set a string in an array */
		DFORCEINLINE void set(const String& value){
			set(index,STRING,value);
			++index;
		}
		/** set a binary file in an array */
		DFORCEINLINE void set(void *value, size_t len,bool destroydata=false){
			DefineValue<Binary> *ptr=new DefineValue<Binary>(TABLE,Binary());
			((Binary*)ptr->getValue())->len=len;
			((Binary*)ptr->getValue())->value=value;
			((Binary*)ptr->getValue())->destroydata=destroydata;
			table[index]=ptr;
			++index;
		}

		/** return a floating point associate a table/array key */
		DFORCEINLINE float getFloat(const KeyTable& key,float vdefault=0) const{
			if(existsAsType(key,FLOAT)) return *((float*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a vector2D associate a table/array key */
		DFORCEINLINE const Vec2& getVector2D(const KeyTable& key,const Vec2& vdefault=Vec2::ZERO) const{
			if(existsAsType(key,VECTOR2D)) return *((Vec2*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a vector3D associate a table/array key */
		DFORCEINLINE const Vec3& getVector3D(const KeyTable& key,const Vec3& vdefault=Vec3::ZERO) const{
			if(existsAsType(key,VECTOR3D)) return *((Vec3*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a vector4D associate a table/array key */
		DFORCEINLINE const Vec4& getVector4D(const KeyTable& key,const Vec4& vdefault=Vec4::ZERO) const{
			if(existsAsType(key,VECTOR4D)) return *((Vec4*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a Matrix4x4 associate a table/array key */
		DFORCEINLINE const Matrix4x4& getMatrix4x4(const KeyTable& key,const Matrix4x4& vdefault=Matrix4x4::IDENTITY) const{
			if(existsAsType(key,MATRIX4X4)) return *((Matrix4x4*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a string associate a table/array key */
		DFORCEINLINE const String& getString(const KeyTable& key,const String& vdefault=String::NONE) const{
			if(existsAsType(key,STRING)) return *((String*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a constant table/array associate a table/array key */
		DFORCEINLINE const Table& getConstTable(const KeyTable& key,const Table& vdefault=Table::NONE) const{
			if(existsAsType(key,TABLE)) return *((Table*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a table/array associate a table/array key */
		DFORCEINLINE const Table& getTable(const KeyTable& key,const Table& vdefault=Table::NONE){
			if(existsAsType(key,TABLE)) return *((Table*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a binary file associate a table/array key */
		DFORCEINLINE const Binary& getBinary(const KeyTable& key,const Binary& vdefault=Binary::NONE) const{
			if(existsAsType(key,BINARY)) return *((Binary*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a generic value associate a table/array key */
		template<typename T> T& get(const KeyTable& key){
			return *((T*)(table.find(key)->second->getValue()));
		}
		/** return true if exist a value associated with key */
		DFORCEINLINE bool exists( const KeyTable& key ) const{
			return (table.find( key ) != table.end());
		}
		/** return true if exist as value type associated with key */
		DFORCEINLINE bool existsAsType( const KeyTable& key , TypeDate type  ) const{
			const auto& tvalue=table.find( key );
			if(tvalue != table.end())
				return tvalue->second->type==type;
			return false;
		}
		/** deserialize a string */
		int deserialize(const String& intextfile);
		/** serialize table into a string */
		String serialize(int countspace=0) const;
		/** return  deserialize erros */
		const String& getDeserializeErros(){
			return dErrors.toString();
		}
		/** data struct serialize/deserialize */
		template<class T> 
		class Translator{

		   //single members
		   typedef bool T::*BoolMember;
		   typedef int T::*IntMember;
		   typedef float T::*FloatMember;
		   typedef String T::*StringMember;
		   typedef Vec2 T::*Vec2Member;
		   typedef Vec3 T::*Vec3Member;
		   typedef Vec4 T::*Vec4Member;
		   typedef Mat4 T::*Mat4Member;

		   //ptr maps
		   std::map<KeyTable, BoolMember, KeyTable::KeyCompare > bools;
		   std::map<KeyTable, IntMember, KeyTable::KeyCompare > ints;
		   std::map<KeyTable, FloatMember, KeyTable::KeyCompare > floats;
		   std::map<KeyTable, StringMember, KeyTable::KeyCompare > strings;
		   std::map<KeyTable, Vec2Member, KeyTable::KeyCompare > vec2s;
		   std::map<KeyTable, Vec3Member, KeyTable::KeyCompare > vec3s;
		   std::map<KeyTable, Vec4Member, KeyTable::KeyCompare > vec4s;
		   std::map<KeyTable, Mat4Member, KeyTable::KeyCompare > mat4s;
		   
		public:
						
		  Translator& add(const KeyTable& key, BoolMember member){
				bools[key]=member;
				return (*this);
		  }
		  Translator& add(const KeyTable& key, IntMember member){
				ints[key]=member;
				return (*this);
		  }
		  Translator& add(const KeyTable& key, FloatMember member){
				floats[key]=member;
				return (*this);
		  }
		  Translator& add(const KeyTable& key, StringMember member){
				strings[key]=member;
				return (*this);
		  }
		  Translator& add(const KeyTable& key, Vec2Member member){
				vec2s[key]=member;
				return (*this);
		  }
		  Translator& add(const KeyTable& key, Vec3Member member){
				vec3s[key]=member;
				return (*this);
		  }
		  Translator& add(const KeyTable& key, Vec4Member member){
				vec4s[key]=member;
				return (*this);
		  }
		  Translator& add(const KeyTable& key, Mat4Member member){
				mat4s[key]=member;
				return (*this);
		  }
		  //fast encoding
		  void decoder(const Table& table,T& astruct){
			  for(auto abool:bools)
				  astruct.*(abool.second)=table.getFloat(abool.first)!=0.0;
			  for(auto aint:ints)
				  astruct.*(aint.second)=(int)table.getFloat(aint.first);
			  for(auto afloat:floats)
				  astruct.*(afloat.second)=table.getFloat(afloat.first);
			  for(auto astring:strings)
				  astruct.*(astring.second)=table.getString(astring.first);
			  for(auto avec2:vec2s)
				  astruct.*(avec2.second)=table.getVector2D(avec2.first);
			  for(auto avec3:vec3s)
				  astruct.*(avec3.second)=table.getVector3D(avec3.first);
			  for(auto avec4:vec4s)
				  astruct.*(avec4.second)=table.getVector4D(avec4.first);
			  for(auto amat4:mat4s)
				  astruct.*(amat4.second)=table.getMatrix4x4(amat4.first);
		  }
		  void encoder(const T& astruct,Table& table){
			  for(auto abool:bools)
				  table.set(abool.first,FLOAT,(float)(astruct.*(abool.second)));
			  for(auto aint:ints)
				  table.set(aint.first,FLOAT,(float)(astruct.*(aint.second)));
			  for(auto afloat:floats)
				  table.set(afloat.first,FLOAT,astruct.*(afloat.second));
			  for(auto astring:strings)
				  table.set(astring.first,STRING,astruct.*(astring.second));
			  for(auto avec2:vec2s)
				  table.set(avec2.first,VECTOR2D,astruct.*(avec2.second));
			  for(auto avec3:vec3s)
				  table.set(avec3.first,VECTOR3D,astruct.*(avec3.second));
			  for(auto avec4:vec4s)
				  table.set(avec4.first,VECTOR4D,astruct.*(avec4.second));
			  for(auto amat4:mat4s)
				  table.set(amat4.first,MATRIX4X4,astruct.*(amat4.second));
		  }
		  
		  void decoderStr(const String& textfile,T& astruct){
				Table table;
				table.deserialize(textfile);
				decoder(table,astruct);
		  }
		  void encoderStr(const T& astruct,String& textfile){
			Table table;
			encoder(astruct,table);
			textfile=table.serialize();
		  }
		  //easy encoding
		  T decoder(const Table& table){
			T astruct;
			decoder(table,astruct);
			return T;
		  }
		  Table encoder(const T& astruct){
			Table table;
			encoder(astruct,table);
			return table;
		  }
		  
		  T decoderStr(const String& textfile){
			T astruct;
			decoder(textfile,astruct);
			return T;
		  }
		  String encoderStr(const T& astruct){
			String textfile;
			encoder(astruct,textfile);
			return textfile;
		  }
		  
		};

	private:
		/* private deserialize/serialize */
		int __deserialize(const String& intextfile,int* lenRead=NULL,unsigned int* line=NULL);
		String __serialize(int countspace=0,bool havename=false) const;
		/* set value */
		template <typename T>
		DFORCEINLINE void set(const KeyTable& key,TypeDate type,const T& value ){
			if(exists(key))
				delete table[key];
			table[key]=new DefineValue<T>(type,value);
		}
		/* set private */
		template <typename T>
		DFORCEINLINE void setPt(const KeyTable& key,TypeDate type,const T& value ){
			if(exists(key))
				delete table[key];
			table[key]=new DefineValue<T>(type,value);
			//get max key
			if(!key.isString()){
				if(index<=key.integer()) index=key.integer()+1;
			}
		}
		DFORCEINLINE Table& createTablePt(const KeyTable& key){

			if(exists(key))
				delete table[key];
			DefineValue<Table> *ptr=new DefineValue<Table>(TABLE,Table(getResourcesManager(),rpath));
			table[key]=ptr;
			//get max key
			if(!key.isString()){
				if(index<=key.integer()) index=key.integer()+1;
			}
			//
			return *((Table*)ptr->getValue());
		}
		DFORCEINLINE void setPt(const KeyTable& key, float value){
			setPt(key,FLOAT,value);
		}
		DFORCEINLINE void setPt(const KeyTable& key, const Vec2& value){
			setPt(key,VECTOR2D,value);
		}
		DFORCEINLINE void setPt(const KeyTable& key, const Vec3& value){
			setPt(key,VECTOR3D,value);
		}
		DFORCEINLINE void setPt(const KeyTable& key, const Vec4& value){
			setPt(key,VECTOR4D,value);
		}
		DFORCEINLINE void setPt(const KeyTable& key, const Matrix4x4& value){
			setPt(key,MATRIX4X4,value);
		}
		DFORCEINLINE void setPt(const KeyTable& key, const String& value){
			setPt(key,STRING,value);
		}
		DFORCEINLINE void setPt(const KeyTable& key, void *value, size_t len,bool destroydata=false){
			if(exists(key))
				delete table[key];
			DefineValue<Binary> *ptr=new DefineValue<Binary>(TABLE,Binary());
			((Binary*)ptr->getValue())->len=len;
			((Binary*)ptr->getValue())->value=value;
			((Binary*)ptr->getValue())->destroydata=destroydata;
			table[key]=ptr;
			//get max key
			if(!key.isString()){
				if(index<=key.integer()) index=key.integer()+1;
			}
			//
		}
		/* table */
		UNMAPTable table;
		unsigned int index;
		/* errors */
		class DeserializeErros{
		protected:

			String errors;

		public:

			void push(int line,char c,const String& error){
				errors+=String::toString(line)+":"+c+":"+error+"\n";
			}
			void clear(){
				errors="";
			}
			const String& toString(){
				return errors;
			}
			bool hasErrors(){ return errors.size()>0; }
		};
		DeserializeErros dErrors;

	};

};



#endif
