#!/bin/python
# -*- coding: UTF-8 -*-
import re
# import os
# import subprocess
import get_constants
f = open('pattern2', 'r')
text = f.read()
devices = re.findall('\[devices\]\n(.*?)\n', text)
print devices
volume = re.findall('\[volume\]\n(.*?)\n', text)
print volume
disks = re.findall('#\n#\n\n(.*?) ', text)
print disks
sizes = re.findall('\[block_sizes\]\n(.*?)\n', text)
print sizes
partitions = devices[0].split(' ')
mod = re.findall('#\n#\n\n[0-9]* (.*?) ', text)
print mod
sequence = re.findall('scheme=(.*?)\n', text)
size_disk = int(volume[0][:-1])
byte = (volume[0][-1])
print size_disk
print byte
if mod[0] == 'lrc' and sequence[0] != 'optimum':
    # get_constants.main.scheme=sequence[0]
    get_constants.main()
#     get_constants.get_scheme(sequence[0])
#     get_constants.main()
    # subprocess.Popen(['python', 'get_constants.py'])
    # os.startfile(r'/home/murrm/PycharmProjects/raid/get_constants.py')
    #
    # print 'lol'
# os.system('python get_constants.py')
if len(partitions) < int(disks[0]):
    print 'Error, partitions < disks'
    exit()
parts = []
for i in xrange(int(disks[0])):
    parts.append(partitions[i])
b = ' '.join(parts)
c = sizes[0].split(' ')
if byte == 'G'or byte == 'g':
    size_block = int(disks[0])*2*1024*1024*size_disk
if byte == 'M' or byte == 'm':
    size_block = int(disks[0])*2*1024*size_disk
if byte == 'K' or byte == 'k':
    size_block = int(disks[0])*2*size_disk

groups = re.findall('groups=(.*?),',text)

print groups

table = open('TABLE', 'a')
for i in xrange(len(c)):
    table.write('0 %s insane %s %s %s recover 1 %s \n' % (size_block, mod[0], int(disks[0]), int(c[i]), b))
table.close()
f.close()
# print(result)
# массив из данных в скобках {}
# print size_disk
# размер одного диска
# print byte
# еденицы измерения диска-G,M,K
# print disks[0]
# количество дисков
# print size_block
# размер всего устройства=2*1024*N...
# print mod[0]
# используемый модуль(lrc,raid 6,...)
# print sizes
# массив с размерами блоков
# print partitions
# массив с разделами
# print b
# используемые разделы
# print c
# массив с используемыми размерами блоков

# size_block=int(disks[0])*2
# for line in f:
#     print line
#     result = re.findall

# disks = file.readline().strip()
# print file.readlines()
# for line in file:
#     print line
# data = re.split(r'\n{2,}',file )
# print data
