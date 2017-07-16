from os import walk
from os import path
from os import remove
import sys

for (dirpath, dirnames, filenames) in walk(sys.argv[1]):
    for filename in filenames:
        if path.splitext(filename)[1] == '.cpp':
            print("delete", path.join(dirpath,filename))
            remove(path.join(dirpath,filename))
