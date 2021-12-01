import os
import time

count = 100
total = 0

os.system('./bin/build.sh ../tests/siptest.sip')
cmd = './siptest.sip'

for i in range(count):
    start = time.clock_gettime(time.CLOCK_REALTIME)
    os.system(cmd + ' > out.txt')
    end = time.clock_gettime(time.CLOCK_REALTIME)
    total += end - start

print(total / count)
