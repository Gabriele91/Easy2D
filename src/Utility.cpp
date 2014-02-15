#include <stdafx.h>
#include <Utility.h>
#include <Application.h>
#ifdef  PLATFORM_UNIX
    #include<sys/stat.h>
    #include<unistd.h>
    #include<dirent.h>
#endif
//
using namespace Easy2D;
using namespace Easy2D::Utility;
//
#ifdef PLATFORM_WINDOW
	#include <limits.h>
	#include <errno.h>
	#include <sys/stat.h>
	char *realpath(const char *path, char resolved_path[MAX_PATH])
	{
	  char *return_path = 0;

	  if (path) //Else EINVAL
	  {
		if (resolved_path)
		{
		  return_path = resolved_path;
		}
		else
		{
		  //Non standard extension that glibc uses
		  return_path =(char*) malloc(MAX_PATH);
		}

		if (return_path) //Else EINVAL
		{
		  //This is a Win32 API function similar to what realpath() is supposed to do
		  size_t size = GetFullPathNameA(path, MAX_PATH, return_path, 0);

		  //GetFullPathNameA() returns a size larger than buffer if buffer is too small
		  if (size > MAX_PATH)
		  {
			if (return_path != resolved_path) //Malloc'd buffer - Unstandard extension retry
			{
			  size_t new_size;

			  free(return_path);
			  return_path =(char*) malloc(size);

			  if (return_path)
			  {
				new_size = GetFullPathNameA(path, size, return_path, 0); //Try again

				if (new_size > size) //If it's still too large, we have a problem, don't try again
				{
				  free(return_path);
				  return_path = 0;
				  errno = ENAMETOOLONG;
				}
				else
				{
				  size = new_size;
				}
			  }
			  else
			  {
				//I wasn't sure what to return here, but the standard does say to return EINVAL
				//if resolved_path is null, and in this case we couldn't malloc large enough buffer
				errno = EINVAL;
			  }
			}
			else //resolved_path buffer isn't big enough
			{
			  return_path = 0;
			  errno = ENAMETOOLONG;
			}
		  }

		  //GetFullPathNameA() returns 0 if some path resolve problem occured
		  if (!size)
		  {
			if (return_path != resolved_path) //Malloc'd buffer
			{
			  free(return_path);
			}

			return_path = 0;

			//Convert MS errors into standard errors
			switch (GetLastError())
			{
			  case ERROR_FILE_NOT_FOUND:
				errno = ENOENT;
				break;

			  case ERROR_PATH_NOT_FOUND: case ERROR_INVALID_DRIVE:
				errno = ENOTDIR;
				break;

			  case ERROR_ACCESS_DENIED:
				errno = EACCES;
				break;

			  default: //Unknown Error
				errno = EIO;
				break;
			}
		  }

		  //If we get to here with a valid return_path, we're still doing good
		  if (return_path)
		  {
			struct stat stat_buffer;

			//Make sure path exists, stat() returns 0 on success
			if (stat(return_path, &stat_buffer))
			{
			  if (return_path != resolved_path)
			  {
				free(return_path);
			  }

			  return_path = 0;
			  //stat() will set the correct errno for us
			}
			//else we succeeded!
		  }
		}
		else
		{
		  errno = EINVAL;
		}
	  }
	  else
	  {
		errno = EINVAL;
	  }

	  return return_path;
	}
#endif
//
Path::Path(const String& _path){
	costructor(_path);
}
Path::Path(const std::string& path){
	costructor(String(path));
}
Path::Path(const char* _path){
	costructor(String(_path));
}
void Path::costructor(const String& _path){
	//to canonical path
	path=_path;
	isabs=convertToCanonicalPath(path);
	//get directory:
	int flash=path.rfind("/");
	if(flash>=0){
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
bool Path::convertToCanonicalPath(String& path){
	//get real path
	//char buffer[FILENAME_MAX];
	//realpath(path,buffer);
	//path=buffer;
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
    //start is absolute?
    bool absolute=false;
#ifdef  PLATFORM_UNIX
    String root="/";
    for(size_t i=0;i!=path.size();++i){
        if(path[i]=='/'){
            path=path.substr(i,path.size());
            absolute=true;//is a absolute
            break;
        }
        else
        if(path[i]!=' ') break;
    }
#elif defined(PLATFORM_WINDOW)
	int rootfind=path.find(":");
    String root;
    if(rootfind>0){
        absolute=true;
        std::vector<String> rootAndPath;
        path.split(":",rootAndPath);
        root=" "+rootAndPath[0]+":/";
		//delete left space
		for(size_t i=0;i!=root.size();++i){
			if(root[i]!=' '){
				root=root.substr(i,path.size());
				break;
			}
		}
		//
        path=rootAndPath[1];
    }
#endif
	//replace "void path"
	path.replace("//","");
	if(path.size()==0||path=="."||path=="./"){ path="./"; return false; }
	//replace ../
	std::vector<String> dirs;
	path.split("/",dirs);
	path=""; //release
	for(uint i=0;i<dirs.size()-1;++i){
	 if(dirs[i]!="."){
		 if(dirs[i+1]!="..")
			path+=dirs[i]+"/";
		  else
			  ++i;
	 }
	}
	if(dirs.size()>1){
		 if(dirs[dirs.size()-1]!=".")
			 if(dirs[dirs.size()-1]!="..")
				path+=dirs[dirs.size()-1];
	}
	else
		path=dirs[0];
	//replace is made a void path..
	if(path.size()==0){
        //is an absolute path
        if(absolute)
            path=root+path;
        else //is relative
            path="./";
        return absolute;
    }
	//directory?
	int point=path.rfind(".");
	int flash=path.rfind("/");
	//is a directory path
	if(flash>point){
		//if directory not have end part
		if(path[path.size()-1]!='/') path+='/';
	}
	//is an absolute path
    if(absolute)
        path=root+path;
    //
    return absolute;
}
