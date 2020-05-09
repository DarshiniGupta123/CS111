#!/usr/bin/python

import csv
import sys

from collections import defaultdict

totalBlocks = 0
totalInodes = 0
reservedBlock = 0

inodes = []

bFreeBlocks = []
iFreeInodes = []

indirect = []
direct = []

my_bit_block_map = {}

exitFlag = 0

firstInode = 0

def blockErrors(block_number, level, inode, offset):
    
    global my_bit_block_map, totalBlocks, exitFlag, reservedBlock
    
    if block_number != 0:
        
        if block_number < 0 or block_number > totalBlocks:
            sys.stdout.write("INVALID " + str(level) + str(block_number) + " IN INODE " + str(inode) + " AT OFFSET " + str(offset) + "\n")
            exitFlag = 2
        elif block_number < reservedBlock: #RESERVED BLOCK?
            sys.stdout.write("RESERVED " + str(level) + str(block_number) + " IN INODE " + str(inode) + " AT OFFSET " + str(offset) + "\n")
            exitFlag = 2
        elif block_number in my_bit_block_map:
            #sys.stdout.write("DUPLICATE " + str(level) + " " + str(block_number) + " IN INODE " + str(inode) + " AT OFFSET " + str(offset) + "\n")
            #exitFlag = 2
            mylist = [level, inode, offset]
            my_bit_block_map[block_number].append((level, inode, offset))
            exitFlag = 2
        else:
            my_bit_block_map[block_number] = [(level, inode, offset)]
        

def blocking():
    global inodes, my_bit_block_map, bFreeBlocks, indirect, exitFlag
    
    for inode in inodes:

        if inode[2] == 's' and int(row[10]) <= 60:
            continue
        else:
            for block in range(12, 24):
                if int(inode[block]) != 0:
                    blockErrors(int(inode[block]), "BLOCK ", int(inode[1]), 0)
            if int(inode[24]) != 0:
                blockErrors(int(inode[24]), "INDIRECT BLOCK ", int(inode[1]), 12)
            if int(inode[25]) != 0:
                blockErrors(int(inode[25]), "DOUBLE INDIRECT BLOCK ", int(inode[1]), 268)
            if int(inode[26]) != 0:
                blockErrors(int(inode[26]), "TRIPLE INDIRECT BLOCK ", int(inode[1]), 65804)


    for i in indirect:
        level = int(i[2])
        l = ""
        offset = 0
        if level == 1:
            l = "INDIRECT BLOCK "
            offset = 12
        if level == 2:
            l = "DOUBLE INDIRECT BLOCK "
            offset = 268
        if level == 3:
            l = "TRIPLE INDIRECT BLOCK "
            offset = 65804
        blockErrors(int(i[5]), l, int(i[1]), offset) 
    
    for block_number2 in range(8, totalBlocks):
        if block_number2 in bFreeBlocks and block_number2 in my_bit_block_map:
            sys.stdout.write("ALLOCATED BLOCK " + str(block_number2) + " ON FREELIST" + "\n")
            exitFlag = 2
        if block_number2 not in my_bit_block_map and block_number2 not in bFreeBlocks:
            sys.stdout.write("UNREFERENCED BLOCK " + str(block_number2) + "\n")
            exitFlag = 2
        if block_number2 in my_bit_block_map:
            if len(my_bit_block_map[block_number2]) > 1:
                #for level, inode, offset in my_bit_block_map[block_number2]:
                #print block_number2
                for b in my_bit_block_map[block_number2]:
                    #print b
                    sys.stdout.write("DUPLICATE " + str(b[0]) + str(block_number2) + " IN INODE " + str(b[1]) + " AT OFFSET " + str(b[2]) + "\n")
                exitFlag = 2
    
