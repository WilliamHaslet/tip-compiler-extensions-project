import os
from os import *
from os.path import isfile, join
import time

def editTime(file):
    return os.stat(file).st_mtime

def compile1():
    os.system('./bin/build.sh ../tests/siptest.sip')
    print('Running...')
    os.system('./siptest.sip')
    print('Done')

def compile2():
    print('Compiling...')
    os.system('./quickComp.sh')
    print('Done')
    compile1()

path = '.'
files1 = ['../tests/siptest.sip']
files2 = ['src/codegen/CodeGenFunctions.cpp']
editTimes1 = []
editTimes2 = []
fileCount1 = len(files1)
fileCount2 = len(files2)

for f in files1:
    editTimes1.append(editTime(f))

for f in files2:
    editTimes2.append(editTime(f))

needsCompile = False

while True:
    time.sleep(0.5)
    for i in range(fileCount1):
        if editTimes1[i] != editTime(files1[i]):
            editTimes1[i] = editTime(files1[i])
            needsCompile = True
    if needsCompile:
        compile1()
        needsCompile = False

    for i in range(fileCount2):
        if editTimes2[i] != editTime(files2[i]):
            editTimes2[i] = editTime(files2[i])
            needsCompile = True
    if needsCompile:
        compile2()
        needsCompile = False
