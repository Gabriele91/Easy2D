from os import walk
from os import path  as ospath
from sys import argv


def read_all(path):
    with open(path, "rb") as readfile:
        return readfile.read()

def write_all(path,string):
    with open(path, "wb") as readfile:
        readfile.write(string)

def next_include(lines):
    size=len(lines)
    iret=-1
    for i in range(0,size):
        if lines[i].find("#include")!=-1:
            iret=i
    return iret

def first_define(lines):
    size=len(lines)
    for i in range(0,size):
        if lines[i].find("#define")!=-1:
            return i
    return -1

def in_h(path,nsname):
    sfile=read_all(path)
    #windows suck
    sfile=sfile.replace("\r\n","\n")
    #start lines
    lines=sfile.split("\n");
    #new list
    lines2=list()
    addedENS=False
    #append
    for line in reversed(lines):
        lines2.append(line)
        if (not addedENS) and line.find("#endif")!=-1:
            lines2.append("};")
            addedENS=True
    #original order
    lines=list(reversed(lines2))
    #find last include
    toadd=next_include(lines)
    #also search first define (#define FILE_H)
    if toadd<0:
        toadd=first_define(lines)
    #also raise an exception
    if toadd<0:
        raise "fuke file: "+path
    #append namespace
    lines=lines[:toadd+1] + ["namespace "+nsname+"{"] + lines[toadd+1:]
    #list to string
    sefile="\n".join(lines)
    #save
    write_all(path,sefile)


def in_cpp(path,nsname):
    #read
    sfile=read_all(path)
    #windows suck
    sfile=sfile.replace("\r\n","\n")
    #start lines
    lines=sfile.split("\n");
    #find last include
    toadd=next_include(lines)
    #append namespace
    if toadd<0:
        lines=["using namespace "+nsname+";"] + lines
    else: 
        lines=lines[:toadd+1] + ["using namespace "+nsname+";"] + lines[toadd+1:]
    #list to string
    sefile="\n".join(lines)
    #save
    write_all(path,sefile)



def namespace(folder,nsname):
    for root, dirs, files in walk(folder):
        for file_ in files:
            print(file_)
            name, ext = ospath.splitext(file_)     
            if ext == ".cpp":
                # CPP
                # call the proper function with:
                in_cpp(ospath.join(root,file_),nsname)
            elif ext == ".h":
                # H
                # call the proper function with:
                in_h(ospath.join(root,file_),nsname)
                
 
 
if __name__ == '__main__':
    path=None
    nsname=None
    
    if len(argv)<2:
        path=raw_input("get path: ")
    else:
        path=argv[1]

    if len(argv)<3:
        nsname=raw_input("get name: ")
    else:
        nsname=argv[2]

    namespace(path,nsname)