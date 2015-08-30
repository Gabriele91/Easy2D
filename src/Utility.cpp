#include <stdafx.h>
#include <Utility.h>
#include <Application.h>
#include <Math3D.h>
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
#include <Shlwapi.h>
//define unix macro
#ifndef PATH_MAX
	#define PATH_MAX MAX_PATH
#endif
//function get relative path
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
            return_path =(char*) Easy2D::malloc(MAX_PATH);
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

					Easy2D::free(return_path);
					return_path = (char*)Easy2D::malloc(size);

                    if (return_path)
                    {
                        new_size = GetFullPathNameA(path, size, return_path, 0); //Try again

                        if (new_size > size) //If it's still too large, we have a problem, don't try again
                        {
							Easy2D::free(return_path);
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
					Easy2D::free(return_path);
                }

                return_path = 0;

                //Convert MS errors into standard errors
                switch (GetLastError())
                {
                case ERROR_FILE_NOT_FOUND:
                    errno = ENOENT;
                    break;

                case ERROR_PATH_NOT_FOUND:
                case ERROR_INVALID_DRIVE:
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
						Easy2D::free(return_path);
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
Path::Path(const String& _path)
{
    costructor(_path);
}
Path::Path(const std::string& path)
{
    costructor(String(path));
}
Path::Path(const char* _path)
{
    costructor(String(_path));
}

static void getDirectoryAndFileFromPath(const String& path, String& directory, String& file)
{

	//get directory end:
	String::ConstIterator flash = path.rfind("/");
	//gat names
	if (flash != path.cend())
	{
		directory = path.substr(path.cbegin(), flash);
		file = path.substr(flash + 1, path.cend());
	}
	else
	{
		file = path;
	}
}

static void getExtensionFromFileName(const String& file, String& filebasename, String& ext, String& leftext)
{
	if (file.size())
	{
		String::ConstReverseIterator point = file.find(".");
		String::ConstReverseIterator rightPoint = file.rfind(".");
		//get base file name
		filebasename = file.substr(file.cbegin(), point);
		//get extensions
		ext = file.substr(rightPoint-1, file.cend());
		leftext = file.substr(point+1, file.cend());
	}
}

void Path::costructor(const String& _path)
{
    //to canonical path
    path=_path;
    isabs=convertToCanonicalPath(path);
    //get dir name and file name
	getDirectoryAndFileFromPath(path,directory,file);
	//get extension and base name
	getExtensionFromFileName(file,filebasename,ext,leftext);
}
bool Path::existsFile()
{
    struct stat st;
    return stat(path,&st) == 0;
    return false;
}
bool Path::existsDirectory()
{
    struct stat st;
    if(stat(directory,&st) == 0)
        return (st.st_mode & S_IFDIR) != 0;
    return false;
}

std::vector<String> Path::getFiles() const
{

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

    while ((dirp = readdir(dp)) != NULL)
    {
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
    //struct stat st;
    String std_dir=directory+"/";
    String path_all=std_dir+"*";
    String namefile;
    HANDLE hFind = FindFirstFile(path_all, &ffd);

    do
    {
        if ( !strcmp( ffd.cFileName, "."   )) continue;
        if ( !strcmp( ffd.cFileName, ".."  )) continue;
        //if isn't a directory
        if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            //push
            out.push_back(ffd.cFileName);

    }
    while (FindNextFile(hFind, &ffd) != 0);

#endif

    return out;
}
std::vector<String> Path::getSubDirs() const
{

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

    while ((dirp = readdir(dp)) != NULL)
    {
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
    //struct stat st;
    String std_dir=directory+"/";
    String path_all=std_dir+"*";
    String namefile;
    HANDLE hFind = FindFirstFile(path_all, &ffd);

    do
    {
        if ( !strcmp( ffd.cFileName, "."   )) continue;
        if ( !strcmp( ffd.cFileName, ".."  )) continue;
        //if is a directory
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            //push
            out.push_back(ffd.cFileName);

    }
    while (FindNextFile(hFind, &ffd) != 0);

#endif

    return out;
}

String Path::getCanonicalPath(const String& path)
{
    String outPath(path);
    convertToCanonicalPath(outPath);
    return outPath;
}

static void trim(String& path)
{
	path = path.trim();
}

static void backslashToSlash(String& path)
{
	//utf 8 string raw
	char* rawstr = (char*)path.cStr();
	//for all bytes
	for (int i = 0; i != path.byteSize(); ++i)
	{
		if( rawstr[i] == '\\' )
		{
			rawstr[i] = '/';
		}
	}
}

static bool rootPathParser(String& path,String& rootName)
{
#ifdef  PLATFORM_UNIX
	String::ConstIterator it=path.find("/");
	rootName = "/";
	if (it != path.cend())
	{
		path = path.substr(it,path.cend());
		return true;
	}
#else
	String::ConstIterator it   = path.find(":");
	if (it != path.cend())
	{
		std::vector<String> tokens;
		//win path: <root>':''/'<path>
		path.split(":", tokens);
		//tokens[0] = <root>
		rootName = tokens[0]+":/";
		//tokens[1] ='/'<path>
		path     = tokens[1].substr(tokens[1].cbegin()+1, tokens[1].cend());
		return true;
	}
#endif
	return false;
}

static void resolveRelativePath(String& path)
{
	//replace ../
    std::vector<String> dirs;
	path.split("/", dirs);
	path = ""; //release
	for (uint i = 0; i<dirs.size() - 1; ++i)
	{
		if (dirs[i] != ".")
		{
			if (dirs[i + 1] != "..")
				path += dirs[i] + "/";
			else
				++i;
		}
	}
	if (dirs.size()>1)
	{
		if (dirs[dirs.size() - 1] != ".")
			if (dirs[dirs.size() - 1] != "..")
				path += dirs[dirs.size() - 1];
	}
	else
		path = dirs[0];
	//is a directory?
	String::ConstReverseIterator point = path.rfind(".");
	String::ConstReverseIterator flash = path.rfind("/");
	//is a file path
	if (flash > point) return;
	//if is a directory the path must end with the slash
	if (path[path.size() - 1] != '/') path += '/';
}

bool Path::convertToCanonicalPath(String& path)
{
	//delete spaces
	trim(path);
	//to unix format
	backslashToSlash(path);
	//is a root/absolute path?
	String root;
	bool absolute = rootPathParser(path, root);
    //replace "void path"
    path.replace("//","");
	//is "local"
    if(path.size()==0||path=="."||path=="./")
    {
        path="./";
        return false;
    }
	//resol the relative path
	resolveRelativePath(path);
    //replace is made a void path..
    if(path.size()==0)
    {
        //is an absolute path
        if(absolute)
            path=root+path;
        else //is relative
            path="./";
        return absolute;
    }
    //is an absolute path
    if(absolute)
        path=root+path;
    //
    return absolute;
}

Path Path::getRelativePathTo(const Path& path) const
{
    //from path
    auto from=path.getAbsolute();
    //to path
    auto to=getAbsolute();
    //calc path
#ifdef  PLATFORM_UNIX
    //dirs from
    std::vector<String> fromDirs;
    from.getDirectory().split("/",fromDirs);
    //dirs to
    std::vector<String> toDirs;
    to.getDirectory().split("/",toDirs);
    
    //vars
    auto maxLoop=Math::min(fromDirs.size(),toDirs.size());
    String newPath;
    size_t i=0;
    //search dif
    for(i=0;i!=maxLoop;++i)
    {
        if(fromDirs[i]!=toDirs[i]) break;
    }
    //back
    if(maxLoop-i)
    {
        newPath=String("../")*(maxLoop-i);
    }
    //next
    if(maxLoop<toDirs.size())
    {
        for(i=maxLoop;i!=toDirs.size();++i)
        {
            newPath+=toDirs[i]+"/";
        }
    }
    //add file
    if(to.getFilename().size())
    {
        newPath+=to.getFilename();
    }
    //return
    return newPath;
#elif defined(PLATFORM_WINDOW)
    //calc relative
	char szOut[MAX_PATH];
	String dirFrom = from.getDirectory();
	String dirTo = to.getPath();
	dirFrom.replaceAll("/", "\\");
	dirTo.replaceAll("/", "\\");
    PathRelativePathToA(szOut,
						dirFrom,
                        FILE_ATTRIBUTE_DIRECTORY,
                        dirTo,
                        FILE_ATTRIBUTE_NORMAL);
    //return
    return szOut;
#endif

}

Path Path::getRelativePath() const
{
    //get working directory
    Path workingDir(Application::instance()->appWorkingDirectory());
    //return
    return getRelativePathTo(workingDir);
}
Path Path::getAbsolute() const
{
    if(!isAbsolute()) return *this;
    //get absolute
    char szOut[PATH_MAX];
    realpath(getPath(),szOut);
    return szOut;
}