def main():
    global totalBlocks, totalInodes, inodes, bFreeBlocks, iFreeInodes, indirect, direct, exitFlag, my_bit_block_map, firstInode, reservedBlock   
    
    if len(sys.argv) != 2:
        print >> sys.stderr, "Unrecognized argument"
        sys.exit(1)
        
    fd = sys.argv[1]

    if fd < 0:
        print >> sys.stderr,  "Error with argument"
        sys.exit(1)

    try:
        csv_file = open(fd, 'r')
    except OSError:
        print >> sys.stderr,  "Error opening file"
        sys.exit(1)

    with csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        for row in csv_reader:
            if row[0] == "SUPERBLOCK":
                totalBlocks = int(row[1])
                totalInodes = int(row[2])
                inodeSize = int(row[4])
                blockSize = int(row[3])
                firstInode = int(row[7])
            elif row[0] == "GROUP":
                startingBlock = int(row[8])
            elif row[0] == "BFREE":
                bFreeBlocks.append(int(row[1]))
            elif row[0] == "IFREE":
                iFreeInodes.append(int(row[1]))
            elif row[0] == "INODE":
                inodes.append(row)
                
            elif row[0] == "DIRENT":
                direct.append(row)

            elif row[0] == "INDIRECT":
                indirect.append(row)
            
            else:
                print >> sys.stderr, "Unrecognized Type"
                sys.exit(1)

    reservedBlock = startingBlock + (totalInodes * inodeSize / blockSize)
            
    #DONE PARSING
    blocking()
            
    #INODE CHECKING
    aInodeList = []
    aInode = []
    freeInodes2 = iFreeInodes
    
    for inode in inodes:
        if int(inode[3]) != 0:
            if int(inode[1]) in iFreeInodes:
                sys.stdout.write("ALLOCATED INODE " + inode[1] + " ON FREELIST" + "\n")
                exitFlag = 2
                freeInodes2.remove(int(inode[1]))
            aInodeList.append(int(inode[1]))
            aInode.append(inode)
        elif int(inode[3]) == 0 and int(inode[1]) not in iFreeInodes:
            sys.stdout.write("UNALLOCATED INODE " + inode[1] + " NOT ON FREELIST" + "\n")
            exitFlag = 2
            freeInodes2.append(int(inode[1]))

        
    for inode in range(firstInode, totalInodes):
        if inode not in iFreeInodes and inode not in aInodeList:
            sys.stdout.write("UNALLOCATED INODE " + str(inode) + " NOT ON FREELIST" + "\n")
            exitFlag = 2
            freeInodes2.append(int(inode))

    #DIRECT CHECKING
    inode_ref_array = defaultdict(int)
    inode_par_array = defaultdict(int)
    
    for inode in inodes:
        type = inode[2]
        if type != 'd':
            continue
        par_ino = int(inode[1])
        #print par_ino
        for directory in direct:
            if int(directory[1]) == par_ino:
                child_ino = int(directory[3])
                child_name = directory[6]
                if child_ino < 0 or child_ino > totalInodes:
                    sys.stdout.write("DIRECTORY INODE " + str(directory[1]) + " NAME " + str(child_name) + " INVALID INODE " + str(child_ino) + "\n")
                    exitFlag = 2
                if child_ino in freeInodes2:
                    sys.stdout.write("DIRECTORY INODE " + str(directory[1]) + " NAME " + str(child_name) + " UNALLOCATED INODE " + str(child_ino) + "\n")
                    exitFlag = 2
                if child_name == "'.'" and child_ino != par_ino:
                    sys.stdout.write("DIRECTORY INODE " + str(directory[1]) + " NAME " + str(child_name) + " LINK TO INODE " + str(child_ino) + "SHOULD BE" + str(par_ino) + "\n")
                    exitFlag = 2
                if inode_par_array[child_ino] == 0:
                    inode_par_array[child_ino] = par_ino
                elif child_name != "'..'" and child_name != "'.'":
                    inode_par_array[child_ino] = par_ino
                inode_ref_array[child_ino] += 1
                

    #print inode_par_array
    #print inode_ref_array
    for inode in inodes:
        type = inode[2]
        par_ino = int(inode[1])
        if inode_ref_array[par_ino] != int(inode[6]):
            sys.stdout.write("INODE " + str(inode[1]) + " HAS " + str(inode_ref_array[par_ino]) + " LINKS BUT LINKCOUNT IS " + str(inode[6]) + "\n")
            exitFlag = 2
        if type != 'd':
            continue
        for directory in direct:
            if int(directory[1]) == par_ino:
                child_ino = int(directory[3])
                child_name = directory[6]
                
                if child_name == "'..'" and child_ino != inode_par_array[par_ino]:
                    sys.stdout.write("DIRECTORY INODE " + str(directory[1]) + " NAME " + str(child_name) + " LINK TO INODE " + str(child_ino) + " SHOULD BE " + str(inode_par_array[par_ino]) + "\n")
                    exitFlag = 2

        
    
    exit(exitFlag)    

if __name__ == "__main__":
    main()
