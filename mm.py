#!/usr/bin/env python2
"""
mm, (yet another) makefile maker
Version 0.1 

The Makefile template used in this script is 
adopted from the sample Makefile in FLTK 1.3 Documentation

"""

import os, re
import sys
import time

macros = {\
'objs' : '',
'srcs' : '', 
'cxxflags' : '', 
'ldflags' : '', 
'ldstatic' : '', 
'target' : 'main',
'debug' : '-g', 
'compiler': 'g++', 
'dependencies': '',
'time' : time.asctime()}

makefile_name = 'Makefile'

makefile_template = """\
# Makefile
# Generated by Makefile Generater at {time}

TARGET = {target}
CC = {compiler}
OBJS = {objs}
SRCS = {srcs}
CXXFLAGS = {cxxflags}
LDFLAGS = {ldflags}
LDSTATIC = {ldstatic}
DEBUG = {debug}
LINK = $(CC)

# LINK = $(shell fltk-config --use-gl --compile)

.SUFFIXES: .o .cpp
%.o: %.cpp
\t$(CC) $(DEBUG) $(CXXFLAGS) -c $<

all: $(TARGET)
\t$(LINK) -o $(TARGET) $(OBJS) $(LDSTATIC)

$(TARGET): $(OBJS)
{dependencies}

clean:
\t@rm $(OBJS) -vf
\t@rm $(TARGET) -vf
\t@echo "Clean successfully"

# end of Makefile
"""

# get current working directory
pwd = os.getenv('PWD')
# why not just switch to pwd ?
os.chdir(pwd)

# global configs
recursive = False

def get_filelist(suffixes=None):
    if not suffixes:
        suffixes = ['cpp', 'c', 'cxx']
    if not recursive:
        filelist = os.listdir(pwd)
        return [name for name in filelist if name.split('.')[-1].lower() in suffixes]
    else:
        filelist = []
        for root, dirs, files in os.walk('.'):
            filelist += [os.path.join(root, file) for file in files if file.split('.')[-1].lower() in suffixes]
        return filelist

def get_direct_dp(filename):
    dplist = []
    with open(os.path.join(pwd, filename)) as f:
        for line in f:
            match = re.search(r'#include\s+"(.+)"', line)
            if match:
                dplist.append(match.group(1))
    return dplist

# should be able to detect circular dependency ?
def get_dp(srclist):
    headerlist = get_filelist(['h', 'hpp'])
    filedp = dict()
    srcdp = dict()
    # should separate circular detection
    def dfs(dpset, fileno, filename):
        dpset.add(filename)
        fileno[filename] = counter + 1
        counter += 1
        for dp in filedp[filename]:
            if dp not in dpset:
                dfs(dpset, fileno, dp)
        m = min([fileno[f] for f in filedp[filename]])
        return m

    # initialization
    for file in headerlist + srclist:
        filedp[file] = get_direct_dp(file)
    for file in srclist:
        srcdp[file] = filedp[file]
        for dp in srcdp[file]:
            for dpdp in filedp[dp]:
                if dpdp not in srcdp[file]:
                    srcdp[file].append(dpdp)
    return srcdp

def fill_template(template, macros_dict):
    for key, value in macros_dict.iteritems():
        macro = '{%s}' % key
        template = template.replace(macro, value, 1)
    return template

def generate_makefile():
    if os.path.exists(makefile_name):
        # backup original makefile
        os.rename(makefile_name, makefile_name + '.backup')
        print 'Old Makefile is backuped'
    srcs = get_filelist()
    objs = [name[:name.rfind('.')] + '.o' for name in srcs]
    dpdict = get_dp(srcs)

    macros['srcs'] = ' '.join(srcs)
    macros['objs'] = ' '.join(objs)
    macros['dependencies'] = '\n'.join(['%s : %s' % (obj, ' '.join(dp)) for obj, dp in dpdict.iteritems()])

    with open(makefile_name, 'w') as f:
        f.write(fill_template(makefile_template, macros))

    print 'Generating Makefile successful'

def main():
    if len(sys.argv) > 1:
        # the only config
        if '-r' in sys.argv:
            recursive = True
        if sys.argv[1] == 'help':
            print 'help yourself, read the code!'
        elif sys.argv[1] == 'tolower':
            rename_lower()
        elif sys.argv[1] == 'include-tolower':
            rename_lower_head_file_in_text()
        elif sys.argv[1] == 'comp4411':
            """
            using fltk and opengl
            probably also using fltk_images
            """
            print 'Generating Makefile for COMP4411'
            macros['cxxflags'] = '$(shell fltk-config --use-gl --use-images --cxxflags ) -I. '
            macros['ldflags'] = '$(shell fltk-config --use-gl --use-images --ldflags ) '
            macros['ldstatic'] = '$(shell fltk-config --use-gl --use-images --ldstaticflags ) '
            generate_makefile()
        else:
            print "Unknown parameters"
    else:
        generate_makefile()

def rename_lower_head_file_in_text():
    filelist = get_filelist(['cpp', 'h', 'hpp', 'c', 'cxx'])

    """
    for the first time, know the meaning of r before a string
    it is 'raw string' that backslash is not handled in the special way
    *, ?, +, and add an extra ? to mean non-greedy
    almost all chars are treated as their special meaning if they ever have some special meanings.
    which is different from the case in vim
    ^ start of string/line
    $ end of string/line
    {} exact how many times of a pattern
    also, {}? is possible
    special chars will lose their specail meanings in []
    [^] complement
    """
    myre = re.compile(r'(#include\s*".+?")')
    # group() or group(0) mean return the whole string
    lower_func = lambda s : s.group(0).lower()
    content = None
    for fpath in filelist:
        with open(fpath) as f:
            content = f.read()
        if myre.search(content):
            """ most research pattern is cached, but still, try using compiled version """
            content = myre.sub(lower_func, content)
            with open(fpath, 'w') as f:
                f.write(content)


def rename_lower():
    """
    You're stupid, should write a special script for renaming
    """
    filelist = get_filelist(['cpp', 'h', 'hpp', 'c', 'cxx'])
    print filelist
    for file in filelist:
        os.rename(file, file.lower())
        
if __name__ == '__main__':
    main()
