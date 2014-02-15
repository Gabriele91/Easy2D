#ifndef UTILITY_H
#define UTILITY_H


#include <Config.h>
#include <string>
#include <EString.h>

namespace Easy2D {
	namespace Utility {

		/////////////////////////////////////////////
		//function and wrapper for reverse foreach
		template<class T>
		class const_reverse_wrapper {
		  const T& container;

		public:
		  const_reverse_wrapper(const T& cont) : container(cont){ }
		  auto begin() const  ->  decltype(container.rbegin())   { 
			  return container.rbegin(); 
		  }
		  auto end() const  ->  decltype(container.rbegin())   { 
			  return container.rend(); 
		  }
		};
		template<class T>
		class reverse_wrapper {
		  T& container;
		  
		public:
		  reverse_wrapper(T& cont) : container(cont){ }
		  auto begin() ->  decltype(container.rbegin())  { 
			  return container.rbegin();
		  }
		  auto end() ->  decltype(container.rend())  { 
			  return container.rend();
		  }
		};
		/////////////////////////////////////////////
		template<class T>
		const_reverse_wrapper<T> reverse(const T& cont) {
		  return const_reverse_wrapper<T>(cont);
		}
		template<class T>
		reverse_wrapper<T> reverse(T& cont) {
		  return reverse_wrapper<T>(cont);
		}
		/////////////////////////////////////////////

		class Path {

		public:
			
			Path(const char* path);
			Path(const String& path);
			Path(const std::string& path);
			DFORCEINLINE const String& getPath() const { return path; }
			DFORCEINLINE const String& getDirectory() const { return directory; }
			DFORCEINLINE const String& getFilename() const { return file; }
			DFORCEINLINE const String& getFilebasename() const { return filebasename; }
			DFORCEINLINE const String& getExtension() const { return ext; }
			DFORCEINLINE const bool isAbsolute() const { return isabs; }
			//
			bool existsFile();
			bool existsDirectory();
			//			
			std::vector<String> getFiles() const;
			std::vector<String> getSubDirs() const;
			//
			static String getCanonicalPath(const String& path);
			static bool convertToCanonicalPath(String& path);
			//
			//cast
			operator const String& () const{
				return this->path;
			}
			operator const char* () const{
				return this->path;
			}
 
		 private:
			
			DFORCEINLINE void costructor(const String& path);
			String path;
			String directory;
			String file;
			String filebasename;
			String ext;
			bool   isabs;
		};
	
	};
};

#endif