import sys
name=raw_input("Name files:")
nameupper=name.upper()

#make h file
ofile = open("include/"+name+".h","w")
ofile.write("#ifndef "+nameupper+"_H\n")
ofile.write("#define "+nameupper+"_H\n\n")
ofile.write("namespace Easy2D {\n\n\n\n")
ofile.write("};\n\n")
ofile.write("#endif")
ofile.close()
#make cppfile
ofile = open("src/"+name+".cpp","w")
ofile.write("#include <stdafx.h>\n")
ofile.write("#include <"+name+".h>\n")
ofile.write("///////////////////////\n")
ofile.write("using namespace Easy2D;\n")
ofile.write("///////////////////////\n\n")
ofile.close()