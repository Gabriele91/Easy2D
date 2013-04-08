#include <stdafx.h>
#include "Utility.h"
//
using namespace Easy2D;
using namespace Easy2D::Utility;
//
Path::Path(const String& _path){
	costructor(_path);
}
Path::Path(const char* _path){
	costructor(String(_path));
}
void Path::costructor(const String& _path){
	//to canonical path
	path=_path;
	convertToCanonicalPath(path);
	//get directory:	
	int flash=path.rfind("/");
	if(flash){
		directory=path.substr(0,flash);
		file=path.substr(flash+1,path.size()-flash-1);
	}
	else{
		file=path;
	}
	//get base name		
	if(file.size()){
		int point=file.find(".");
		filebasename=file.substr(0,point);
	}
	//Extension
	if(file.size()){	
		int point=file.rfind(".");
		if(point>=0)
			ext=file.substr(point+1,path.size()-point-1);
	}
}
bool Path::existsFile(){		
	struct stat st;
	return stat(path,&st) == 0;
	return false;
}
bool Path::existsDirectory(){
	struct stat st;
	if(stat(directory,&st) == 0)
		return (st.st_mode & S_IFDIR) != 0;
	return false;
}

std::vector<String> Path::getFiles() const{
	
	std::vector<String> out;	

	#ifdef  PLATFORM_UNIX 
		
		DIR *dp;
		struct dirent *dirp;
		struct stat st;
		String std_dir=directory+"/";
		String path_all=std_dir+"*";
		String namefile;

		if((dp  = opendir(std_dir)) == NULL)
			return out;
		
		while ((dirp = readdir(dp)) != NULL){
			//get all path
			namefile=std_dir+dirp->d_name;
			//if isn't a directory
			if(stat(namefile,&st)==0 && (st.st_mode & S_IFDIR) == 0)
				//push
				out.push_back(dirp->d_name);
		}
		closedir(dp);

	#elif defined(PLATFORM_WINDOW)

		WIN32_FIND_DATA ffd;
		struct stat st;
		String std_dir=directory+"/";
		String path_all=std_dir+"*";
		String namefile;
		HANDLE hFind = FindFirstFile(path_all, &ffd);

		do{
			if ( !strcmp( ffd.cFileName, "."   )) continue;
            if ( !strcmp( ffd.cFileName, ".."  )) continue;
			//if isn't a directory
			if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				//push
				out.push_back(ffd.cFileName);
			
		}while (FindNextFile(hFind, &ffd) != 0);

	#endif

	return out;
}
std::vector<String> Path::getSubDirs() const{
	
	std::vector<String> out;	

	#ifdef  PLATFORM_UNIX 
		
		DIR *dp;
		struct dirent *dirp;
		struct stat st;
		String std_dir=directory+"/";
		String path_all=std_dir+"*";
		String namefile;

		if((dp  = opendir(std_dir)) == NULL)
			return out;
		
		while ((dirp = readdir(dp)) != NULL){
			//get all path
			namefile=std_dir+dirp->d_name;
			//if is a directory
			if(stat(namefile,&st)==0 && (st.st_mode & S_IFDIR) != 0)
				//push
				out.push_back(dirp->d_name);
		}
		closedir(dp);

	#elif defined(PLATFORM_WINDOW)

		WIN32_FIND_DATA ffd;
		struct stat st;
		String std_dir=directory+"/";
		String path_all=std_dir+"*";
		String namefile;
		HANDLE hFind = FindFirstFile(path_all, &ffd);

		do{
			if ( !strcmp( ffd.cFileName, "."   )) continue;
            if ( !strcmp( ffd.cFileName, ".."  )) continue;
			//if is a directory
			if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				//push
				out.push_back(ffd.cFileName);
			
		}while (FindNextFile(hFind, &ffd) != 0);

	#endif
	
	return out;
}

String Path::getCanonicalPath(const String& path){
	String outPath(path);
	convertToCanonicalPath(outPath);
	return outPath;
}
void Path::convertToCanonicalPath(String& path){	
	//unix format:
	for(auto& c:path) if(c=='\\') c='/';
	//delete space at end
	int space_len=0;
	for(auto c:reverse(path)) 
		if(c==' ') 
			++space_len;
		else
			break;
	path=path.substr(0,path.size()-space_len);
	//directory?
	int point=path.rfind(".");
	int flash=path.rfind("/");
	//is a directory path
	if(flash>point){
		//if directory not have end part
		if(path[path.size()-1]!='/') path+='/';
	}
	//
}