#! /usr/bin/env python
# -*- coding: utf-8 -*-

# roughnessToFoam.py
#
# 1. uses first *.map file as input - from case directory
# 2. runs roughnessToFoam.C --> which writes 0/z0 file (expecting a basic uniform value z0 file with all the boundary patches to exist already)
# 3. copies the terrain_solid z0 nonuniform values to nut[terrain_solid][z0] 

from PyFoam.RunDictionary.ParsedParameterFile   import ParsedParameterFile
from argparse import ArgumentParser
from subprocess import call
import os, sys, glob

def remove_extra_spaces(src):
    """
    read src and write a new version to the same file without extra spaces.

    The new version contains only a single space between every item.
    src line: "a   b   c\n"
    new src line: "a b c\n"
    """
    with open(src) as f:
        lines = f.readlines()
    with open(src, 'w+') as f:
        f.writelines(' '.join(l.split()) + '\n' for l in lines)

def main(args):
    # 0 - reading input
    cwd = os.getcwd()
    os.chdir(args.target)
    if len(glob.glob("*.map")) > 1:
        print "error: more then a single map file in the directory, please delete all except one"
        raise SystemExit
    mapFileName = glob.glob("*.map")[0]
    remove_extra_spaces(mapFileName)
    from pdb import set_trace
    #set_trace()
    
    # 0.5 creating z0 dummy file
    os.system("cp -r 0/p 0/z0")
    z0Dict = ParsedParameterFile("0/z0")
    z0Dict.header["object"] = "z0"
    for b in z0Dict["boundaryField"]:
        print b
        if type(b) is str:
            if b.find("terrain")>-1 or b.find("ground")>-1:
                print "found terrain/ground in z0 at patch " + b
                z0Dict["boundaryField"][b]["value"]="uniform 0"
                z0Dict["boundaryField"][b]["type"]="fixedValue"
    z0Dict["dimensions"] = "[ 0 1 0 0 0 0 0]"
    z0Dict.writeFile()

    # 1 - running roughnessToFoam.C
    callString = "( echo %s; echo %d; echo %d; echo %d; echo %d; echo %d; echo %d) | roughnessToFoam" % (mapFileName, args.offsetX, args.offsetY, args.point_ax, args.point_ay, args.point_bx, args.point_by)
    os.system(callString)

    # 2 - reading modified z0 file - made with roughnessToFoam.C
    z0Dict = ParsedParameterFile("0/z0")
    for b in z0Dict["boundaryField"]:
        if type(b) is str:
            if b.find("terrain")>-1:
                # TODO - save each different *terrain*'s z0 and place in matching nut *terrain*. at the moment - only one patch with "terrain" in it is expected, and one with "ground". 
                z0Terrain = z0Dict["boundaryField"][b]["value"]
                print "taken *terrain* z0 from %s" % b
            if b.find("ground")>-1:
                z0Ground = z0Dict["boundaryField"][b]["value"]
                print "taken *ground* z0 from %s" % b

    # 3 - writing z0 into nut file
    nutDict = ParsedParameterFile("0/nut")
    for b in nutDict["boundaryField"]:
        if type(b) is str:
            for c in nutDict["boundaryField"][b]:
                if "z0" in c:
                        if b.find("terrain")>-1:
                            nutDict["boundaryField"][b]["z0"] = z0Terrain
                        if b.find("ground")>-1:
                            nutDict["boundaryField"][b]["z0"] = 0
                            nutDict["boundaryField"][b]["z0"] = z0Ground
                        
    nutDict.writeFile()
    os.chdir(cwd)

if __name__ == '__main__':
    # reading arguments
    parser = ArgumentParser()
    parser.add_argument('--offsetX', type=float, default=0, help='x offset of mesh surface vs. z0 polygon map file (WAsP format)')
    parser.add_argument('--offsetY', type=float, default=0, help='y offset of mesh surface vs. z0 polygon map file (WAsP format)')
    parser.add_argument('--point_ax',type=float, default=1, help='x of point (1,0) for mesh surface vs. z0 polygon map file (WAsP format)')
    parser.add_argument('--point_ay', type=float, default=0,help='y of point (1,0) for mesh surface vs. z0 polygon map file (WAsP format)')
    parser.add_argument('--point_bx',type=float, default=0, help='x of point (0,1) for mesh surface vs. z0 polygon map file (WAsP format)')
    parser.add_argument('--point_by',type=float, default=1, help='y of point (0,1) for mesh surface vs. z0 polygon map file (WAsP format)')
    parser.add_argument('--target',default='./', help='location of directory in which the case is located')
    args = parser.parse_args(sys.argv[1:])
    main(args)
