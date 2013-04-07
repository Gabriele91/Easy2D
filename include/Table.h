#ifndef TABLE_H
#define TABLE_H

#include <Config.h>
#include <Math2D.h>
#include <EString.h>
#include <Resource.h>
#include <iostream>

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
			//
			bool asType(TypeDate tp){
				return type==tp;
			}
			//
			virtual void* getValue()=0;
			virtual Value* clone()=0;
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



		/* costructor */
		Table(ResourcesManager<Table> *rsmr,
			  const String& pathfile="");
		Table();
		/* destructor */
		~Table();

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
			DefineValue<Table> *ptr=new DefineValue<Table>(TABLE,Table());
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
		DFORCEINLINE const Matrix4x4& getMatrix4x4(const KeyTable& key,const Matrix4x4& vdefault=Matrix4x4()) const{
			if(existsAsType(key,MATRIX4X4)) return *((Matrix4x4*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a string associate a table/array key */	
		DFORCEINLINE const String& getString(const KeyTable& key,const String& vdefault=String()) const{
			if(existsAsType(key,STRING)) return *((String*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a constant table/array associate a table/array key */	
		DFORCEINLINE const Table& getConstTable(const KeyTable& key,const Table& vdefault=Table()) const{
			if(existsAsType(key,TABLE)) return *((Table*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a table/array associate a table/array key */	
		DFORCEINLINE Table& getTable(const KeyTable& key,Table& vdefault=Table()){
			if(existsAsType(key,TABLE)) return *((Table*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a binary file associate a table/array key */	
		DFORCEINLINE const Binary& getBinary(const KeyTable& key,const Binary& vdefault=Binary()) const{
			if(existsAsType(key,BINARY)) return *((Binary*)(table.find(key)->second->getValue()));
			return vdefault;
		}
		/** return a generic value associate a table/array key */	
		template<typename T> T& get(){
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
			DefineValue<Table> *ptr=new DefineValue<Table>(TABLE,Table());
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
				errors;
			}
			const String& toString(){
				return errors;
			}
		};
		DeserializeErros dErrors;

	};

};



#endif